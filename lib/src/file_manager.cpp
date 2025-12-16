/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "file_manager.h"
#include "saver.h"
#include "logger.h"
#include <random>

// fileNode implementation
fileNode::fileNode(std::string content) : content(std::move(content)), cnt(1) {}

// Helper to get storage reference
ffvms::IStorage& FileManager::get_storage_ref() {
    if (storage_) return *storage_;
    return Saver::get_saver();
}

// Helper to get logger reference
ffvms::ILogger& FileManager::get_logger_ref() {
    if (logger_) return *logger_;
    return Logger::get_logger();
}

// FileManager implementation
unsigned long long FileManager::get_new_id() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<unsigned long long> dis;
    
    unsigned long long id;
    do {
        id = dis(gen);
    } while (mp.count(id));
    return id;
}

bool FileManager::file_exist(unsigned long long fid) {
    if (!mp.count(fid)) {
        get_logger_ref().log("File id " + std::to_string(fid) + " does not exists. This is not normal.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    return true;
}

bool FileManager::check_file(unsigned long long fid) {
    if (!file_exist(fid)) return false;
    if (mp[fid].cnt <= 0) {
        get_logger_ref().log("File ID " + std::to_string(fid) + " counter is <= 0, abnormal state.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    return true;
}

bool FileManager::save() {
    ffvms::DataTable data;
    for (auto& it : mp) {
        data.push_back(std::vector<std::string>());
        data.back().push_back(std::to_string(it.first));
        data.back().push_back(it.second.content);
        data.back().push_back(std::to_string(it.second.cnt));
    }
    if (!get_storage_ref().save(DATA_STORAGE_NAME, data)) return false;
    return true;
}

bool FileManager::load() {
    ffvms::DataTable data;
    if (!get_storage_ref().load(DATA_STORAGE_NAME, data)) return false;
    mp.clear();
    for (auto& it : data) {
        if (it.size() != 3) {
            get_logger_ref().log("FileSystem: File is corrupted and cannot be read.", 
                                 ffvms::LogLevel::WARNING, __LINE__);
            mp.clear();
            return false;
        }
        if (!ffvms::IStorage::is_all_digits(it[0])) {
            get_logger_ref().log("FileSystem: File is corrupted and cannot be read.", 
                                 ffvms::LogLevel::WARNING, __LINE__);
            mp.clear();
            return false;
        }
        unsigned long long key = ffvms::IStorage::str_to_ull(it[0]);
        unsigned long long cnt = ffvms::IStorage::str_to_ull(it[2]);
        std::string& content = it[1];
        auto t = std::make_pair(key, fileNode(content));
        t.second.cnt = cnt;
        mp.insert(t);
    }
    return true;
}

FileManager::FileManager() : storage_(nullptr), logger_(nullptr) {
    if (!load()) return;
}

FileManager::FileManager(ffvms::IStorage* storage, ffvms::ILogger* logger) 
    : storage_(storage), logger_(logger) {
    if (!load()) return;
}

FileManager::~FileManager() {
    save();
}

FileManager& FileManager::get_file_manager() {
    static FileManager file_manager;
    return file_manager;
}

unsigned long long FileManager::create_file(const std::string& content) {
    unsigned long long id = get_new_id();
    mp[id] = fileNode(content);
    return id;
}

bool FileManager::increase_counter(unsigned long long fid) {
    if (!mp.count(fid)) {
        get_logger_ref().log("File id does not exists.", ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    if (!check_file(fid)) return false;
    mp[fid].cnt++;
    return true;
}

bool FileManager::decrease_counter(unsigned long long fid) {
    if (!mp.count(fid)) {
        get_logger_ref().log("File id does not exists.", ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    if (!check_file(fid)) return false;
    if (--mp[fid].cnt <= 0) {
        mp.erase(mp.find(fid));
    }
    return true;
}

bool FileManager::update_content(unsigned long long fid, unsigned long long& new_id, 
                                 const std::string& content) {
    if (!file_exist(fid)) return false;
    if (!decrease_counter(fid)) return false;
    new_id = get_new_id();
    mp[new_id].content = content;
    mp[new_id].cnt = 1;
    return true;
}

bool FileManager::get_content(unsigned long long fid, std::string& content) {
    if (!file_exist(fid)) return false;
    content = mp[fid].content;
    return true;
}
