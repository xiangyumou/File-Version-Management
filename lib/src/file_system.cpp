/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "file_system.h"
#include "logger.h"
#include "node_manager.h"
#include <ctime>

// Helper to get logger reference
ffvms::ILogger& FileSystem::get_logger_ref() {
    if (logger_) return *logger_;
    return Logger::get_logger();
}

// Helper to get node manager reference
ffvms::INodeManager& FileSystem::get_node_manager_ref() {
    if (node_manager_) return *node_manager_;
    return NodeManager::get_node_manager();
}

FileSystem::FileSystem() 
    : tree_(std::make_unique<BSTree>())
    , logger_(nullptr)
    , node_manager_(nullptr) {
    if (version_manager_.empty()) {
        version_manager_.create_version();
    }
    unsigned long long latest_version_id;
    if (!version_manager_.get_latest_version(latest_version_id)) return;
    switch_version(static_cast<int>(latest_version_id));
}

FileSystem::FileSystem(ffvms::ILogger* logger, ffvms::INodeManager* node_manager)
    : tree_(std::make_unique<BSTree>(logger, node_manager))
    , logger_(logger)
    , node_manager_(node_manager) {
    if (version_manager_.empty()) {
        version_manager_.create_version();
    }
    unsigned long long latest_version_id;
    if (!version_manager_.get_latest_version(latest_version_id)) return;
    switch_version(static_cast<int>(latest_version_id));
}

bool FileSystem::decrease_counter(treeNode* p) {
    if (!tree_->check_node(p, __LINE__)) return false;
    if (--p->cnt == 0) {
        get_logger_ref().log("Node " + get_node_manager_ref().get_name(p->link) + " will be deleted...", 
                             ffvms::LogLevel::INFO, __LINE__);
        get_node_manager_ref().delete_node(p->link);
        delete p;
        get_logger_ref().log("Deleting completed.", ffvms::LogLevel::INFO, __LINE__);
    }
    return true;
}

bool FileSystem::recursive_delete_nodes(treeNode* p, bool delete_brother) {
    if (p == nullptr) {
        get_logger_ref().log("Get a null pointer in line " + std::to_string(__LINE__), 
                             ffvms::LogLevel::WARNING, __LINE__);
        return false;
    }
    recursive_delete_nodes(p->first_son, true);
    if (delete_brother) recursive_delete_nodes(p->next_brother, true);
    decrease_counter(p);
    return true;
}

bool FileSystem::delete_node() {
    if (!tree_->check_path()) return false;
    treeNode* t = tree_->path.back();
    if (!tree_->check_node(t, __LINE__)) return false;
    tree_->path.pop_back();
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::rebuild_nodes(treeNode* p) {
    if (!tree_->check_path()) return false;
    int relation = 0;
    std::stack<treeNode*> stk;
    stk.push(p);
    for (; tree_->check_node(tree_->path.back(), __LINE__) && tree_->path.back()->cnt > 1; tree_->path.pop_back()) {
        treeNode* t = new treeNode();
        (*t) = (*tree_->path.back());
        get_node_manager_ref().increase_counter(t->link);
        if (relation == 1) t->first_son = stk.top();
        else t->next_brother = stk.top();
        relation = tree_->is_son() ? 1 : 0;
        stk.push(t);
        if (!decrease_counter(tree_->path.back())) return false;
    }
    if (!tree_->check_node(tree_->path.back(), __LINE__)) return false;
    (relation ? tree_->path.back()->first_son : tree_->path.back()->next_brother) = stk.top();
    for (; !stk.empty(); stk.pop()) {
        tree_->path.push_back(stk.top());
    }
    if (tree_->path.back() == nullptr) tree_->path.pop_back();
    if (!tree_->check_path()) return false;
    return true;
}

bool FileSystem::travel_tree(treeNode* p, std::string& tree_info, int tab_cnt) {
    if (p == nullptr) {
        get_logger_ref().log("Get a null pointer in line " + std::to_string(__LINE__), 
                             ffvms::LogLevel::WARNING, __LINE__);
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
    tree_info += get_node_manager_ref().get_name(p->link) + '\n';
    travel_tree(p->first_son, tree_info, tab_cnt + 1);
    travel_tree(p->next_brother, tree_info, tab_cnt);
    return true;
}

bool FileSystem::kmp(const std::string& str, const std::string& tar) {
    return str.find(tar) != std::string::npos;
}

bool FileSystem::travel_find(const std::string& name, 
                             std::vector<std::pair<std::string, std::vector<std::string>>>& res) {
    if (kmp(get_node_manager_ref().get_name(tree_->path.back()->link), name)) {
        std::vector<std::string> p;
        if (!get_current_path(p)) return false;
        res.push_back(std::make_pair(get_node_manager_ref().get_name(tree_->path.back()->link), p));
    }
    if (tree_->path.back()->next_brother != nullptr) {
        tree_->path.push_back(tree_->path.back()->next_brother);
        travel_find(name, res);
        tree_->path.pop_back();
    }
    if (tree_->path.back()->first_son != nullptr) {
        tree_->path.push_back(tree_->path.back()->first_son);
        travel_find(name, res);
        tree_->path.pop_back();
    }
    return true;
}

bool FileSystem::switch_version(int version_id) {
    if (!version_manager_.version_exist(static_cast<unsigned long long>(version_id))) {
        get_logger_ref().log("This version is not in the system.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    CURRENT_VERSION = version_id;
    treeNode* p;
    if (!version_manager_.get_version_pointer(static_cast<unsigned long long>(version_id), p)) {
        return false;
    }
    tree_->path.clear();
    tree_->path.push_back(p);
    if (p->first_son == nullptr) {
        get_logger_ref().log("The root directory does not have a \"first son\" folder, which is abnormal.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    tree_->path.push_back(tree_->path.back()->first_son);
    return true;
}

bool FileSystem::make_file(const std::string& name) {
    if (tree_->name_exist(name)) {
        get_logger_ref().log(name + ": Name exist.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    if (!tree_->goto_tail()) return false;
    treeNode* t = new treeNode(treeNode::FILE);
    t->link = get_node_manager_ref().get_new_node(name);
    if (!rebuild_nodes(t)) return false;
    return true;
}

bool FileSystem::make_dir(const std::string& name) {
    if (tree_->name_exist(name)) {
        get_logger_ref().log(name + ": Name exist.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    if (!tree_->goto_tail()) return false;
    treeNode* t = new treeNode(treeNode::DIR);
    if (t == nullptr) {
        get_logger_ref().log("The system did not allocate memory for this operation.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    t->link = get_node_manager_ref().get_new_node(name);
    if (!rebuild_nodes(t)) return false;
    return true;
}

bool FileSystem::change_directory(const std::string& name) {
    if (!tree_->go_to(name)) return false;
    if (tree_->path.back()->type != treeNode::DIR) {
        get_logger_ref().log(name + ": Not a directory.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    if (!tree_->check_node(tree_->path.back()->first_son, __LINE__)) {
        return false;
    }
    tree_->path.push_back(tree_->path.back()->first_son);
    return true;
}

bool FileSystem::remove_file(const std::string& name) {
    if (!tree_->go_to(name)) return false;
    if (tree_->path.back()->type != treeNode::FILE) {
        get_logger_ref().log(name + ": Not a file.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    treeNode* t = tree_->path.back();
    tree_->path.pop_back();
    if (!tree_->check_path()) return false;
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!decrease_counter(t)) return false;
    return true;
}

bool FileSystem::remove_dir(const std::string& name) {
    if (!tree_->go_to(name)) return false;
    if (tree_->path.back()->type != treeNode::DIR) {
        get_logger_ref().log(name + ": Not a directory.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    if (!tree_->check_path()) return false;
    treeNode* t = tree_->path.back();
    tree_->path.pop_back();
    if (!rebuild_nodes(t->next_brother)) return false;
    if (!recursive_delete_nodes(t)) return false;
    return true;
}

bool FileSystem::update_name(const std::string& fr_name, const std::string& to_name) {
    if (!tree_->go_to(fr_name)) return false;
    if (tree_->name_exist(to_name)) {
        get_logger_ref().log(to_name + ": Name exists.", ffvms::LogLevel::WARNING, __LINE__);
        return false;
    }
    treeNode* t = new treeNode();
    if (t == nullptr) {
        get_logger_ref().log("The system did not allocate memory for this operation.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    if (!tree_->check_path()) return false;
    treeNode* back = tree_->path.back();
    *t = *back;
    t->cnt = 1;
    t->link = get_node_manager_ref().update_name(t->link, to_name);
    tree_->path.pop_back();
    if (!rebuild_nodes(t)) return false;
    if (!decrease_counter(back)) return false;
    return true;
}

bool FileSystem::update_content(const std::string& name, const std::string& content) {
    if (!tree_->go_to(name)) return false;
    if (!tree_->check_path()) return false;
    if (tree_->path.back()->type != treeNode::FILE) {
        get_logger_ref().log(name + ": Not a file.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    treeNode* back = tree_->path.back();
    treeNode* t = new treeNode();
    if (t == nullptr) {
        get_logger_ref().log("The system did not allocate memory for this operation.", 
                             ffvms::LogLevel::FATAL, __LINE__);
        return false;
    }
    *t = *back;
    t->cnt = 1;
    t->link = get_node_manager_ref().update_content(t->link, content);
    tree_->path.pop_back();
    if (!rebuild_nodes(t)) return false;
    if (!decrease_counter(back)) return false;
    return true;
}

bool FileSystem::get_content(const std::string& name, std::string& content) {
    if (!tree_->go_to(name)) return false;
    if (!tree_->check_path()) return false;
    if (tree_->path.back()->type != treeNode::FILE) {
        get_logger_ref().log(name + ": Not a file.", ffvms::LogLevel::INFO, __LINE__);
        return false;
    }
    content = get_node_manager_ref().get_content(tree_->path.back()->link);
    return true;
}

bool FileSystem::tree(std::string& tree_info) {
    if (!tree_->check_path()) return false;
    tree_info = get_node_manager_ref().get_name(tree_->path.front()->link) + '\n';
    if (!travel_tree(tree_->path.front()->first_son, tree_info)) return false;
    return true;
}

bool FileSystem::goto_last_dir() {
    return tree_->goto_last_dir();
}

bool FileSystem::list_directory_contents(std::vector<std::string>& content) {
    return tree_->list_directory_contents(content);
}

bool FileSystem::create_version(unsigned long long model_version, const std::string& info) {
    if (!version_manager_.create_version(model_version, info)) return false;
    unsigned long long latest_version_id;
    if (!version_manager_.get_latest_version(latest_version_id)) return false;
    if (!switch_version(static_cast<int>(latest_version_id))) return false;
    return true;
}

bool FileSystem::create_version(const std::string& info, unsigned long long model_version) {
    return create_version(model_version, info);
}

bool FileSystem::version(std::vector<std::pair<unsigned long long, versionNode>>& version_log) {
    return version_manager_.get_version_log(version_log);
}

bool FileSystem::get_update_time(const std::string& name, std::string& update_time) {
    if (!tree_->go_to(name)) return false;
    update_time = get_node_manager_ref().get_update_time(tree_->path.back()->link);
    return true;
}

bool FileSystem::get_create_time(const std::string& name, std::string& create_time) {
    if (!tree_->go_to(name)) return false;
    create_time = get_node_manager_ref().get_create_time(tree_->path.back()->link);
    return true;
}

bool FileSystem::get_type(const std::string& name, treeNode::TYPE& type) {
    if (!tree_->go_to(name)) return false;
    type = tree_->path.back()->type;
    return true;
}

bool FileSystem::get_current_path(std::vector<std::string>& p) {
    return tree_->get_current_path(p);
}

bool FileSystem::Find(const std::string& name, 
                      std::vector<std::pair<std::string, std::vector<std::string>>>& res) {
    auto path_backup = tree_->path;
    tree_->path.clear();
    tree_->path.push_back(path_backup.front());
    travel_find(name, res);
    tree_->path = path_backup;
    return true;
}

int FileSystem::get_current_version() {
    return CURRENT_VERSION;
}
