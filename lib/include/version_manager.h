/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include "bs_tree.h"
#include "bs_tree.h"
#include "interfaces/i_node_manager.h"
#include "interfaces/i_logger.h"
#include "interfaces/i_storage.h"
#include "saver.h" // Needed for default constructor in cpp, or forward declare? Keep it for now.
#include <string>
#include <vector>
#include <map>

// Constants
constexpr unsigned long long NO_MODEL_VERSION = 0x3f3f3f3f;

struct versionNode {
    std::string info;
    treeNode* p;

    versionNode() = default;
    versionNode(std::string info, treeNode* p) : info(info), p(p) {}
};

class VersionManager {
private:
    std::map<unsigned long long, versionNode> version;
    ffvms::INodeManager* node_manager_ = nullptr;
    ffvms::ILogger* logger_ = nullptr;
    ffvms::IStorage* storage_ = nullptr;
    
    // Helpers
    ffvms::ILogger& get_logger_ref();
    ffvms::INodeManager& get_node_manager_ref();
    ffvms::IStorage& get_storage_ref();
    static constexpr unsigned long long NULL_NODE = 0x3f3f3f3f3f3fULL;
    std::string DATA_TREENODE_INFO = "VersionManager::DATA_TREENODE_INFO";
    std::string DATA_VERSION_INFO = "VersionManager::DATA_VERSION_INFO";

    bool load();
    bool save();
    void dfs(treeNode* cur, std::map<treeNode*, unsigned long long>& label);
    bool recursive_increase_counter(treeNode* p, bool modify_brother = false);

public:
    VersionManager();
    VersionManager(ffvms::ILogger* logger, ffvms::INodeManager* node_manager, ffvms::IStorage* storage);
    ~VersionManager();
    bool init_version(treeNode* p, treeNode* vp);
    bool create_version(unsigned long long model_version = NO_MODEL_VERSION, std::string info = "");
    bool version_exist(unsigned long long id);
    bool get_version_pointer(unsigned long long id, treeNode*& p);
    bool get_latest_version(unsigned long long& id);
    bool get_version_log(std::vector<std::pair<unsigned long long, versionNode>>& version_log);
    bool empty();
};

#endif // VERSION_MANAGER_H
