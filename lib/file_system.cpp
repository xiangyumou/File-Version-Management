#ifndef FILE_SYSTEM_CPP
#define FILE_SYSTEM_CPP

#include "version_manager.cpp"
#include "bs_tree.cpp"
#include "node_manager.cpp"
#include "saver.cpp"
#include "encryptor.cpp"
#include "logger.cpp"
#include <ctime>
#include <string>
#include <stack>

class FileSystem : private BSTree {
private:

    VersionManager version_manager;
    Logger &logger = Logger::get_logger();
    NodeManager &node_manager = NodeManager::get_node_manager();
    int CURRENT_VERSION;

    bool decrease_counter(treeNode *p);
    bool recursive_delete_nodes(treeNode *p, bool delete_brother=false);
    bool delete_node();
    bool rebuild_nodes(treeNode *p);

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
    bool get_content(std::string name, std::string &content);
    bool tree(std::string &tree_info);
    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string> &content);
    bool create_version(unsigned long long model_version=NO_MODEL_VERSION, std::string info="");
    bool create_version(std::string info = "", unsigned long long model_version=NO_MODEL_VERSION);
    bool version(std::vector<std::pair<unsigned long long, versionNode>> &version_log);
    int get_current_version();
};






                        /* ======= class Node ======= */

std::string NodeManager::Node::get_time() {
    static char t[100];
    time_t timep;
    time(&timep);
    struct tm* p = gmtime(&timep);
    sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    return std::string(t);
}

NodeManager::Node::Node() = default;
NodeManager::Node::Node(std::string name) {
    this->name = name;
    this->create_time = get_time();
    this->update_time = get_time();
    this->content = "";
};

void NodeManager::Node::update_update_time() {
    this->update_time = get_time();
}







                        /* ======= class FileSystem ======= */

FileSystem::FileSystem() {
    version_manager.create_version();
    unsigned long long latest_version_id;
    if (!version_manager.get_latest_version(latest_version_id)) return;
    switch_version(latest_version_id);
}

bool FileSystem::decrease_counter(treeNode *p) {
    if (!check_node(p, __LINE__)) return false;
    if (--p->cnt == 0) {
        logger.log("Node " + node_manager.get_name(p->link) + " will be deleted...");
        node_manager.delete_node(p->link);
        delete p;
        logger.log("Deleting completed.");
    }
    return true;
}

bool FileSystem::recursive_delete_nodes(treeNode *p, bool delete_brother) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    recursive_delete_nodes(p->first_son, true);
    if (delete_brother) recursive_delete_nodes(p->next_brother, true);
    decrease_counter(p);
    return true;
}

bool FileSystem::delete_node() {
    if (!check_path()) return false;
    treeNode *t = path.back();
    if (!check_node(t, __LINE__)) return false;
    path.pop_back();
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::rebuild_nodes(treeNode *p) {
    if (!check_path()) return false;
    int relation = 0;   // 0 next_brother 1 first_son
    std::stack<treeNode*> stk;
    stk.push(p);
    for (; check_node(path.back(), __LINE__) && path.back()->cnt > 1; path.pop_back()) {
        treeNode *t = new treeNode();
        (*t) = (*path.back());
        node_manager.increase_counter(t->link);
        if (relation == 1) t->first_son = stk.top();
        else t->next_brother = stk.top();
        relation = is_son();
        stk.push(t);
        if (!decrease_counter(path.back())) return false;
    }
    if (!check_node(path.back(), __LINE__)) return false;
    (relation ? path.back()->first_son : path.back()->next_brother) = stk.top();
    for (; !stk.empty(); stk.pop()) {
        path.push_back(stk.top());
    }
    if (path.back() == nullptr) path.pop_back();
    if (!check_path()) return false;
    return true;
}

bool FileSystem::switch_version(int version_id) {
    if (!version_manager.version_exist(version_id)) {
        logger.log("This version is not in the system.");
        return false;
    }
    CURRENT_VERSION = version_id;
    treeNode *p;
    if (!version_manager.get_version_pointer(version_id, p)) {
        return false;
    }
    path.clear();
    path.push_back(p);
    if (p->first_son == nullptr) {
        logger.log("The root directory does not have a \"first son\" folder, which is abnormal. Please check that the procedure is correct.", Logger::FATAL, __LINE__);
        return false;
    }
    path.push_back(path.back()->first_son);
    return true;
}

bool FileSystem::make_file(std::string name) {
    if (name_exist(name)) {
        logger.log(name + ": Name exist.");
        return false;
    }
    if (!goto_tail()) return false;
    treeNode *t = new treeNode(treeNode::FILE);
    t->link = node_manager.get_new_node(name);
    if (!rebuild_nodes(t)) return false;
    return true;
}

bool FileSystem::make_dir(std::string name) {
    if (name_exist(name)) {
        logger.log(name + ": Name exist.");
        return false;
    }
    if (!goto_tail()) return false;
    treeNode *t = new treeNode(treeNode::DIR);
    if (t == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    t->link = node_manager.get_new_node(name);
    if (!rebuild_nodes(t)) return false;
    return true;
}

bool FileSystem::change_directory(std::string name) {
    if (!go_to(name)) return false;
    if (path.back()->type != 1) {
        logger.log(name + ": Not a directory.");
        return false;
    }
    check_node(path.back()->first_son, __LINE__);
    path.push_back(path.back()->first_son);
    return true;
}

bool FileSystem::remove_file(std::string name) {
    if (!go_to(name)) return false;
    if (path.back()->type != 0) {
        logger.log(name + ": Not a file.");
        return false;
    }
    treeNode *t = path.back();
    path.pop_back();
    if (!check_path()) return false;
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::remove_dir(std::string name) {
    if (!go_to(name)) return false;
    if (path.back()->type != 1) {
        logger.log(name + ": Not a directory.");
        return false;
    }
    if (!check_path()) return false;
    treeNode *t = path.back();
    path.pop_back();        // 20211023
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!recursive_delete_nodes(t)) return false;
    return true;
}

bool FileSystem::update_name(std::string fr_name, std::string to_name) {
    if (!go_to(fr_name)) return false;
    treeNode *t = new treeNode();
    if (t == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    if (!check_path()) return false;
    treeNode *back = path.back();
    *t = *back;
    t->cnt = 1;
    t->link = node_manager.update_name(t->link, to_name);
    path.pop_back();
    if (!rebuild_nodes(t)) return false;
    if (!decrease_counter(back)) return false; 
    // 这里必须最后decrease，如果提前回导致节点丢失
    return true;
}

bool FileSystem::update_content(std::string name, std::string content) {
    if (!go_to(name)) false;
    if (!check_path()) return false;
    if (path.back()->type != 0) {
        logger.log(name + ": Not a file.");
        return false;
    }
    treeNode *back = path.back();
    treeNode *t = new treeNode();
    if (t == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    *t = *back;
    t->cnt = 1;
    t->link = node_manager.update_content(t->link, content);
    path.pop_back();
    if (!rebuild_nodes(t)) return false;
    if (!decrease_counter(back)) return false;
    return true;
}

bool FileSystem::get_content(std::string name, std::string &content) {
    if (!go_to(name)) return "";
    if (!check_path()) return false;
    if (path.back()->type != 0) {
        logger.log(name + ": Not a file.");
        return false;
    }
    content = node_manager.get_content(path.back()->link);
    return true;
}

bool FileSystem::tree(std::string &tree_info) {
    if (!check_path()) return false;
    if (!travel_tree(path.front(), tree_info)) return false;
    puts("");
    return true;
}

bool FileSystem::goto_last_dir() {
    return BSTree::goto_last_dir();
}
bool FileSystem::list_directory_contents(std::vector<std::string> &content) {
    return BSTree::list_directory_contents(content);
}

bool FileSystem::create_version(unsigned long long model_version, std::string info) {
    if (!version_manager.create_version(model_version, info)) return false;
    unsigned long long latest_version;
    if (!version_manager.get_latest_version(latest_version)) {
        return false;
    }
    if (!switch_version(latest_version)) return false;
    return true;
}

bool FileSystem::create_version(std::string info, unsigned long long model_version) {
    return create_version(model_version, info);
}

bool FileSystem::version(std::vector<std::pair<unsigned long long, versionNode>> &version_log) {
    return version_manager.get_version_log(version_log);
}

int FileSystem::get_current_version() {
    return CURRENT_VERSION;
}

#endif