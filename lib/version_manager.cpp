/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

Author: Mu Xiangyu, Chant Mee 
*/

#ifndef VERSION_MANAGER_CPP
#define VERSION_MANAGER_CPP

#include "bs_tree.cpp"
#include "node_manager.cpp"
#include "logger.cpp"
#include <string>

#define NO_MODEL_VERSION 0x3f3f3f3f

struct versionNode {
    std::string info;
    treeNode *p;

    versionNode() = default;
    versionNode(std::string info, treeNode *p) : info(info), p(p) {}
};

class VersionManager {
private:
    std::map<unsigned long long, versionNode> version;
    NodeManager &node_manager = NodeManager::get_node_manager();
    Logger logger = Logger::get_logger();
public:
    bool recursive_increase_counter(treeNode *p, bool modify_brother=false);
    bool init_version(treeNode *p, treeNode *vp);
    bool create_version(unsigned long long model_version=NO_MODEL_VERSION, std::string info="");
    bool version_exist(unsigned long long id);
    bool get_version_pointer(unsigned long long id, treeNode *&p);
    bool get_latest_version(unsigned long long &id);
    bool get_version_log(std::vector<std::pair<unsigned long long, versionNode>> &version_log);
};

bool VersionManager::recursive_increase_counter(treeNode *p, bool modify_brother) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    recursive_increase_counter(p->first_son, true);
    if (modify_brother) recursive_increase_counter(p->next_brother, true);
    p->cnt ++;
    node_manager.increase_counter(p->link);
    logger.log("The counter for node " + node_manager.get_name(p->link) + " has been incremented by one.");
    return true;
}

bool VersionManager::init_version(treeNode *p, treeNode *vp) {
    if (p == nullptr || vp == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__), Logger::FATAL, __LINE__);
        return false;
    }
    p->first_son = vp->first_son;
    if (!recursive_increase_counter(p, 1)) return false;
    return true;
}

bool VersionManager::create_version(unsigned long long model_version, std::string version_info) {
    if (model_version != NO_MODEL_VERSION && !version_exist(model_version)) {
        logger.log("The version number does not exist in the system.", Logger::WARNING, __LINE__);
        return false;
    }
    treeNode *new_version = new treeNode(treeNode::DIR);
    new_version->cnt = 0;
    if (new_version == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    new_version->cnt = 0;
    new_version->link = node_manager.get_new_node("root");
    if (model_version != NO_MODEL_VERSION) delete new_version->first_son;
    treeNode *model = model_version == NO_MODEL_VERSION ? new_version : version[model_version].p;
    if (!init_version(new_version, model)) return false;
    unsigned long long id = version.empty() ? 1001 : (*version.rbegin()).first + 1;
    version[id] = versionNode(version_info, new_version);
    return true;
}

bool VersionManager::version_exist(unsigned long long id) {
    return version.count(id);
}

bool VersionManager::get_version_pointer(unsigned long long id, treeNode *&p) {
    if (!version_exist(id)) {
        logger.log("Version " + std::to_string(id) + " does not exist.", Logger::WARNING, __LINE__);
        return false;
    }
    p = version[id].p;
    return true;
}

bool VersionManager::get_latest_version(unsigned long long &id) {
    if (version.empty()) {
        logger.log("No version exists in the system. Please create a new version to use.", Logger::WARNING, __LINE__);
        return false;
    }
    id = version.rbegin()->first;
    return true;
}

bool VersionManager::get_version_log(std::vector<std::pair<unsigned long long, versionNode>> &version_log) {
    for (auto &it : version) {
        version_log.push_back(it);
    }
    return true;
}

#endif