#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#define wa std::cerr << "----WARN----" << '\n';
#define ll std::cerr << "----1111----" << '\n';
#define rr std::cerr << "----2222----" << '\n';

class Logger {
private:
    // Set the log file name here
    std::string log_file = "Logger.log";

    // Gets the current time as a string
    std::string get_time();

public:
    Logger();

    /**
     * Unless there is no other processing method, the User-oriented and 
     * developer-oriented functions in the program only returns true or false 
     * to indicate whether the requested operation is successfully executed.
     * 
     * If the execution is successful and the function has a return value, 
     * then the return value will be stored in this variable; if it is not 
     * successfully executed, the reason for not being successfully executed 
     * will be stored in this variable.
    */
    std::string informaion;

    /**
     * This function can be used in multiple places to obtain a logger.
     * Log management is implemented by the Logger class. 
     * You only need to correctly mark the log level of the information that 
     * needs to be recorded and the line number where the information is 
     * located when necessary, and output it through the log function.
    */
    static Logger& get_logger();

    /**
    * An explanation of the four types of log registration:
    * INFO: 
    * Normal logs are used to record what operations the program has performed.
    * At this level, logs are only printed to log files and not to the console.
    * 
    * DEBUG: 
    * This level is mainly used when debugging programs.
    * Using this level will output information to the console using cerr.
    * 
    * WARNING:
    * This level is used when the user does something wrong.
    * For example, if the user tries to access a folder that does not exist, the
    * function will only return false, because it is printed this way.
    * To find out exactly what happened, you can use the logger's "information" variable.
    * 
    * FATAL:
    * This level records information that may cause the program to crash.
    * At this level, information will be forced to be output to the console 
    * through cerr and the log level will be marked.
    * Due to the limited level of the author, errors of this level are 
    * almost never caught. QAQ
    */
    enum LOG_LEVEL {
        INFO = 0, DEBUG, WARNING, FATAL
    };

    /**
     * Use this function to record logs. 
     * "content" is the content to be recorded, and "level" is the log level. 
     * For a detailed introduction to "level", please see above.
     * If you use a log level other than INFO, you must provide the line 
     * number where the log function is called, that is, __LINE__.
    */
    void log(std::string content, LOG_LEVEL level=INFO, int line=__LINE__);
};

class NodeManager {
private:
    class Node {
        public:
            std::string name;
            std::string create_time;
            std::string update_time;
            std::string content;

            std::string get_time();
            Node();
            Node(std::string name);
            void update_update_time();
    };

    std::map<int, std::pair<int, Node>> mp;

    bool node_exist(int id);
    int get_new_id();
public:
    int get_new_node(std::string name);
    void delete_node(int idx);
    int update_content(int idx, std::string content);
    int update_name(int idx, std::string name);
    std::string get_content(int idx);
    std::string get_name(int idx);
    std::string get_update_time(int idx);
    std::string get_create_time(int idx);
    void increase_counter(int idx);
    int _get_counter(int idx);
};

class FileSystem {
private:
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

    std::vector<treeNode*> version;
    std::vector<treeNode*> path;
    Logger &logger = Logger::get_logger();
    NodeManager node_manager;

    bool check_path();
    bool check_node(treeNode *p, int line);
    bool travel_tree(treeNode *p);
    bool decrease_counter(treeNode *p);
    bool recursive_delete_nodes(treeNode *p, bool delete_brother=false);
    bool delete_node();
    bool recursive_increase_counter(treeNode *p, bool modify_brother=false);
    bool is_son();
    bool rebuild_nodes(treeNode *p);
    bool init_version(treeNode *p, treeNode *vp);
    bool goto_tail();
    bool goto_head();
    bool name_exist(std::string name);
    bool go_to(std::string name);

public:
    FileSystem();
    bool goto_last_dir();
    bool switch_version(int version_id);
    bool commit_version(int model_version=-1);
    std::vector<std::string> list_directory_contents();
    bool make_file(std::string name);
    bool make_dir(std::string name);
    bool change_directory(std::string name);
    bool remove_file(std::string name);
    bool remove_dir(std::string name);
    void update_name(std::string fr_name, std::string to_name) {
        if (!go_to(fr_name)) return;
        treeNode *t = new treeNode();
        treeNode *back = path.back();
        *t = *back;
        t->cnt = 1;
        t->link = node_manager.update_name(t->link, to_name);
        path.pop_back();
        rebuild_nodes(t);
        decrease_counter(back);  // 这里必须最后decrease，如果提前回导致节点丢失
    }
    void update_content(std::string name, std::string content) {
        if (!go_to(name)) return;
        if (path.back()->type != 0) {
            std::cerr << name << " is not a file." << '\n';
            return;
        }
        treeNode *t = new treeNode();
        treeNode *back = path.back();
        *t = *back;
        t->cnt = 1;
        t->link = node_manager.update_content(t->link, content);
        path.pop_back();
        rebuild_nodes(t);
        decrease_counter(back);
    }
    std::string get_content(std::string name) {
        if (!go_to(name)) return "";
        if (path.back()->type != 0) {
            std::cerr << name << " is not a file." << '\n';
            return "";
        }
        return node_manager.get_content(path.back()->link);
    }
    bool tree();
};

void print(FileSystem &fs) {
    auto content = fs.list_directory_contents();
    for (auto &it : content) {
        std::cout << it << ' ';
    }
    std::cout << '\n';
}

void do_cmd(FileSystem &fs, int op, std::string nm="-1") {
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    Logger &logger = Logger::get_logger();
    if (op == 0) fs.make_file(nm);
    else if (op == 1) fs.make_dir(nm);
    else if (op == 2) fs.change_directory(nm);
    else fs.goto_last_dir();
}

void test() {
    FileSystem fs;
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    std::vector<int> op({1, 0, 1, 2, 1, 3, 2, 0, 0, 3, 1, 2, 0, 3, 2, 2, 0, 3, 3, 3});
    std::vector<std::string> nm({"1", "2", "3", "1", "5", "-1", "3", "6", "7", "-1", "8", "8", "4", "-1", "1", "5", "9", "-1", "-1", "-1"});
    for (int i = 0; i < op.size(); i++) {
        do_cmd(fs, op[i], nm[i]);
    }
    fs.update_content("2", "123123123");
    fs.commit_version(0);
    fs.update_content("2", "312312312312");
    fs.switch_version(0);
    fs.commit_version(0);
    fs.remove_file("2");
    fs.switch_version(0);
    fs.remove_dir("3");
    fs.tree();
}

int main() {
    test();
    return 0;
}





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






                        /* ======= class NodeManager ======= */

bool NodeManager::node_exist(int id) {
    return mp.count(id);
}

int NodeManager::get_new_id() {
    int id = rand() * rand();
    id = id < 0 ? -id : id;
    while (node_exist(id)) {
        id = rand() * rand();
        id = id < 0 ? -id : id;
    }
    return id;
}

int NodeManager::get_new_node(std::string name) {
    int new_id = get_new_id();
    mp[new_id] = std::make_pair(1, Node(name));
    return new_id;
};

void NodeManager::delete_node(int idx) {
    if (!node_exist(idx)) return;
    if (--mp[idx].first == 0) {
        mp.erase(mp.find(idx));
    }
}

int NodeManager::update_content(int idx, std::string content) {
    if (!node_exist(idx)) return -1;
    std::string name = get_name(idx);
    std::string create_time = get_update_time(idx);
    delete_node(idx);
    idx = get_new_node(name);
    mp[idx].second.content = content;
    mp[idx].second.create_time = create_time;
    return idx;
}

int NodeManager::update_name(int idx, std::string name) {
    if (!node_exist(idx)) return -1;
    std::string content = get_content(idx);
    std::string create_time = get_update_time(idx);
    delete_node(idx);
    idx = get_new_node(name);
    mp[idx].second.content = content;
    mp[idx].second.create_time = create_time;
    return idx;
}

std::string NodeManager::get_content(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.content;
}

std::string NodeManager::get_name(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.name;
}

std::string NodeManager::get_update_time(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.update_time;
}

std::string NodeManager::get_create_time(int idx) {
    if (!node_exist(idx)) return "";
    return mp[idx].second.create_time;
}

void NodeManager::increase_counter(int idx) {
    if (!node_exist(idx)) return;
    mp[idx].first ++;
}

int NodeManager::_get_counter(int idx) {
    if (!node_exist(idx)) return -1;
    return mp[idx].first;
}






                        /* ======= class treeNode ======= */
FileSystem::treeNode::treeNode() = default;
FileSystem::treeNode::treeNode(TYPE type) {
    this->type = type;
    this->cnt = 1;
    this->next_brother = nullptr;
    this->link = -1;
    if (type == FILE || type == HEAD_NODE) this->first_son = nullptr;
    else if (type == DIR) this->first_son = new treeNode(HEAD_NODE);
}



                        /* ======= class FileSystem ======= */
bool FileSystem::check_path() {
    if (path.empty()) {
        logger.log("Path is empty. This not normal.", Logger::FATAL, __LINE__);
        return false;
    }
    return true;
}

bool FileSystem::check_node(treeNode *p, int line) {
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

bool FileSystem::travel_tree(treeNode *p) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    static int tab_cnt = 1;
    if (p->type == 2) {
        travel_tree(p->next_brother);
        return true;
    }
    for (unsigned int i = 0; i < tab_cnt; i++) {
        if (i < tab_cnt - 1) {
            std::cout << "    ";
        } else if (p->next_brother != nullptr) {
            std::cout << "├── ";
        } else {
            std::cout << "└── ";
        }
    }
    std::cout << node_manager.get_name(p->link) << '\n';
    tab_cnt++;
    travel_tree(p->first_son);
    tab_cnt--;
    travel_tree(p->next_brother);
    return true;
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

bool FileSystem::recursive_increase_counter(treeNode *p, bool modify_brother) {
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

bool FileSystem::is_son() {
    if (!check_path()) return false;
    return path.back()->type == 2;
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

bool FileSystem::init_version(treeNode *p, treeNode *vp) {
    if (p == nullptr || vp == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__), Logger::FATAL, __LINE__);
        return false;
    }
    p->first_son = vp->first_son;
    if (!recursive_increase_counter(p, 1)) return false;
    if (!switch_version(version.size() - 1)) return false;
    return true;
}

bool FileSystem::goto_tail() {
    if (!check_path()) return false;
    while (path.back()->next_brother != nullptr) {
        path.push_back(path.back()->next_brother);
    }
    if (!check_path()) return false;
    return true;
}

bool FileSystem::goto_head() {
    if (!check_path()) return false;
    for (; !path.empty() && !is_son(); path.pop_back());
    if (!check_path()) return false;
    return true;
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
        logger.log("no file or directory named " + name);
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

FileSystem::FileSystem() {
    commit_version();
}

bool FileSystem::goto_last_dir() {
    if (!goto_head()) return false;
    if (path.size() > 2) {
        path.pop_back();
    }
    if (!check_path()) return false;
    return true;
}

bool FileSystem::switch_version(int version_id) {
    if (version_id < 0 || version_id >= version.size()) {
        logger.log("This version is not in the system.");
        return false;
    }
    path.clear();
    path.push_back(version[version_id]);
    path.push_back(path.back()->first_son);
    return true;
}

bool FileSystem::commit_version(int model_version) {
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

std::vector<std::string> FileSystem::list_directory_contents() {
    std::vector<std::string> content;
    if (!goto_head()) return content;
    while (path.back()->next_brother != nullptr) {
        content.push_back(node_manager.get_name(path.back()->next_brother->link));
        path.push_back(path.back()->next_brother);
    }
    return content;
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

bool FileSystem::tree() {
    if (!check_path()) return false;
    if (!travel_tree(path.front())) return false;
    puts("");
    return true;
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

Logger::Logger() = default;

Logger& Logger::get_logger() {
    static Logger logger;
    return logger;
}

void Logger::log(std::string content, LOG_LEVEL level, int line) {
    static std::ofstream out(log_file, std::ios_base::app);
    std::string informaion = " (" + get_time() + ") " + content;
    if (level == INFO) {
        out << "level: INFO " << '\n' << informaion << std::endl;
    } else if (level == DEBUG) {
        out << "level: DEBUG " << '\n' <<  "line: " << line << informaion << std::endl;
        std::cerr << informaion << std::endl;
    } else if (level == WARNING) {
        out << "level: WARNING " << '\n' << "line: " << line << informaion << std::endl;
    } else {
        out << "level: FATAL " << '\n' << "line: " << line << informaion << std::endl;
        std::cerr << "level: FATAL " << '\n' << "line: " << line << informaion << std::endl;
    }
}