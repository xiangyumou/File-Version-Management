/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "file_system.h"
#include <ctime>

FileSystem::FileSystem() {
    if (version_manager.empty()) {
        version_manager.create_version();
    }
    unsigned long long latest_version_id;
    if (!version_manager.get_latest_version(latest_version_id)) return;
    switch_version(static_cast<int>(latest_version_id));
}

bool FileSystem::decrease_counter(treeNode* p) {
    if (!check_node(p, __LINE__)) return false;
    if (--p->cnt == 0) {
        logger.log("Node " + node_manager.get_name(p->link) + " will be deleted...");
        node_manager.delete_node(p->link);
        delete p;
        logger.log("Deleting completed.");
    }
    return true;
}

bool FileSystem::recursive_delete_nodes(treeNode* p, bool delete_brother) {
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
    treeNode* t = path.back();
    if (!check_node(t, __LINE__)) return false;
    path.pop_back();
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::rebuild_nodes(treeNode* p) {
    if (!check_path()) return false;
    int relation = 0;
    std::stack<treeNode*> stk;
    stk.push(p);
    for (; check_node(path.back(), __LINE__) && path.back()->cnt > 1; path.pop_back()) {
        treeNode* t = new treeNode();
        (*t) = (*path.back());
        node_manager.increase_counter(t->link);
        if (relation == 1) t->first_son = stk.top();
        else t->next_brother = stk.top();
        relation = is_son() ? 1 : 0;
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

bool FileSystem::travel_tree(treeNode* p, std::string& tree_info, int tab_cnt) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    if (p->type == treeNode::HEAD_NODE) {
        travel_tree(p->next_brother, tree_info, tab_cnt);
        return true;
    }
    for (int i = 0; i < tab_cnt; i++) {
        if (i < tab_cnt - 1) {
            tree_info += "    ";
        } else if (p->next_brother != nullptr) {
            tree_info += "├── ";
        } else {
            tree_info += "└── ";
        }
    }
    tree_info += node_manager.get_name(p->link) + '\n';
    travel_tree(p->first_son, tree_info, tab_cnt + 1);
    travel_tree(p->next_brother, tree_info, tab_cnt);
    return true;
}

bool FileSystem::kmp(std::string str, std::string tar) {
    return str.find(tar) != std::string::npos;
}

bool FileSystem::travel_find(std::string name, std::vector<std::pair<std::string, std::vector<std::string>>>& res) {
    if (kmp(node_manager.get_name(path.back()->link), name)) {
        std::vector<std::string> p;
        if (!get_current_path(p)) return false;
        res.push_back(std::make_pair(node_manager.get_name(path.back()->link), p));
    }
    if (path.back()->next_brother != nullptr) {
        path.push_back(path.back()->next_brother);
        travel_find(name, res);
        path.pop_back();
    }
    if (path.back()->first_son != nullptr) {
        path.push_back(path.back()->first_son);
        travel_find(name, res);
        path.pop_back();
    }
    return true;
}

bool FileSystem::switch_version(int version_id) {
    if (!version_manager.version_exist(static_cast<unsigned long long>(version_id))) {
        logger.log("This version is not in the system.");
        return false;
    }
    CURRENT_VERSION = version_id;
    treeNode* p;
    if (!version_manager.get_version_pointer(static_cast<unsigned long long>(version_id), p)) {
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
    treeNode* t = new treeNode(treeNode::FILE);
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
    treeNode* t = new treeNode(treeNode::DIR);
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
    if (path.back()->type != treeNode::DIR) {
        logger.log(name + ": Not a directory.");
        return false;
    }
    if (!check_node(path.back()->first_son, __LINE__)) {
        return false;
    }
    path.push_back(path.back()->first_son);
    return true;
}

bool FileSystem::remove_file(std::string name) {
    if (!go_to(name)) return false;
    if (path.back()->type != treeNode::FILE) {
        logger.log(name + ": Not a file.");
        return false;
    }
    treeNode* t = path.back();
    path.pop_back();
    if (!check_path()) return false;
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::remove_dir(std::string name) {
    if (!go_to(name)) return false;
    if (path.back()->type != treeNode::DIR) {
        logger.log(name + ": Not a directory.");
        return false;
    }
    if (!check_path()) return false;
    treeNode* t = path.back();
    path.pop_back();
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!recursive_delete_nodes(t)) return false;
    return true;
}

bool FileSystem::update_name(std::string fr_name, std::string to_name) {
    if (!go_to(fr_name)) return false;
    if (name_exist(to_name)) {
        logger.log(to_name + ": Name exists.", Logger::WARNING, __LINE__);
        return false;
    }
    treeNode* t = new treeNode();
    if (t == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    if (!check_path()) return false;
    treeNode* back = path.back();
    *t = *back;
    t->cnt = 1;
    t->link = node_manager.update_name(t->link, to_name);
    path.pop_back();
    if (!rebuild_nodes(t)) return false;
    if (!decrease_counter(back)) return false;
    return true;
}

bool FileSystem::update_content(std::string name, std::string content) {
    if (!go_to(name)) return false;
    if (!check_path()) return false;
    if (path.back()->type != treeNode::FILE) {
        logger.log(name + ": Not a file.");
        return false;
    }
    treeNode* back = path.back();
    treeNode* t = new treeNode();
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

bool FileSystem::get_content(std::string name, std::string& content) {
    if (!go_to(name)) return false;
    if (!check_path()) return false;
    if (path.back()->type != treeNode::FILE) {
        logger.log(name + ": Not a file.");
        return false;
    }
    content = node_manager.get_content(path.back()->link);
    return true;
}

bool FileSystem::tree(std::string& tree_info) {
    if (!check_path()) return false;
    tree_info = node_manager.get_name(path.front()->link) + '\n';
    if (!travel_tree(path.front()->first_son, tree_info)) return false;
    return true;
}

bool FileSystem::goto_last_dir() {
    return BSTree::goto_last_dir();
}

bool FileSystem::list_directory_contents(std::vector<std::string>& content) {
    return BSTree::list_directory_contents(content);
}

bool FileSystem::create_version(unsigned long long model_version, std::string info) {
    if (!version_manager.create_version(model_version, info)) return false;
    unsigned long long latest_version_id;
    if (!version_manager.get_latest_version(latest_version_id)) return false;
    if (!switch_version(static_cast<int>(latest_version_id))) return false;
    return true;
}

bool FileSystem::create_version(std::string info, unsigned long long model_version) {
    return create_version(model_version, info);
}

bool FileSystem::version(std::vector<std::pair<unsigned long long, versionNode>>& version_log) {
    return version_manager.get_version_log(version_log);
}

bool FileSystem::get_update_time(std::string name, std::string& update_time) {
    if (!go_to(name)) return false;
    update_time = node_manager.get_update_time(path.back()->link);
    return true;
}

bool FileSystem::get_create_time(std::string name, std::string& create_time) {
    if (!go_to(name)) return false;
    create_time = node_manager.get_create_time(path.back()->link);
    return true;
}

bool FileSystem::get_type(std::string name, treeNode::TYPE& type) {
    if (!go_to(name)) return false;
    type = path.back()->type;
    return true;
}

bool FileSystem::get_current_path(std::vector<std::string>& p) {
    return BSTree::get_current_path(p);
}

bool FileSystem::Find(std::string name, std::vector<std::pair<std::string, std::vector<std::string>>>& res) {
    auto path_backup = path;
    path.clear();
    path.push_back(path_backup.front());
    travel_find(name, res);
    path = path_backup;
    return true;
}

int FileSystem::get_current_version() {
    return CURRENT_VERSION;
}
