/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

Author: Mu Xiangyu, Chant Mee 
*/

#ifndef NODE_MANAGER_CPP
#define NODE_MANAGER_CPP

#include "file_manager.cpp"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>

class NodeManager {
private:
    class Node {
        private:
            FileManager &file_manager = FileManager::get_file_manager();
        public:
            std::string name;
            std::string create_time;
            std::string update_time;
            unsigned long long fid;

            std::string get_time();
            Node();
            Node(std::string name);
            void update_update_time();
    };

    std::map<int, std::pair<int, Node>> mp;
    FileManager &file_manager = FileManager::get_file_manager();
    Logger &logger = Logger::get_logger();

    int get_new_id();
public:
    // std::map<int, std::pair<int, Node>> &MPP = mp;

    bool node_exist(int id);
    int get_new_node(std::string name);
    void delete_node(int idx);
    int update_content(int idx, std::string content);
    int update_name(int idx, std::string name);
    std::string get_content(int idx);
    std::string get_name(int idx);
    std::string get_update_time(int idx);
    std::string get_create_time(int idx);
    void increase_counter(int idx);
    int _get_counter(int idx);
    static NodeManager& get_node_manager();
};




                        /* ======= class Node ======= */

std::string NodeManager::Node::get_time() {
    static char t[100];
    time_t timep;
    time(&timep);
    struct tm* p = gmtime(&timep);
    sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    return std::string(t);
}

NodeManager::Node::Node() = default;
NodeManager::Node::Node(std::string name) {
    this->name = name;
    this->create_time = get_time();
    this->update_time = get_time();
    this->fid = file_manager.create_file("");
}

void NodeManager::Node::update_update_time() {
    this->update_time = get_time();
}


                        /* ======= class NodeManager ======= */

bool NodeManager::node_exist(int id) {
    return mp.count(id);
}

int NodeManager::get_new_id() {
    int id = rand() * rand();
    id = id < 0 ? -id : id;
    while (node_exist(id)) {
        id = rand() * rand();
        id = id < 0 ? -id : id;
    }
    return id;
}

int NodeManager::get_new_node(std::string name) {
    int new_id = get_new_id();
    // std::pair<int, NodeManager::Node> q = std::make_pair(0, Node(name));
    auto t = std::make_pair(1, Node(name));
    // mp[new_id] = std::make_pair(1, Node(name));
    mp.insert(std::make_pair(new_id, t));
    return new_id;
};

void NodeManager::delete_node(int idx) {
    if (!node_exist(idx)) return;
    if (--mp[idx].first == 0) {
        file_manager.decrease_counter(mp[idx].second.fid);
        mp.erase(mp.find(idx));
    }
}

int NodeManager::update_content(int idx, std::string content) {
    if (!node_exist(idx)) return -1;
    std::string name = get_name(idx);
    std::string create_time = get_update_time(idx);
    delete_node(idx);
    idx = get_new_node(name);

    unsigned long long fid = mp[idx].second.fid;
    file_manager.update_content(mp[idx].second.fid, mp[idx].second.fid, content);
    return idx;
}

int NodeManager::update_name(int idx, std::string name) {
    if (!node_exist(idx)) return -1;
    std::string create_time = get_update_time(idx);
    unsigned long long fid = mp[idx].second.fid;
    unsigned long long old_idx = idx;
    file_manager.increase_counter(fid);
    idx = get_new_node(name);
    mp[idx].second.create_time = create_time;
    file_manager.decrease_counter(mp[idx].second.fid);
    mp[idx].second.fid = fid;
    delete_node(old_idx);
    return idx;
}

std::string NodeManager::get_content(int idx) {
    if (!node_exist(idx)) return "-1";
    std::string content;
    file_manager.get_content(mp[idx].second.fid, content);
    return content;
}

std::string NodeManager::get_name(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.name;
}

std::string NodeManager::get_update_time(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.update_time;
}

std::string NodeManager::get_create_time(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.create_time;
}

void NodeManager::increase_counter(int idx) {
    if (!node_exist(idx)) return;
    mp[idx].first ++;
}

int NodeManager::_get_counter(int idx) {
    if (!node_exist(idx)) return -1;
    return mp[idx].first;
}

NodeManager& NodeManager::get_node_manager() {
    static NodeManager node_manager;
    return node_manager;
}

int test_node_manager() {
// int main() {
    unsigned long long fid;
    NodeManager nm;
    FileManager fm = FileManager::get_file_manager();
    Logger &logger = Logger::get_logger();

    typedef unsigned long long ull;
    ull id = nm.get_new_node("123");
    id = nm.update_content(id, "diyici");
    nm.increase_counter(id);
    ull id1 = nm.update_content(id, "diorci");
    nm.increase_counter(id1);
    ull id2 = nm.update_name(id1, "321");
    /**
     * 三个文件:    123、123、321
     * 节点计数:    1 .  1 .  1
     * 文件计数：   1 .  2 .  2
     */
    // for (auto it : nm.MPP) {
    //     std::cout << "cnt: " << it.second.first << ' ' << "name: " << it.second.second.name << ' ' << "fid: " << it.second.second.fid << '\n';
    // }
    // for (auto it : fm.MP) {
    //     std::cout << "fid: " << it.first << ' ' << "cnt: " << it.second.cnt << '\n';
    // }
    return 0;
}

#endif