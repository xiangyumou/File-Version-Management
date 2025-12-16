/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef BS_TREE_H
#define BS_TREE_H

#include "node_manager.h"
#include "logger.h"
#include <vector>
#include <string>

/**
 * @brief Tree node structure for file system
 */
struct treeNode {
    enum TYPE {
        FILE = 0, DIR, HEAD_NODE
    };

    TYPE type;
    int cnt;
    unsigned long long link;
    treeNode* next_brother;
    treeNode* first_son;

    treeNode();
    treeNode(TYPE type);
};

/**
 * @brief Binary Search Tree for file system operations
 */
class BSTree {
private:
    Logger& logger = Logger::get_logger();
    NodeManager& node_manager = NodeManager::get_node_manager();

protected:
    std::vector<treeNode*> path;

    bool check_path();
    bool check_node(treeNode* p, int line);
    bool is_son();
    bool goto_tail();
    bool goto_head();
    bool name_exist(std::string name);
    bool go_to(std::string name);
    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string>& content);
    bool get_current_path(std::vector<std::string>& path);
};

#endif // BS_TREE_H
