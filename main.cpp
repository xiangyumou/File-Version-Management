#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#define wa std::cerr << "----WARN----" << '\n';
#define ll std::cerr << "----1111----" << '\n';
#define rr std::cerr << "----2222----" << '\n';

typedef std::vector<std::vector<std::string>> vvs;

class Logger {
private:
    // Set the log file name here
    std::string log_file = "log.chm";

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
    std::string *information;

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

class Encryptor {
protected:
static const int N = 1 << 10;
private:
    static const char PLACEHOLDER = '\0';

    struct Complex {
        double a, b;
        Complex();
        Complex(double a, double b);
        Complex operator+(const Complex &r) const;
        Complex operator-(const Complex &r) const;
        Complex operator*(const Complex &r) const;
    } buf[N << 1], block[N];

    void fft(Complex a[], int n, int type);
    bool encrypt_block(std::vector<std::pair<double, double>> &res);
    bool decrypt_block(std::vector<int> &res);

protected:

    bool encrypt_sequence(std::vector<int> &sequence, std::vector<std::pair<double, double>> &res);
    bool decrypt_sequence(std::vector<std::pair<double, double>> &sequence, std::vector<int> &res);
};

class Saver : private Encryptor {
private:
    struct dataNode {
        unsigned long long name_hash, data_hash;
        int len;
        std::vector<std::pair<double, double>> data;

        dataNode();
        dataNode(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> &data);
    };

    std::string data_file = "data.chm";
    std::map<unsigned long long, dataNode> mp;
    Logger logger = Logger::get_logger();

    template <class T>
    unsigned long long get_hash(T &s);

    bool load_file();

    /**
     * 在map中的存储方式
     * 以name_hash作为主键，会检索出一个dataNode，里面包括了name_hash, data_hash, len, data.
     * 其中 len为data的pair的对数 / N
    */
    void save_data(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> data);

public:
    Saver();

    /**
     * 保存的格式：
     * name_hash data_hash len 后面跟len对浮点数
    */
    ~Saver();

    /**
     * data字符串的格式:
     * 数据块的个数 
     * 每个数据块中数据的个数
     * 数据的长度
     * 数据的字符表示
     * 每个单元之间都用空格隔开
    */
    bool save(std::string name, std::vector<std::vector<std::string>> &content);
    bool load(std::string name, std::vector<std::vector<std::string>> &content, bool mandatory_access = false);
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
    bool travel_tree(treeNode *p, std::string &tree_info);
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
    bool list_directory_contents(std::vector<std::string> &content);
    bool make_file(std::string name);
    bool make_dir(std::string name);
    bool change_directory(std::string name);
    bool remove_file(std::string name);
    bool remove_dir(std::string name);
    bool update_name(std::string fr_name, std::string to_name);
    bool update_content(std::string name, std::string content);
    bool get_content(std::string name, std::string &content);
    bool tree(std::string &tree_info);
};

void print(FileSystem &fs) {
    std::vector<std::string> content;
    fs.list_directory_contents(content);
    for (auto &it : content) {
        std::cout << it << ' ';
    }
    std::cout << '\n';
}

void do_cmd(FileSystem &fs, int op, std::string nm="-1") {
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    Logger &logger = Logger::get_logger();
    if (op == 0) {
        if (!fs.make_file(nm)) {
            std::cout << logger.information << '\n';
        }
    } else if (op == 1) {
        if (!fs.make_dir(nm)) {
            std::cout << logger.information << '\n';
        }
    } else if (op == 2) {
        if (!fs.change_directory(nm)) {
            std::cout << logger.information << '\n';
        }
    } else {
        if (!fs.goto_last_dir()) {
            std::cout << logger.information << '\n';
        }
    }
}

void test() {
    Logger logger = Logger::get_logger();
    FileSystem fs;
    if (!fs.remove_dir("3")) {
        std::cout << *logger.information << '\n';
    }
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    std::vector<int> op({1, 0, 1, 2, 1, 3, 2, 0, 0, 3, 1, 2, 0, 3, 2, 2, 0, 3, 3, 3});
    std::vector<std::string> nm({"1", "2", "3", "1", "5", "-1", "3", "6", "7", "-1", "8", "8", "4", "-1", "1", "5", "9", "-1", "-1", "-1"});
    for (int i = 0; i < op.size(); i++) {
        do_cmd(fs, op[i], nm[i]);
    }
    std::string tree_info;
    fs.tree(tree_info);
    std::cout << tree_info;
    fs.commit_version(0);
    fs.remove_dir("3");
    fs.tree(tree_info);
    std::cout << tree_info;
    fs.switch_version(0);
    fs.tree(tree_info);
    std::cout << tree_info;


    // fs.update_content("2", "123123123");
    // fs.commit_version(0);
    // fs.update_content("2", "312312312312");
    // fs.switch_version(0);
    // fs.commit_version(0);
    // fs.remove_file("2");
    // fs.switch_version(0);
    // fs.remove_dir("3");
    // std::string tree_info;
    // fs.tree(tree_info);
    // std::cout << tree_info;
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

bool FileSystem::travel_tree(treeNode *p,std::string &tree_info) {
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
    std::vector<std::string> dir_content;
    if (!list_directory_contents(dir_content)) return false;
    for (auto &nm : dir_content) {
        if (nm == name) return true;
    }
    return false;
}

bool FileSystem::go_to(std::string name) {
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

bool FileSystem::list_directory_contents(std::vector<std::string> &content) {
    if (!goto_head()) return false;
    if (!check_path()) return false;
    while (path.back()->next_brother != nullptr) {
        content.push_back(node_manager.get_name(path.back()->next_brother->link));
        path.push_back(path.back()->next_brother);
    }
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



                        /* ======= class Logger ======= */

std::string Logger::get_time() {
    static char t[100];
    time_t timep;
    time(&timep);
    struct tm* p = gmtime(&timep);
    sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    return std::string(t);
}

Logger::Logger() {
    information = new std::string();
}

Logger& Logger::get_logger() {
    static Logger logger;
    return logger;
}

void Logger::log(std::string content, LOG_LEVEL level, int line) {
    static std::ofstream out(log_file, std::ios_base::app);
    *information = std::string(' ' + content);
    std::string app_tm = "(" + get_time() + ")" + *information;
    if (level == INFO) {
        out << "level: INFO " << '\n' << app_tm << std::endl;
    } else if (level == DEBUG) {
        out << "level: DEBUG " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
        std::cerr << "line: " << line << ' ' << app_tm << std::endl;
    } else if (level == WARNING) {
        out << "level: WARNING " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
    } else {
        out << "level: FATAL " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
        std::cerr << "level: FATAL " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
    }
}





                        /* ======= class Encryptor ======= */
Encryptor::Complex::Complex() = default;
Encryptor::Complex::Complex(double a, double b) : a(a), b(b) {}
Encryptor::Complex Encryptor::Complex::operator+(const Complex &r) const {
    return Complex(a + r.a, b + r.b);
}
Encryptor::Complex Encryptor::Complex::operator-(const Complex &r) const {
    return Complex(a - r.a, b - r.b);
}
Encryptor::Complex Encryptor::Complex::operator*(const Complex &r) const {
    return Complex(a * r.a - b * r.b, a * r.b + b * r.a);
}

    void Encryptor::fft(Complex a[], int n, int type) {
        const static double Pi = acos(-1.0);
        if (n == 1) return;
        int m = n >> 1;
        for (int i = 0; i < m; i++) {
            buf[i] = a[i << 1];
            buf[i + m] = a[i << 1 | 1];
        }
        memcpy(a, buf, sizeof(Complex) * n);
        Complex *a1 = a, *a2 = a + m;
        fft(a, m, type);
        fft(a + m, m, type);
        Complex wn = Complex(1, 0), u = Complex(cos(2 * Pi / n), type * sin(2 *Pi / n));
        for (int i = 0; i < m; i++) {
            Complex t = wn * a[m + i];
            wn = wn * u;
            buf[i] = a[i] + t;
            buf[i + m] = a[i] - t;
        }
        memcpy(a, buf, sizeof(Complex) * n);
    }

    bool Encryptor::encrypt_block(std::vector<std::pair<double, double>> &res) {
        fft(block, N, 1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back(std::make_pair(block[i].a, block[i].b));
        }
        return true;
    }

    bool Encryptor::decrypt_block(std::vector<int> &res) {
        fft(block, N, -1);
        res.clear();
        for (int i = 0; i < N; i++) {
            res.push_back((int)(block[i].a / N + 0.5));
            if (block[i].a < 0.0 && std::abs(block[i].a) > 1e-2) res.back()--;
        }
        return true;
    }

    bool Encryptor::encrypt_sequence(std::vector<int> &sequence, std::vector<std::pair<double, double>> &res) {
        int len = sequence.size(), idx = 1;
        while (sequence.size() % N != 0) sequence.push_back(PLACEHOLDER);
        memset(block, 0, sizeof block);
        block[0].a = len;
        res.clear();
        std::vector<std::pair<double, double>> tmp;
        for (auto &it : sequence) {
            block[idx++].a = it;
            if (idx == N) {
                encrypt_block(tmp);
                res.insert(res.end(), tmp.begin(), tmp.end());
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        return true;
    }

    bool Encryptor::decrypt_sequence(std::vector<std::pair<double, double>> &sequence, std::vector<int> &res) {
        if (sequence.size() % N != 0) return false;
        memset(block, 0, sizeof block);
        int idx = 0;
        res.clear();
        int len = -1;
        std::vector<int> tmp;
        for (auto &it : sequence) {
            block[idx].a = it.first;
            block[idx++].b = it.second;
            if (idx == N) {
                decrypt_block(tmp);
                if (len != -1) {
                    res.insert(res.end(), tmp.begin(), tmp.end());
                } else {
                    len = tmp.front();
                    res.insert(res.end(), tmp.begin() + 1, tmp.end());
                }
                idx = 0;
                memset(block, 0, sizeof block);
                tmp.clear();
            }
        }
        res.erase(res.begin() + len, res.end());
        return true;
    }






                        /* ======= class Saver ======= */
Saver::dataNode::dataNode() = default;
Saver::dataNode::dataNode(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> &data) : name_hash(name_hash), data_hash(data_hash), len(data.size() / N), data(data) {}

template <class T>
unsigned long long Saver::get_hash(T &s) {
    unsigned long long seed = 13331, hash = 0;
    for (auto &ch : s) {
        hash = hash * seed + ch;
    }
    return hash;
}

bool Saver::load_file() {
    std::ifstream in(data_file);
    if (!in.good()) {
        logger.log("load_file: No data file.", Logger::WARNING, __LINE__);
        return false;
    }
    mp.clear();
    unsigned long long name_hash, data_hash, len;
    std::vector<std::pair<double, double>> data;
    while (in >> name_hash) {
        data.clear();
        in >> data_hash >> len;
        if (in.eof()) {
            mp.clear();
            logger.log("Read interrupted, please check data integrity.", Logger::WARNING, __LINE__);
            return false;
        }
        for (int i = 0; i < len * N; i++) {
            double a, b;
            in >> a >> b;
            data.push_back(std::make_pair(a, b));
        }
        save_data(name_hash, data_hash, data);
    }
    return true;
}

/**
 * 在map中的存储方式
 * 以name_hash作为主键，会检索出一个dataNode，里面包括了name_hash, data_hash, len, data.
 * 其中 len为data的pair的对数 / N
*/
void Saver::save_data(unsigned long long name_hash, unsigned long long data_hash, std::vector<std::pair<double, double>> data) {
    if (mp.count(name_hash)) {
        mp.erase(mp.find(name_hash));
    }
    mp[name_hash] = dataNode(name_hash, data_hash, data);
}

Saver::Saver() {
    load_file();
}

/**
 * 保存的格式：
 * name_hash data_hash len 后面跟len对浮点数
*/
Saver::~Saver() {
    std::ofstream out(data_file);
    for (auto &data : mp) {
        dataNode &dn = data.second;
        out << data.first << ' ' << dn.data_hash << ' ' << dn.len;
        for (auto &pr : dn.data) {
            out << ' ' << pr.first << ' ' << pr.second;
        }
        out << '\n';
    }
}

/**
 * data字符串的格式:
 * 数据块的个数 
 * 每个数据块中数据的个数
 * 数据的长度
 * 数据的字符表示
 * 每个单元之间都用空格隔开
*/
bool Saver::save(std::string name, std::vector<std::vector<std::string>> &content) {
    std::stringstream ss;
    ss << content.size();
    for (auto &data_block : content) {
        ss << ' ' << data_block.size();
        for (auto &data : data_block) {
            ss << ' ' << data.size();
            for (auto &ch : data) {
                ss << ' ' << ch;
            }
        }
    }
    ss << '\n';
    std::string data;
    std::getline(ss, data);
    std::vector<int> sequence;
    for (auto &it : data) {
        sequence.push_back((int)it);
    }
    std::vector<std::pair<double, double>> res;
    encrypt_sequence(sequence, res);
    unsigned long long name_hash = get_hash(name);
    unsigned long long data_hash = get_hash(data);
    save_data(name_hash, data_hash, res);
    return true;
}

bool Saver::load(std::string name, std::vector<std::vector<std::string>> &content, bool mandatory_access) {
    unsigned long long name_hash = get_hash(name);
    if (!mp.count(name_hash)) {
        logger.log("Failed to load data. No data named A exists. ", Logger::WARNING, __LINE__);
        return false;
    }
    dataNode &data = mp[name_hash];
    std::vector<int> sequence;
    decrypt_sequence(data.data, sequence);
    if (get_hash(sequence) != data.data_hash) {
        logger.log("Data failed to pass integrity verification.", Logger::WARNING, __LINE__);
        if (!mandatory_access) return false;
    }
    std::string str;
    for (auto &it : sequence) {
        str.push_back(it);
    }
    std::stringstream ss(str);
    /**
     * data字符串的格式:
     * 数据块的个数 
     * 每个数据块中数据的个数
     * 数据的长度
     * 数据的字符表示
     * 每个单元之间都用空格隔开
    */
    content.clear();
    int block_num, data_num, data_len;
    std::string tmp;
    ss >> block_num;
    for (int i = 0; i < block_num; i++) {
        content.push_back(std::vector<std::string>());
        ss >> data_num;
        for (int j = 0; j < data_num; j++) {
            ss >> data_len;
            tmp.clear();
            for (int k = 0; k < data_len; k++) {
                char ch;
                ss >> ch;
                tmp.push_back(ch);
            }
            content.back().push_back(tmp);
        }
    }
    return true;
}