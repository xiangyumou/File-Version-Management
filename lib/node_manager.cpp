#ifndef NODE_MANAGER_CPP
#define NODE_MANAGER_CPP

#include <cmath>
#include <cstring>
#include <string>
#include <map>

class NodeManager {
private:
    class Node {
        public:
            std::string name;
            std::string create_time;
            std::string update_time;
            std::string content;

            std::string get_time();
            Node();
            Node(std::string name);
            void update_update_time();
    };

    std::map<int, std::pair<int, Node>> mp;

    bool node_exist(int id);
    int get_new_id();
public:
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
    mp[new_id] = std::make_pair(1, Node(name));
    return new_id;
};

void NodeManager::delete_node(int idx) {
    if (!node_exist(idx)) return;
    if (--mp[idx].first == 0) {
        mp.erase(mp.find(idx));
    }
}

int NodeManager::update_content(int idx, std::string content) {
    if (!node_exist(idx)) return -1;
    std::string name = get_name(idx);
    std::string create_time = get_update_time(idx);
    delete_node(idx);
    idx = get_new_node(name);
    mp[idx].second.content = content;
    mp[idx].second.create_time = create_time;
    return idx;
}

int NodeManager::update_name(int idx, std::string name) {
    if (!node_exist(idx)) return -1;
    std::string content = get_content(idx);
    std::string create_time = get_update_time(idx);
    delete_node(idx);
    idx = get_new_node(name);
    mp[idx].second.content = content;
    mp[idx].second.create_time = create_time;
    return idx;
}

std::string NodeManager::get_content(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.content;
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

#endif