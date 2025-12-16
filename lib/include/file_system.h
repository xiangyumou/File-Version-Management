/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "version_manager.h"
#include "bs_tree.h"
#include "node_manager.h"
#include "saver.h"
#include "logger.h"
#include <string>
#include <vector>
#include <stack>

/**
 * @brief FileSystem class - Core file system operations
 */
class FileSystem : private BSTree {
private:
    VersionManager version_manager;
    Logger& logger = Logger::get_logger();
    NodeManager& node_manager = NodeManager::get_node_manager();
    int CURRENT_VERSION;

    bool decrease_counter(treeNode* p);
    bool recursive_delete_nodes(treeNode* p, bool delete_brother = false);
    bool delete_node();
    bool rebuild_nodes(treeNode* p);
    bool travel_tree(treeNode* p, std::string& tree_info, int tab_cnt = 1);
    bool travel_find(std::string name, std::vector<std::pair<std::string, std::vector<std::string>>>& res);
    bool kmp(std::string str, std::string tar);

public:
    FileSystem();

    bool switch_version(int version_id);
    bool make_file(std::string name);
    bool make_dir(std::string name);
    bool change_directory(std::string name);
    bool remove_file(std::string name);
    bool remove_dir(std::string name);
    bool update_name(std::string fr_name, std::string to_name);
    bool update_content(std::string name, std::string content);
    bool get_content(std::string name, std::string& content);
    bool tree(std::string& tree_info);
    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string>& content);
    bool create_version(unsigned long long model_version = NO_MODEL_VERSION, std::string info = "");
    bool create_version(std::string info, unsigned long long model_version = NO_MODEL_VERSION);
    bool version(std::vector<std::pair<unsigned long long, versionNode>>& version_log);
    bool get_update_time(std::string name, std::string& update_time);
    bool get_create_time(std::string name, std::string& create_time);
    bool get_type(std::string name, treeNode::TYPE& type);
    bool get_current_path(std::vector<std::string>& p);
    bool Find(std::string name, std::vector<std::pair<std::string, std::vector<std::string>>>& res);
    int get_current_version();
};

#endif // FILE_SYSTEM_H
