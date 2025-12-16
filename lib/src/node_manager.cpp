/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "node_manager.h"
#include "file_manager.h"
#include "saver.h"
#include "logger.h"
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>

// Node implementation
ffvms::IFileManager& Node::get_file_manager_ref() {
    if (file_manager_) return *file_manager_;
    return FileManager::get_file_manager();
}

std::string Node::get_time() {
    auto now = std::time(nullptr);
    auto* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Node::Node() = default;

Node::Node(const std::string& name) : file_manager_(nullptr) {
    this->name = name;
    this->create_time = get_time();
    this->update_time = get_time();
    this->fid = get_file_manager_ref().create_file("");
}

Node::Node(const std::string& name, ffvms::IFileManager* file_manager) 
    : file_manager_(file_manager) {
    this->name = name;
    this->create_time = get_time();
    this->update_time = get_time();
    this->fid = get_file_manager_ref().create_file("");
}

void Node::update_update_time() {
    this->update_time = get_time();
}

// NodeManager helper methods
ffvms::IFileManager& NodeManager::get_file_manager_ref() {
    if (file_manager_) return *file_manager_;
    return FileManager::get_file_manager();
}

ffvms::IStorage& NodeManager::get_storage_ref() {
    if (storage_) return *storage_;
    return Saver::get_saver();
}

ffvms::ILogger& NodeManager::get_logger_ref() {
    if (logger_) return *logger_;
    return Logger::get_logger();
}

// NodeManager implementation
bool NodeManager::node_exist(unsigned long long id) {
    return mp.count(id);
}

unsigned long long NodeManager::get_new_id() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<unsigned long long> dis;
    
    unsigned long long id;
    do {
        id = dis(gen);
    } while (node_exist(id));
    return id;
}

bool NodeManager::save() {
    ffvms::DataTable data;
    for (auto& it : mp) {
        data.push_back(std::vector<std::string>());
        data.back().push_back(std::to_string(it.first));
        data.back().push_back(std::to_string(it.second.first));
        data.back().push_back(it.second.second.name);
        data.back().push_back(it.second.second.create_time);
        data.back().push_back(it.second.second.update_time);
        data.back().push_back(std::to_string(it.second.second.fid));
    }
    if (!get_storage_ref().save(DATA_STORAGE_NAME, data)) return false;
    return true;
}

bool NodeManager::load() {
    ffvms::DataTable data;
    if (!get_storage_ref().load(DATA_STORAGE_NAME, data)) return false;
    mp.clear();
    for (auto& it : data) {
        if (it.size() != 6) {
            get_logger_ref().log("NodeManager: File is corrupted and cannot be read.", 
                                 ffvms::LogLevel::WARNING, __LINE__);
            mp.clear();
            return false;
        }
        bool flag = true;
        if (!ffvms::IStorage::is_all_digits(it[0])) flag = false;
        if (!ffvms::IStorage::is_all_digits(it[1])) flag = false;
        if (!ffvms::IStorage::is_all_digits(it[5])) flag = false;
        if (!flag) {
            mp.clear();
            get_logger_ref().log("NodeManager: File is corrupted and cannot be read.", 
                                 ffvms::LogLevel::WARNING, __LINE__);
            return false;
        }
        unsigned long long key = ffvms::IStorage::str_to_ull(it[0]);
        unsigned long long cnt = ffvms::IStorage::str_to_ull(it[1]);
        unsigned long long fid = ffvms::IStorage::str_to_ull(it[5]);
        std::string& name = it[2];
        std::string& create_time = it[3];
        std::string& update_time = it[4];
        Node t_node = Node();
        t_node.name = name;
        t_node.create_time = create_time;
        t_node.update_time = update_time;
        t_node.fid = fid;
        auto t_pair = std::make_pair(cnt, t_node);
        mp.insert(std::make_pair(key, t_pair));
    }
    return true;
}

NodeManager::NodeManager() 
    : file_manager_(nullptr), storage_(nullptr), logger_(nullptr) {
    if (!load()) return;
}

NodeManager::NodeManager(ffvms::IFileManager* file_manager, ffvms::IStorage* storage, 
                         ffvms::ILogger* logger)
    : file_manager_(file_manager), storage_(storage), logger_(logger) {
    if (!load()) return;
}

NodeManager::~NodeManager() {
    if (!save()) return;
}

unsigned long long NodeManager::get_new_node(const std::string& name) {
    unsigned long long new_id = get_new_id();
    auto t = std::make_pair(1ULL, Node(name, file_manager_));
    mp.insert(std::make_pair(new_id, t));
    return new_id;
}

void NodeManager::delete_node(unsigned long long idx) {
    if (!node_exist(idx)) return;
    if (--mp[idx].first == 0) {
        get_file_manager_ref().decrease_counter(mp[idx].second.fid);
        mp.erase(mp.find(idx));
    }
}

unsigned long long NodeManager::update_content(unsigned long long idx, const std::string& content) {
    if (!node_exist(idx)) return static_cast<unsigned long long>(-1);
    std::string name = get_name(idx);
    std::string create_time = get_create_time(idx);
    delete_node(idx);
    idx = get_new_node(name);
    mp[idx].second.create_time = create_time;

    unsigned long long fid = mp[idx].second.fid;
    get_file_manager_ref().update_content(mp[idx].second.fid, fid, content);
    return idx;
}

unsigned long long NodeManager::update_name(unsigned long long idx, const std::string& name) {
    if (!node_exist(idx)) return static_cast<unsigned long long>(-1);
    std::string create_time = get_create_time(idx);
    unsigned long long fid = mp[idx].second.fid;
    unsigned long long old_idx = idx;
    get_file_manager_ref().increase_counter(fid);
    idx = get_new_node(name);
    mp[idx].second.create_time = create_time;
    get_file_manager_ref().decrease_counter(mp[idx].second.fid);
    mp[idx].second.fid = fid;
    delete_node(old_idx);
    return idx;
}

std::string NodeManager::get_content(unsigned long long idx) {
    if (!node_exist(idx)) return "-1";
    std::string content;
    get_file_manager_ref().get_content(mp[idx].second.fid, content);
    return content;
}

std::string NodeManager::get_name(unsigned long long idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.name;
}

std::string NodeManager::get_update_time(unsigned long long idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.update_time;
}

std::string NodeManager::get_create_time(unsigned long long idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.create_time;
}

void NodeManager::increase_counter(unsigned long long idx) {
    if (!node_exist(idx)) return;
    mp[idx].first++;
}

unsigned long long NodeManager::_get_counter(unsigned long long idx) {
    if (!node_exist(idx)) return static_cast<unsigned long long>(-1);
    return mp[idx].first;
}

NodeManager& NodeManager::get_node_manager() {
    static NodeManager node_manager;
    return node_manager;
}
