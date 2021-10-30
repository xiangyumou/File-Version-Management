#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define wa std::cerr << "----WARN----" << '\n';
#define ll std::cerr << "----1111----" << '\n';
#define rr std::cerr << "----2222----" << '\n';

class Logger {
private:
    std::string log_file = "Logger.log";
    std::vector<std::string> logs;

    std::string get_time();
    ~Logger();

public:
    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    static Logger& get_logger();

    void log(std::string content);
};

// 获取新节点 修改节点信息 获取节点信息 删除节点

class NodeManager {
private:
    class Node {
        public:
            std::string name;
            std::string create_time;
            std::string update_time;
            std::string content;

            std::string get_time() {
                static char t[100];
                time_t timep;
                time(&timep);
                struct tm* p = gmtime(&timep);
                sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
                return std::string(t);
            }

            Node() = default;
            Node(std::string name) {
                this->name = name;
                this->create_time = get_time();
                this->update_time = get_time();
                this->content = "";
            };

            void update_update_time() {
                this->update_time = get_time();
            }
    };

    std::map<int, std::pair<int, Node>> mp;

    bool node_exist(int id) {
        return mp.count(id);
    }

    int get_new_id() {
        int id = rand() * rand();
        id = id < 0 ? -id : id;
        while (node_exist(id)) {
            id = rand() * rand();
            id = id < 0 ? -id : id;
        }
        return id;
    }

public:
    int get_new_node(std::string name = "") {
        int new_id = get_new_id();
        mp[new_id] = std::make_pair(1, Node(name));
        return new_id;
    };

    void delete_node(int idx) {
        if (!node_exist(idx)) return;
        if (--mp[idx].first == 0) {
            mp.erase(mp.find(idx));
        }
    }

    int update_content(int idx, std::string content) {
        if (!node_exist(idx)) return -1;
        std::string name = get_name(idx);
        std::string create_time = get_update_time(idx);
        delete_node(idx);
        idx = get_new_node(name);
        mp[idx].second.content = content;
        mp[idx].second.create_time = create_time;
        return idx;
    }

    int update_name(int idx, std::string name) {
        if (!node_exist(idx)) return -1;
        std::string content = get_content(idx);
        std::string create_time = get_update_time(idx);
        delete_node(idx);
        idx = get_new_node(name);
        mp[idx].second.content = content;
        mp[idx].second.create_time = create_time;
        return idx;
    }

    std::string get_content(int idx) {
        if (!node_exist(idx)) return "";
        return mp[idx].second.content;
    }

    std::string get_name(int idx) {
        if (!node_exist(idx)) return "";
        return mp[idx].second.name;
    }

    std::string get_update_time(int idx) {
        if (!node_exist(idx)) return "";
        return mp[idx].second.update_time;
    }

    std::string get_create_time(int idx) {
        if (!node_exist(idx)) return "";
        return mp[idx].second.create_time;
    }

    void increase_counter(int idx) {
        if (!node_exist(idx)) return;
        mp[idx].first ++;
    }

    int _get_counter(int idx) {
        if (!node_exist(idx)) return -1;
        return mp[idx].first;
    }
};

void test() {
    NodeManager nm;
    std::vector<int> id;
    for (int i = 0; i < 5; i++) {
        id.push_back(nm.get_new_node(std::to_string(i)));
        // std::cout << nm._get_counter(id[i]) << ' ';
    }
    for (int i = id.size() - 1; i >= 0; i--) {
        id[i] = nm.update_content(id[i], std::to_string(i + 1));
        int t = rand() % 4 + 1;
        for (int j = 0; j < t; j++) {
            nm.increase_counter(id[i]);
        }
        int tt = rand() % t + 2;
        for (int j = 0; j < tt; j++) {
            nm.delete_node(id[i]);
        }
        std::cout << nm._get_counter(id[i]) << ' ' << t - tt << '\n';
    }
    // for (int i = 0; i < id.size(); i++) {
        // std::cout << nm.get_name(id[i]) << ' ' << nm.get_content(id[i]) << ' ' << nm.get_create_time(id[i]) << ' ' << nm.get_update_time(id[i]) << '\n';
    // }
}

int main() {
    srand(time(0));
    test();
    return 0;
};