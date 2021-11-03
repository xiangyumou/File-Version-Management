#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define ll std::cerr << "====1111====" << '\n';
#define rr std::cerr << "====2222====" << '\n';
#define wa std::cerr << "====WARN====" << '\n';

typedef std::vector<std::vector<std::string>> vvs;

class Encryptor {
protected:
static const int N = 1 << 10;
private:
    static const char PLACEHOLDER = '\0';

    struct Complex {
        double a, b;
        Complex() {}
        Complex(double a, double b) : a(a), b(b) {}
        Complex operator+(const Complex &r) const {
            return Complex(a + r.a, b + r.b);
        }
        Complex operator-(const Complex &r) const {
            return Complex(a - r.a, b - r.b);
        }
        Complex operator*(const Complex &r) const {
            return Complex(a * r.a - b * r.b, a * r.b + b * r.a);
        }
    } buf[N << 1], block[N];

    void fft(Complex a[], int n, int type) {
        const static double Pi = acos(-1.0);
        if (n == 1) return;
        int m = n >> 1;
        for (int i = 0; i < m; i++) {
            buf[i] = a[i << 1];
            buf[i + m] = a[i << 1 | 1];
        }
        memcpy(a, buf, sizeof(Complex) * n);
        Complex *a1 = a, *a2 = a + m;
        fft(a, m, type);
        fft(a + m, m, type);
        Complex wn = Complex(1, 0), u = Complex(cos(2 * Pi / n), type * sin(2 *Pi / n));
        for (int i = 0; i < m; i++) {
            Complex t = wn * a[m + i];
            wn = wn * u;
            buf[i] = a[i] + t;
            buf[i + m] = a[i] - t;
        }
        memcpy(a, buf, sizeof(Complex) * n);
    }

    bool encrypt_block(std::vector<std::pair<double, double>> &res) {
        fft(block, N, 1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back(std::make_pair(block[i].a, block[i].b));
        }
        return true;
    }

    bool decrypt_block(std::vector<int> &res) {
        fft(block, N, -1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back((int)(block[i].a / N + 0.5));
            if (block[i].a < 0.0 && std::abs(block[i].a) > 1e-2) res.back()--;
        }
        return true;
    }

protected:

    bool encrypt_sequence(std::vector<int> &sequence, std::vector<std::pair<double, double>> &res) {
        int len = sequence.size(), idx = 1;
        while (sequence.size() % N != 0) sequence.push_back(PLACEHOLDER);
        memset(block, 0, sizeof block);
        block[0].a = len;
        res.clear();
        std::vector<std::pair<double, double>> tmp;
        for (auto &it : sequence) {
            block[idx++].a = it;
            if (idx == N) {
                encrypt_block(tmp);
                res.insert(res.end(), tmp.begin(), tmp.end());
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        return true;
    }

    bool decrypt_sequence(std::vector<std::pair<double, double>> &sequence, std::vector<int> &res) {
        if (sequence.size() % N != 0) return false;
        memset(block, 0, sizeof block);
        int idx = 0;
        res.clear();
        int len = -1;
        std::vector<int> tmp;
        for (auto &it : sequence) {
            block[idx].a = it.first;
            block[idx++].b = it.second;
            if (idx == N) {
                decrypt_block(tmp);
                if (len != -1) {
                    res.insert(res.end(), tmp.begin(), tmp.end());
                } else {
                    len = tmp.front();
                    res.insert(res.end(), tmp.begin() + 1, tmp.end());
                }
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        res.erase(res.begin() + len, res.end());
        return true;
    }
} ec;

class Saver : private Encryptor {
private:
    struct dataNode {
        unsigned long long name_hash, data_hash;
        int len;
        std::vector<std::pair<double, double>> data;

        dataNode() = default;
        dataNode(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> &data) : name_hash(name_hash), data_hash(data_hash), len(data.size() / N), data(data) {}
    };

    std::string data_file = "data.chm";
    std::map<unsigned long long, dataNode> mp;

    template <class T>
    unsigned long long get_hash(T &s) {
        unsigned long long seed = 13331, hash = 0;
        for (auto &ch : s) {
            hash = hash * seed + ch;
        }
        return hash;
    }

    bool load_file() {
        std::ifstream in(data_file);
        if (!in.good()) return false;
        mp.clear();
        unsigned long long name_hash, data_hash, len;
        std::vector<std::pair<double, double>> data;
        while (in >> name_hash) {
            data.clear();
            in >> data_hash >> len;
            if (in.eof()) {
                mp.clear();
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
    void save_data(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> data) {
        if (mp.count(name_hash)) {
            mp.erase(mp.find(name_hash));
        }
        mp[name_hash] = dataNode(name_hash, data_hash, data);
    }

public:
    Saver() {
        load_file();
    }

    /**
     * 保存的格式：
     * name_hash data_hash len 后面跟len对浮点数
    */
    ~Saver() {
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
    bool save(std::string name, std::vector<std::vector<std::string>> &content) {
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

    bool load(std::string name, std::vector<std::vector<std::string>> &content, bool mandatory_access = false) {
        unsigned long long name_hash = get_hash(name);
        if (!mp.count(name_hash)) {
            return false;
        }
        dataNode &data = mp[name_hash];
        std::vector<int> sequence;
        decrypt_sequence(data.data, sequence);
        if (get_hash(sequence) != data.data_hash && !mandatory_access) {
            return false;
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
} sa;

std::string get_rand_str() {
    std::string s;
    for (int i = 0; i < rand() % 10 + 1; i++) {
        s.push_back(rand() % 26 + 'a');
    }
    return s;
}

int main() {
    srand(time(0));
    vvs content;
    for (int i = 0; i < 3; i++) {
        content.push_back(std::vector<std::string>());
        for (int j = 0; j < 3; j++) {
            content.back().push_back(get_rand_str());
            std::cout << content.back().back() << '\n';
        }
        puts("");
    }
    sa.save("second", content);
    std::cout << sa.load("second", content) << '\n';
    for (int i = 0; i < content.size(); i++) {
        for (int j = 0; j < content[i].size(); j++) {
            std::cout << content[i][j] << '\n';
        }
        puts("");
    }
    return 0;
}