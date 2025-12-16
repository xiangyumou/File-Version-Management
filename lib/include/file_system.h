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
#include "interfaces/i_logger.h"
#include "interfaces/i_node_manager.h"
#include "interfaces/i_storage.h"
#include <string>
#include <vector>
#include <stack>
#include <memory>

// Forward declarations
class NodeManager;
class Logger;

/**
 * @brief FileSystem class - Core file system operations
 * 
 * Uses composition with BSTree instead of inheritance for better testability.
 */
class FileSystem {
private:
    std::unique_ptr<BSTree> tree_;  ///< Tree structure (composition)
    VersionManager version_manager_;
    int CURRENT_VERSION;
    
    // Dependencies
    ffvms::ILogger* logger_ = nullptr;
    ffvms::INodeManager* node_manager_ = nullptr;
    
    // Helper to get dependencies
    ffvms::ILogger& get_logger_ref();
    ffvms::INodeManager& get_node_manager_ref();

    // Internal helper methods
    bool decrease_counter(treeNode* p);
    bool recursive_delete_nodes(treeNode* p, bool delete_brother = false);
    bool delete_node();
    bool rebuild_nodes(treeNode* p);
    bool travel_tree(treeNode* p, std::string& tree_info, int tab_cnt = 1);
    bool travel_find(const std::string& name, 
                     std::vector<std::pair<std::string, std::vector<std::string>>>& res);
    bool kmp(const std::string& str, const std::string& tar);

public:
    /// Default constructor
    FileSystem();
    
    /// Constructor with dependency injection
    FileSystem(ffvms::ILogger* logger, ffvms::INodeManager* node_manager, ffvms::IStorage* storage = nullptr);
    
    virtual ~FileSystem() = default;

    // File system operations
    bool switch_version(int version_id);
    bool make_file(const std::string& name);
    bool make_dir(const std::string& name);
    bool change_directory(const std::string& name);
    bool remove_file(const std::string& name);
    bool remove_dir(const std::string& name);
    bool update_name(const std::string& fr_name, const std::string& to_name);
    bool update_content(const std::string& name, const std::string& content);
    bool get_content(const std::string& name, std::string& content);
    bool tree(std::string& tree_info);
    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string>& content);
    bool create_version(unsigned long long model_version = NO_MODEL_VERSION, 
                        const std::string& info = "");
    bool create_version(const std::string& info, 
                        unsigned long long model_version = NO_MODEL_VERSION);
    bool version(std::vector<std::pair<unsigned long long, versionNode>>& version_log);
    bool get_update_time(const std::string& name, std::string& update_time);
    bool get_create_time(const std::string& name, std::string& create_time);
    bool get_type(const std::string& name, treeNode::TYPE& type);
    bool get_current_path(std::vector<std::string>& p);
    bool Find(const std::string& name, 
              std::vector<std::pair<std::string, std::vector<std::string>>>& res);
    int get_current_version();
    bool navigate_to_path(const std::vector<std::string>& path);
};

#endif // FILE_SYSTEM_H
