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

    std::string get_time() {
        static char t[100];
        time_t timep;
        time(&timep);
        struct tm* p = gmtime(&timep);
        sprintf(t, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        return std::string(t);
    }

    ~Logger() {
        std::ofstream out(log_file, std::ios_base::app);
        for (auto &it : logs) {
            out << it << '\n';
        }
    }
public:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    static Logger& get_logger() {
        static Logger logger;
        return logger;
    }
    void log(std::string content) {
        logs.push_back("(" + get_time() + ") " + content);
    }
};

class treeNode {
public:
    std::string name, content;
    int type;   // 0: 文件  1: 文件夹   2: 头节点
    int cnt;
    treeNode *next_brother, *first_son;

    treeNode() = default;
    treeNode(std::string name, int type, int cnt=1) {
        this->name = name, this->type = type;
        this->content = "";
        this->cnt = cnt;
        this->next_brother = nullptr;
        if (type == 0) this->first_son = nullptr;
        else if (type == 1) this->first_son = new treeNode("head node", 2, cnt);
    }
};

class FileSystem {
private:
    std::vector<treeNode*> path, version;

    void decrease_counter(treeNode *p) {
        if (--p->cnt == 0) delete p;
    }

    void recursive_delete_nodes(treeNode *p, bool delete_brother=false) {
        if (p == nullptr) return;
        recursive_delete_nodes(p->first_son, true);
        if (delete_brother) recursive_delete_nodes(p->next_brother, true);
        decrease_counter(p);
    }

    void delete_node() {
        treeNode *t = path.back();
        path.pop_back();
        rebuild_nodes(t->next_brother);
        decrease_counter(t);
    }

    void recursive_modify_counter(treeNode *p, int diff, bool modify_brother=false) {
        if (p == nullptr) return;
        recursive_modify_counter(p->first_son, diff, true);
        if (modify_brother) recursive_modify_counter(p->next_brother, diff, true);
        p->cnt += diff;
    }

    bool is_son() {
        return path.back()->type == 2;
    }

    void rebuild_nodes(treeNode *p) {
        // std::cout << p->name << ' ';
        int relation = 0;
        std::stack<treeNode*> stk;
        stk.push(p);
        for (; path.back()->cnt > 1; path.pop_back()) {
            treeNode *t = new treeNode();
            (*t) = (*path.back());
            if (relation == 1) t->first_son = stk.top();
            else t->next_brother = stk.top();
            relation = is_son();
            stk.push(t);
            decrease_counter(path.back());
        }
        // std::cout << path.back()->cnt << ' ';
        (relation ? path.back()->first_son : path.back()->next_brother) = stk.top();
        for (; !stk.empty(); stk.pop()) {
            path.push_back(stk.top());
        }
        if (path.back() == nullptr) path.pop_back();
        // std::cout << '\n';
    }

    void init_version(treeNode *p, treeNode *vp) {
        if (vp == nullptr) vp = p;
        p->first_son = vp->first_son;
        // std::cout << p->name << ' ' << vp->name << '\n';
        // std::cout << p->cnt << ' ' << vp->cnt << '\n';
        // std::cout << p->first_son->cnt << '\n';
        recursive_modify_counter(p, 1);
        // std::cout << p->first_son->cnt << '\n';
        path.push_back(p);
        path.push_back(p->first_son);
    }

    void goto_tail() {
        while (path.back()->next_brother != nullptr) {
            path.push_back(path.back()->next_brother);
        }
    }

    void goto_head() {
        while (!is_son()) {
            // std::cout << path.size() << ' ' << path.back()->type << '\n';
            path.pop_back();
        }
    }

    bool name_exist(std::string name) {
        auto dir_content = list_directory_contents();
        for (auto &nm : dir_content) {
            if (nm == name) return true;
        }
        return false;
    }

    bool go_to(std::string name) {
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

public:
    FileSystem() {
        commit_version();
    }

    void goto_last_dir() {
        goto_head();
        if (path.size() > 2) {
            path.pop_back();
        }
    }

    void switch_version(int version_id) {
        path.clear();
        path.push_back(version[version_id]);
        path.push_back(path.back()->first_son);
    }

    void commit_version(int model_version=-1) {
        version.push_back(new treeNode("root dir" + std::to_string(version.size()), 1, 0));
        // 内存泄漏！！！！path->first_son
        // std::cout << version.back()->type << ' ' << version.back()->first_son->type << '\n';
        if (model_version != -1) delete version.back()->first_son;
        treeNode *model = model_version == -1 ? nullptr : version[model_version];
        init_version(version.back(), model);
    }

    std::vector<std::string> list_directory_contents() {
        std::vector<std::string> content;
        goto_head();
        while (path.back()->next_brother != nullptr) {
            content.push_back(path.back()->next_brother->name);
            path.push_back(path.back()->next_brother);
        }
        return content;
    }

    void make_file(std::string name) {
        if (name_exist(name)) {
            std::cerr << name << ": name exist" << '\n';
        }
        goto_tail();
        treeNode *t = new treeNode(name, 0);
        // std::cout << path.front()->name << ' ';
        // std::cout << name << ' ' << path.back()->cnt << '\n';
        rebuild_nodes(t);
    }

    void make_dir(std::string name) {
        if (name_exist(name)) {
            std::cerr << name << ": name exist" << '\n';
        }
        goto_tail();
        treeNode *t = new treeNode(name, 1);
        rebuild_nodes(t);
    }

    void change_directory(std::string name) {
        go_to(name);
        if (path.back()->type != 1) {
            std::cerr << name << ": not a directory" << '\n';
            return;
        }
        path.push_back(path.back()->first_son);
    }

    void remove_file(std::string name) {
        if (!go_to(name)) return;
        if (path.back()->type != 1) {
            std::cerr << name << ": not a file" << '\n';
        }
        delete_node();
    }

    void remove_dir(std::string name) {
        if (!go_to(name)) return;
        if (path.back()->type != 1) {
            std::cerr << name << ": not a directory" << '\n';
            return;
        }
        treeNode *t = path.back();
        path.pop_back();
        rebuild_nodes(t->next_brother);
        recursive_delete_nodes(path.back());
    }
};

void test() {
    FileSystem fs;
    // for (int i = 0; i < 10; i++) {
    //     if (i & 1) fs.make_file(std::to_string(i));
    //     else fs.make_dir(std::to_string(i));
    // }
    // auto content = fs.list_directory_contents();
    // std::cout << content.size() << '\n';
    // for (auto it : content) std::cout << it << ' ';
    // std::cout << '\n';

    // fs.remove_file("0");
    // fs.remove_file("3");
    // fs.remove_file("6");
    // fs.remove_file("9");
    // fs.remove_dir("3");
    // fs.remove_dir("0");
    // fs.remove_dir("3");
    // fs.remove_dir("5");
    // return 0;
    // for (int i = 0; i < 10; i++) {
    //     if (i % 3 == 0) {
    //         fs.remove_file(std::to_string(i));
    //     }
    // }

    // content = fs.list_directory_contents();
    // // auto content = fs.list_directory_contents();
    // for (auto it : content) std::cout << it << ' ';
}

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

void test1() {
    FileSystem fs;
    // 0: make_file  1: make_dir  2: cd   3: cd ..
    std::vector<int> op({1, 0, 1, 2, 0, 1,  3, 2, 0, 0,  3, 0, 2, 2, 0});
    std::vector<int> nm({1, 2, 3, 1, 4, 5, -1, 3, 6, 7, -1, 8, 1, 5, 9});
    for (int i = 0; i < op.size(); i++) {
        do_cmd(fs, op[i], nm[i]);
    }
    print(fs);
    fs.goto_last_dir();
    fs.goto_last_dir();
    print(fs);
    fs.commit_version(0);
    fs.make_file("10");
    fs.switch_version(0);
    print(fs);
    fs.remove_dir("3");
    print(fs);
    fs.switch_version(1);
    print(fs);
}

int main() {
    // test();
    test1();
    return 0;
}