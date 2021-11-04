/* Author: MuXiangyu, Chant Mee */

#ifndef SAVER_CPP
#define SAVER_CPP

#include "logger.cpp"
#include "encryptor.cpp"
#include <vector>
#include <string>
#include <sstream>
#include <map>

typedef std::vector<std::vector<std::string>> vvs;

class Saver : private Encryptor {
private:
    struct dataNode {
        unsigned long long name_hash, data_hash;
        int len;
        std::vector<std::pair<double, double>> data;

        dataNode();
        dataNode(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> &data);
    };

    std::string data_file = "data.chm";
    std::map<unsigned long long, dataNode> mp;
    Logger logger = Logger::get_logger();

    template <class T>
    unsigned long long get_hash(T &s);

    bool load_file();

    /**
     * 在map中的存储方式
     * 以name_hash作为主键，会检索出一个dataNode，里面包括了name_hash, data_hash, len, data.
     * 其中 len为data的pair的对数 / N
    */
    void save_data(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> data);

public:
    Saver();

    /**
     * 保存的格式：
     * name_hash data_hash len 后面跟len对浮点数
    */
    ~Saver();

    /**
     * data字符串的格式:
     * 数据块的个数 
     * 每个数据块中数据的个数
     * 数据的长度
     * 数据的字符表示
     * 每个单元之间都用空格隔开
    */
    bool save(std::string name, std::vector<std::vector<std::string>> &content);
    bool load(std::string name, std::vector<std::vector<std::string>> &content, bool mandatory_access = false);

    static Saver& get_saver();
};



                        /* ======= class Saver ======= */
Saver::dataNode::dataNode() = default;
Saver::dataNode::dataNode(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> &data) : name_hash(name_hash), data_hash(data_hash), len(data.size() / N), data(data) {}

template <class T>
unsigned long long Saver::get_hash(T &s) {
    unsigned long long seed = 13331, hash = 0;
    for (auto &ch : s) {
        hash = hash * seed + ch;
    }
    return hash;
}

bool Saver::load_file() {
    std::ifstream in(data_file);
    if (!in.good()) {
        logger.log("load_file: No data file.", Logger::WARNING, __LINE__);
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
            logger.log("Read interrupted, please check data integrity.", Logger::WARNING, __LINE__);
            return false;
        }
        for (int i = 0; i < len * N; i++) {
            double a, b;
            in >> a >> b;
            data.push_back(std::make_pair(a, b));
        }
        save_data(name_hash, data_hash, data);
    }
    return true;
}

/**
 * 在map中的存储方式
 * 以name_hash作为主键，会检索出一个dataNode，里面包括了name_hash, data_hash, len, data.
 * 其中 len为data的pair的对数 / N
*/
void Saver::save_data(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> data) {
    if (mp.count(name_hash)) {
        mp.erase(mp.find(name_hash));
    }
    mp[name_hash] = dataNode(name_hash, data_hash, data);
}

Saver::Saver() {
    load_file();
}

/**
 * 保存的格式：
 * name_hash data_hash len 后面跟len对浮点数
*/
Saver::~Saver() {
    std::ofstream out(data_file);
    for (auto &data : mp) {
        dataNode &dn = data.second;
        out << data.first << ' ' << dn.data_hash << ' ' << dn.len;
        for (auto &pr : dn.data) {
            out << ' ' << pr.first << ' ' << pr.second;
        }
        out << '\n';
    }
}

/**
 * data字符串的格式:
 * 数据块的个数 
 * 每个数据块中数据的个数
 * 数据的长度
 * 数据的字符表示
 * 每个单元之间都用空格隔开
*/
bool Saver::save(std::string name, std::vector<std::vector<std::string>> &content) {
    std::stringstream ss;
    ss << content.size();
    for (auto &data_block : content) {
        ss << ' ' << data_block.size();
        for (auto &data : data_block) {
            ss << ' ' << data.size();
            for (auto &ch : data) {
                ss << ' ' << ch;
            }
        }
    }
    ss << '\n';
    std::string data;
    std::getline(ss, data);
    std::vector<int> sequence;
    for (auto &it : data) {
        sequence.push_back((int)it);
    }
    std::vector<std::pair<double, double>> res;
    encrypt_sequence(sequence, res);
    unsigned long long name_hash = get_hash(name);
    unsigned long long data_hash = get_hash(data);
    save_data(name_hash, data_hash, res);
    return true;
}

bool Saver::load(std::string name, std::vector<std::vector<std::string>> &content, bool mandatory_access) {
    unsigned long long name_hash = get_hash(name);
    if (!mp.count(name_hash)) {
        logger.log("Failed to load data. No data named A exists. ", Logger::WARNING, __LINE__);
        return false;
    }
    dataNode &data = mp[name_hash];
    std::vector<int> sequence;
    decrypt_sequence(data.data, sequence);
    if (get_hash(sequence) != data.data_hash) {
        logger.log("Data failed to pass integrity verification.", Logger::WARNING, __LINE__);
        if (!mandatory_access) return false;
    }
    std::string str;
    for (auto &it : sequence) {
        str.push_back(it);
    }
    std::stringstream ss(str);
    /**
     * data字符串的格式:
     * 数据块的个数 
     * 每个数据块中数据的个数
     * 数据的长度
     * 数据的字符表示
     * 每个单元之间都用空格隔开
    */
    content.clear();
    int block_num, data_num, data_len;
    std::string tmp;
    ss >> block_num;
    for (int i = 0; i < block_num; i++) {
        content.push_back(std::vector<std::string>());
        ss >> data_num;
        for (int j = 0; j < data_num; j++) {
            ss >> data_len;
            tmp.clear();
            for (int k = 0; k < data_len; k++) {
                char ch;
                ss >> ch;
                tmp.push_back(ch);
            }
            content.back().push_back(tmp);
        }
    }
    return true;
}

Saver& Saver::get_saver() {
    static Saver saver;
    return saver;
}

#endif