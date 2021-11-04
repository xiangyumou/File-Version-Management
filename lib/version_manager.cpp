#ifndef VERSION_MANAGER_CPP
#define VERSION_MANAGER_CPP

#include "bs_tree.cpp"
#include "node_manager.cpp"
#include "logger.cpp"

class VersionManager {
private:
    NodeManager node_manager = NodeManager::get_node_manager();
    Logger logger = Logger::get_logger();
public:
    std::vector<treeNode*> version;

    bool recursive_increase_counter(treeNode *p, bool modify_brother=false);
    bool init_version(treeNode *p, treeNode *vp);
    bool create_version(int model_version=-1);
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

bool VersionManager::create_version(int model_version) {
    version.push_back(new treeNode(treeNode::DIR));
    if (version.back() == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    version.back()->cnt = 0;
    version.back()->link = node_manager.get_new_node("root");
    if (model_version != -1) delete version.back()->first_son;
    treeNode *model = model_version == -1 ? version.back() : version[model_version];
    if (!init_version(version.back(), model)) return false;
    return true;
}

#endif