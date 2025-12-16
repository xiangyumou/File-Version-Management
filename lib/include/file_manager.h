/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "logger.h"
#include "saver.h"
#include <string>
#include <map>

struct fileNode {
    std::string content;
    unsigned long long cnt;

    fileNode() = default;
    fileNode(std::string content);
};

class FileManager {
private:
    std::string DATA_STORAGE_NAME = "FileManager::map_relation";
    std::map<unsigned long long, fileNode> mp;
    Saver& saver = Saver::get_saver();
    Logger& logger = Logger::get_logger();

    unsigned long long get_new_id();
    bool file_exist(unsigned long long fid);
    bool check_file(unsigned long long fid);
    bool save();
    bool load();

public:
    FileManager();
    ~FileManager();
    static FileManager& get_file_manager();
    unsigned long long create_file(std::string content = "");
    bool increase_counter(unsigned long long fid);
    bool decrease_counter(unsigned long long fid);
    bool update_content(unsigned long long fid, unsigned long long& new_id, std::string content);
    bool get_content(unsigned long long fid, std::string& content);
};

#endif // FILE_MANAGER_H
