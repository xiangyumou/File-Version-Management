/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "saver.h"
#include "logger.h"
#include <cctype>
#include <fstream>
#include <sstream>

// dataNode implementation
dataNode::dataNode() = default;

dataNode::dataNode(unsigned long long name_hash, unsigned long long data_hash,
                   std::vector<std::pair<double, double>>& data, int n) {
    this->name_hash = name_hash;
    this->data_hash = data_hash;
    this->len = static_cast<int>(data.size()) / n;
    this->data = data;
}

// Helper to get logger reference
ffvms::ILogger& Saver::get_logger_ref() {
    if (logger_) return *logger_;
    return Logger::get_logger();
}

// Saver implementation
template <class T>
unsigned long long Saver::get_hash(T& s) {
    unsigned long long seed = 13331, hash = 0;
    for (auto& ch : s) {
        hash = hash * seed + ch;
    }
    return hash;
}

bool Saver::load_file() {
    std::ifstream in(data_file);
    if (!in.good()) {
        get_logger_ref().log("load_file: No data file.", ffvms::LogLevel::WARNING, __LINE__);
        return false;
    }
    mp.clear();
    unsigned long long name_hash, data_hash, len;
    std::vector<std::pair<double, double>> data;
    while (in >> name_hash) {
        data.clear();
        in >> data_hash >> len;
        if (in.eof()) {
            mp.clear();
            get_logger_ref().log("Read interrupted, please check data integrity.", ffvms::LogLevel::WARNING, __LINE__);
            return false;
        }
        for (unsigned long long i = 0; i < len * N; i++) {
            double a, b;
            in >> a >> b;
            data.push_back(std::make_pair(a, b));
        }
        save_data(name_hash, data_hash, data);
    }
    return true;
}

void Saver::save_data(unsigned long long name_hash, unsigned long long data_hash,
                      std::vector<std::pair<double, double>> data) {
    if (mp.count(name_hash)) {
        mp.erase(mp.find(name_hash));
    }
    mp[name_hash] = dataNode(name_hash, data_hash, data, N);
}

int Saver::read(std::string& s) {
    size_t cur = 0;
    int d = 0;
    for (; cur < s.size() && !isdigit(s[cur]); cur++);
    for (; cur < s.size() && isdigit(s[cur]); cur++) {
        d = d * 10 + s[cur] - '0';
    }
    s.erase(s.begin(), s.begin() + cur + 1);
    return d;
}

Saver::Saver() : logger_(nullptr) {
    load_file();
}

Saver::Saver(ffvms::ILogger* logger) : logger_(logger) {
    load_file();
}

Saver::~Saver() {
    std::ofstream out(data_file);
    for (auto& data : mp) {
        dataNode& dn = data.second;
        out << data.first << ' ' << dn.data_hash << ' ' << dn.len;
        for (auto& pr : dn.data) {
            out << ' ' << pr.first << ' ' << pr.second;
        }
        out << '\n';
    }
}

// IStorage interface implementation
bool Saver::save(const std::string& name, const ffvms::DataTable& content) {
    std::string data;
    data += std::to_string(content.size());
    for (const auto& data_block : content) {
        data += " " + std::to_string(data_block.size());
        for (const auto& dt : data_block) {
            data += " " + std::to_string(dt.size()) + " " + dt;
        }
    }
    std::vector<int> sequence;
    for (auto& it : data) {
        sequence.push_back(static_cast<int>(it));
    }
    std::vector<std::pair<double, double>> res;
    encrypt_sequence(sequence, res);
    unsigned long long name_hash = get_hash(name);
    unsigned long long data_hash = get_hash(data);
    save_data(name_hash, data_hash, res);
    return true;
}

// IStorage interface implementation
bool Saver::load(const std::string& name, ffvms::DataTable& content, bool mandatory_access) {
    unsigned long long name_hash = get_hash(name);
    if (!mp.count(name_hash)) {
        get_logger_ref().log("Failed to load data. No data named " + name + " exists.", ffvms::LogLevel::WARNING, __LINE__);
        return false;
    }
    dataNode& data = mp[name_hash];
    std::vector<int> sequence;
    decrypt_sequence(data.data, sequence);
    if (get_hash(sequence) != data.data_hash) {
        get_logger_ref().log("Data failed to pass integrity verification.", ffvms::LogLevel::WARNING, __LINE__);
        if (!mandatory_access) return false;
    }
    std::string str;
    for (auto& it : sequence) {
        str.push_back(static_cast<char>(it));
    }
    
    content.clear();
    int block_num, data_num, data_len;
    block_num = read(str);
    for (int i = 0; i < block_num; i++) {
        content.push_back(std::vector<std::string>());
        data_num = read(str);
        for (int j = 0; j < data_num; j++) {
            data_len = read(str);
            if (str.size() < static_cast<size_t>(data_len)) {
                get_logger_ref().log("Failed to load data. Data corrupted.", ffvms::LogLevel::WARNING, __LINE__);
                return false;
            }
            content.back().push_back(std::string(str.begin(), str.begin() + data_len));
            str.erase(str.begin(), str.begin() + data_len);
        }
    }
    return true;
}

bool Saver::is_all_digits(const std::string& s) {
    if (s.empty()) return false;
    for (auto& ch : s) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

unsigned long long Saver::str_to_ull(const std::string& s) {
    unsigned long long res = 0;
    for (auto& ch : s) {
        res = res * 10 + ch - '0';
    }
    return res;
}

Saver& Saver::get_saver() {
    static Saver saver;
    return saver;
}
