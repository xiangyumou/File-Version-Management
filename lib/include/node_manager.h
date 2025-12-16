/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "file_manager.h"
#include "saver.h"
#include "logger.h"
#include <string>
#include <map>

/**
 * @brief Node class representing file/folder metadata
 */
class Node {
private:
    FileManager& file_manager = FileManager::get_file_manager();
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

/**
 * @brief NodeManager class for managing file/folder nodes
 */
class NodeManager {
private:
    std::map<unsigned long long, std::pair<unsigned long long, Node>> mp;
    FileManager& file_manager = FileManager::get_file_manager();
    std::string DATA_STORAGE_NAME = "NodeManager::map_relation";
    Saver& saver = Saver::get_saver();
    Logger& logger = Logger::get_logger();

    unsigned long long get_new_id();
    bool load();
    bool save();

public:
    NodeManager();
    ~NodeManager();
    bool node_exist(unsigned long long id);
    unsigned long long get_new_node(std::string name);
    void delete_node(unsigned long long idx);
    unsigned long long update_content(unsigned long long idx, std::string content);
    unsigned long long update_name(unsigned long long idx, std::string name);
    std::string get_content(unsigned long long idx);
    std::string get_name(unsigned long long idx);
    std::string get_update_time(unsigned long long idx);
    std::string get_create_time(unsigned long long idx);
    void increase_counter(unsigned long long idx);
    unsigned long long _get_counter(unsigned long long idx);
    static NodeManager& get_node_manager();
};

#endif // NODE_MANAGER_H
