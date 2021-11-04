#ifndef BS_TREE_CPP
#define BS_TREE_CPP

#include "node_manager.cpp"
#include "logger.cpp"
#include <vector>

struct treeNode {
    enum TYPE {
        FILE = 0, DIR, HEAD_NODE
    };

    TYPE type;
    int cnt, link;
    treeNode *next_brother, *first_son;

    treeNode();
    treeNode(TYPE type);
};

class BSTree {
private:
    Logger &logger = Logger::get_logger();
    NodeManager &node_manager = NodeManager::get_node_manager();
protected:
    std::vector<treeNode*> path;

    bool check_path();
    bool check_node(treeNode *p, int line);
    bool travel_tree(treeNode *p, std::string &tree_info);
    bool is_son();
    bool goto_tail();
    bool goto_head();
    bool name_exist(std::string name);
    bool go_to(std::string name);

    bool goto_last_dir();
    bool list_directory_contents(std::vector<std::string> &content);
};



                        /* ======= struct treeNode ======= */
treeNode::treeNode() = default;
treeNode::treeNode(TYPE type) {
    this->type = type;
    this->cnt = 1;
    this->next_brother = nullptr;
    this->link = -1;
    if (type == FILE || type == HEAD_NODE) this->first_son = nullptr;
    else if (type == DIR) this->first_son = new treeNode(HEAD_NODE);
}


                        /* ======= class BSTree ======= */

bool BSTree::check_path() {
    if (path.empty()) {
        logger.log("Path is empty. This not normal.", Logger::FATAL, __LINE__);
        return false;
    }
    return true;
}

bool BSTree::check_node(treeNode *p, int line) {
    if (p == nullptr) {
        logger.log("The pointer is empty, please check whether the program is correct.", Logger::FATAL, line);
        return false;
    }
    if (p->cnt <= 0) {
        logger.log("The node counter is already less than or equal to 0, please check the program!", Logger::FATAL, line);
        return false;
    }
    return true;
}

bool BSTree::travel_tree(treeNode *p,std::string &tree_info) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    static int tab_cnt = 1;
    if (p->type == 2) {
        travel_tree(p->next_brother, tree_info);
        return true;
    }
    for (unsigned int i = 0; i < tab_cnt; i++) {
        if (i < tab_cnt - 1) {
            tree_info += "    ";
        } else if (p->next_brother != nullptr) {
            tree_info += "├── ";
        } else {
            tree_info += "└── ";
        }
    }
    tree_info += node_manager.get_name(p->link) + '\n';
    tab_cnt++;
    travel_tree(p->first_son, tree_info);
    tab_cnt--;
    travel_tree(p->next_brother, tree_info);
    return true;
}

bool BSTree::is_son() {
    if (!check_path()) return false;
    return path.back()->type == 2;
}

bool BSTree::goto_tail() {
    if (!check_path()) return false;
    while (path.back()->next_brother != nullptr) {
        path.push_back(path.back()->next_brother);
    }
    if (!check_path()) return false;
    return true;
}

bool BSTree::goto_head() {
    if (!check_path()) return false;
    for (; !path.empty() && !is_son(); path.pop_back());
    if (!check_path()) return false;
    return true;
}

bool BSTree::name_exist(std::string name) {
    std::vector<std::string> dir_content;
    if (!list_directory_contents(dir_content)) return false;
    for (auto &nm : dir_content) {
        if (nm == name) return true;
    }
    return false;
}

bool BSTree::go_to(std::string name) {
    if (!name_exist(name)) {
        logger.log("no file or directory named " + name, Logger::WARNING, __LINE__);
        return false;
    }
    if (!goto_head()) return false;
    while (node_manager.get_name(path.back()->link) != name) {
        if (path.back()->next_brother == nullptr) {
            return false;
        }
        path.push_back(path.back()->next_brother);
    }
    return true;
}

bool BSTree::goto_last_dir() {
    if (!goto_head()) return false;
    if (path.size() > 2) {
        path.pop_back();
    }
    if (!check_path()) return false;
    return true;
}

bool BSTree::list_directory_contents(std::vector<std::string> &content) {
    if (!goto_head()) return false;
    if (!check_path()) return false;
    while (path.back()->next_brother != nullptr) {
        content.push_back(node_manager.get_name(path.back()->next_brother->link));
        path.push_back(path.back()->next_brother);
    }
    return true;
}

#endif