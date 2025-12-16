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

#include "interfaces/i_node_manager.h"
#include "interfaces/i_logger.h"
#include "core/types.h"
#include <vector>
#include <string>

/**
 * @brief Tree node structure for file system
 * 
 * Uses manual memory management with reference counting (cnt field).
 * Nodes can be shared across versions - when cnt reaches 0, node can be deleted.
 */
struct treeNode {
    enum TYPE {
        FILE = 0, DIR, HEAD_NODE
    };

    TYPE type;
    int cnt;  ///< Reference count for version sharing
    unsigned long long link;  ///< Link to NodeManager for metadata
    treeNode* next_brother;
    treeNode* first_son;

    treeNode();
    explicit treeNode(TYPE type);
};

// Forward declarations
class NodeManager;
class Logger;

/**
 * @brief Binary Search Tree for file system operations
 * 
 * Provides tree navigation and management for the virtual file system.
 * All methods are now public to support composition pattern.
 */
class BSTree {
private:
    // Dependencies (can be injected or use singletons)
    ffvms::ILogger* logger_ = nullptr;
    ffvms::INodeManager* node_manager_ = nullptr;
    
    // Helper to get dependencies
    ffvms::ILogger& get_logger_ref();
    ffvms::INodeManager& get_node_manager_ref();

public:
    /// Current path in the tree (made public for composition)
    std::vector<treeNode*> path;

    /// Default constructor (uses global singletons)
    BSTree() = default;
    
    /// Constructor with dependency injection
    BSTree(ffvms::ILogger* logger, ffvms::INodeManager* node_manager);
    
    virtual ~BSTree() = default;

    // Tree navigation and management methods (now public for composition)
    bool check_path();
    bool check_node(treeNode* p, int line);
    bool is_son();
    bool goto_tail();
    bool goto_head();
    bool name_exist(const std::string& name);
    bool go_to(const std::string& name);
    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string>& content);
    bool get_current_path(std::vector<std::string>& p);
};

#endif // BS_TREE_H
