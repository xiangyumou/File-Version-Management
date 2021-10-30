#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

#define wa std::cerr << "----WARN----" << '\n';
#define ll std::cerr << "----1111----" << '\n';
#define rr std::cerr << "----2222----" << '\n';

class Logger {
private:
    std::string log_file = "Logger.log";
    std::vector<std::string> logs;

    std::string get_time();
    ~Logger();

public:
    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    static Logger& get_logger();

    void log(std::string content);
};

// 获取新节点 修改节点信息 获取节点信息 删除节点

class treeNode {
public:
    std::string name;
    int type;   // 0: 文件  1: 文件夹   2: 头节点
    int cnt;
    treeNode *next_brother, *first_son;

    treeNode();
    treeNode(std::string name, int type, int cnt=1);
};

class FileSystem {
private:
    std::vector<treeNode*> path, version;
    Logger &logger = Logger::get_logger();

    void travel_tree(treeNode *p);
    void decrease_counter(treeNode *p);
    void recursive_delete_nodes(treeNode *p, bool delete_brother=false);
    void delete_node();
    void recursive_modify_counter(treeNode *p, int diff, bool modify_brother=false);
    bool is_son();
    void rebuild_nodes(treeNode *p);
    void init_version(treeNode *p, treeNode *vp);
    void goto_tail();
    void goto_head();
    bool name_exist(std::string name);
    bool go_to(std::string name);

public:
    FileSystem();
    void goto_last_dir();
    void switch_version(int version_id);
    void commit_version(int model_version=-1);
    std::vector<std::string> list_directory_contents();
    void make_file(std::string name);
    void make_dir(std::string name);
    void change_directory(std::string name);
    void remove_file(std::string name);
    void remove_dir(std::string name);
    void tree();
};

void print(FileSystem &fs) {
    auto content = fs.list_directory_contents();
    for (auto &it : content) {
        std::cout << it << ' ';
    }
    std::cout << '\n';
}

void do_cmd(FileSystem &fs, int op, int nm=-1) {
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    if (op == 0) fs.make_file(std::to_string(nm));
    else if (op == 1) fs.make_dir(std::to_string(nm));
    else if (op == 2) fs.change_directory(std::to_string(nm));
    else fs.goto_last_dir();
}

void test() {
    FileSystem fs;
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    std::vector<int> op({1, 0, 1, 2, 1, 3, 2, 0, 0, 3, 1, 2, 0, 3, 2, 2, 0});
    std::vector<int> nm({1, 2, 3, 1, 5, -1, 3, 6, 7, -1, 8, 8, 4, -1, 1, 5, 9});
    for (int i = 0; i < op.size(); i++) {
        do_cmd(fs, op[i], nm[i]);
    }
    fs.tree();
    fs.goto_last_dir();
    fs.goto_last_dir();
    fs.goto_last_dir();
    fs.goto_last_dir();
    fs.make_dir("小志");
    fs.tree();
    fs.commit_version(0);
    fs.remove_dir("8");
    fs.tree();
    fs.switch_version(0);
}

int main() {
    test();
    return 0;
}





                        /* ======= class treeNode ======= */
treeNode::treeNode() = default;
treeNode::treeNode(std::string name, int type, int cnt) {
    this->name = name, this->type = type;
    this->cnt = cnt;
    this->next_brother = nullptr;
    if (type == 0 || type == 2) this->first_son = nullptr;
    else if (type == 1) this->first_son = new treeNode("head node", 2, cnt);
}



                        /* ======= class FileSystem ======= */
void FileSystem::travel_tree(treeNode *p) {
    if (p == nullptr) return;
    static int tab_cnt = 1;
    for (unsigned int i = 0; i < tab_cnt; i++) {
        if (i < tab_cnt - 1) {
            std::cout << "    ";
        } else if (p->next_brother != nullptr) {
            std::cout << "├── ";
        } else {
            std::cout << "└── ";
        }
    }
    std::cout << p->name << '\n';
    // std::cout << p << ' ' << p->first_son << '\n';
    tab_cnt++;
    travel_tree(p->first_son);
    tab_cnt--;
    travel_tree(p->next_brother);
}

void FileSystem::decrease_counter(treeNode *p) {
    if (--p->cnt == 0) {
        logger.log("Node " + p->name + " will be deleted...");
        delete p;
        logger.log("Deleting completed.");
    }
}

void FileSystem::recursive_delete_nodes(treeNode *p, bool delete_brother) {
    if (p == nullptr) return;
    recursive_delete_nodes(p->first_son, true);
    if (delete_brother) recursive_delete_nodes(p->next_brother, true);
    decrease_counter(p);
}

void FileSystem::delete_node() {
    treeNode *t = path.back();
    path.pop_back();
    rebuild_nodes(t->next_brother);
    decrease_counter(t);
}

void FileSystem::recursive_modify_counter(treeNode *p, int diff, bool modify_brother) {
    if (p == nullptr) return;
    recursive_modify_counter(p->first_son, diff, true);
    if (modify_brother) recursive_modify_counter(p->next_brother, diff, true);
    p->cnt += diff;
    logger.log("The counter for node " + p->name + " has been incremented by one.");
}

bool FileSystem::is_son() {
    return path.back()->type == 2;
}

void FileSystem::rebuild_nodes(treeNode *p) {
    int relation = 0;   // 0 next_brother 1 first_son
    std::stack<treeNode*> stk;
    stk.push(p);
    // for (int i = 0; i < path.size(); i++) {
    //     std::cout << path[i]->name << '\n';
    // }
    for (; path.back()->cnt > 1; path.pop_back()) {
        treeNode *t = new treeNode();
        (*t) = (*path.back());
        if (relation == 1) t->first_son = stk.top();
        else t->next_brother = stk.top();
        relation = is_son();
        stk.push(t);
        decrease_counter(path.back());
    }
    (relation ? path.back()->first_son : path.back()->next_brother) = stk.top();
    for (; !stk.empty(); stk.pop()) {
        path.push_back(stk.top());
    }
    if (path.back() == nullptr) path.pop_back();
}

void FileSystem::init_version(treeNode *p, treeNode *vp) {
    p->first_son = vp->first_son;
    recursive_modify_counter(p, 1);
    switch_version(version.size() - 1);
}

void FileSystem::goto_tail() {
    while (path.back()->next_brother != nullptr) {
        path.push_back(path.back()->next_brother);
    }
}

void FileSystem::goto_head() {
    while (!is_son()) {
        path.pop_back();
    }
}

bool FileSystem::name_exist(std::string name) {
    auto dir_content = list_directory_contents();
    for (auto &nm : dir_content) {
        if (nm == name) return true;
    }
    return false;
}

bool FileSystem::go_to(std::string name) {
    if (!name_exist(name)) {
        std::cerr << "no file or directory named " << name << '\n';
        return false;
    }
    goto_head();
    while (path.back()->name != name) {
        if (path.back()->next_brother == nullptr) {
            return false;
        }
        path.push_back(path.back()->next_brother);
    }
    return true;
}

FileSystem::FileSystem() {
    commit_version();
}

void FileSystem::goto_last_dir() {
    goto_head();
    if (path.size() > 2) {
        path.pop_back();
    }
}

void FileSystem::switch_version(int version_id) {
    path.clear();
    path.push_back(version[version_id]);
    path.push_back(path.back()->first_son);
}

void FileSystem::commit_version(int model_version) {
    version.push_back(new treeNode("root dir" + std::to_string(version.size()), 1, 0));
    if (model_version != -1) delete version.back()->first_son;
    treeNode *model = model_version == -1 ? version.back() : version[model_version];
    init_version(version.back(), model);
}

std::vector<std::string> FileSystem::list_directory_contents() {
    std::vector<std::string> content;
    goto_head();
    while (path.back()->next_brother != nullptr) {
        content.push_back(path.back()->next_brother->name);
        path.push_back(path.back()->next_brother);
    }
    return content;
}

void FileSystem::make_file(std::string name) {
    if (name_exist(name)) {
        std::cerr << name << ": name exist" << '\n';
        return;
    }
    goto_tail();
    treeNode *t = new treeNode(name, 0);
    rebuild_nodes(t);
}

void FileSystem::make_dir(std::string name) {
    if (name_exist(name)) {
        std::cerr << name << ": name exist" << '\n';
        return;
    }
    goto_tail();
    treeNode *t = new treeNode(name, 1);
    // std::cout << t->name << ' ' << t->next_brother << ' ' << t->first_son << ' ' << t->first_son->name << '\n';
    rebuild_nodes(t);
}

void FileSystem::change_directory(std::string name) {
    go_to(name);
    if (path.back()->type != 1) {
        std::cerr << name << ": not a directory" << '\n';
        return;
    }
    path.push_back(path.back()->first_son);
}

void FileSystem::remove_file(std::string name) {
    if (!go_to(name)) return;
    if (path.back()->type != 1) {
        std::cerr << name << ": not a file" << '\n';
    }
    delete_node();
}

void FileSystem::remove_dir(std::string name) {
    if (!go_to(name)) return;
    if (path.back()->type != 1) {
        std::cerr << name << ": not a directory" << '\n';
        return;
    }
    treeNode *t = path.back();
    path.pop_back();        // 20211023
    rebuild_nodes(t->next_brother);
    recursive_delete_nodes(t);
}

void FileSystem::tree() {
    travel_tree(path.front());
}



                        /* ======= class Logger ======= */

std::string Logger::get_time() {
    static char t[100];
    time_t timep;
    time(&timep);
    struct tm* p = gmtime(&timep);
    sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    return std::string(t);
}

Logger::~Logger() {
    std::ofstream out(log_file, std::ios_base::app);
    for (auto &it : logs) {
        out << it << '\n';
    }
}

Logger::Logger() = default;
Logger::Logger(const Logger&) = delete;
Logger& Logger::operator=(const Logger&) = delete;

Logger& Logger::get_logger() {
    static Logger logger;
    return logger;
}

void Logger::log(std::string content) {
    logs.push_back("(" + get_time() + ") " + content);
}