#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#define wa std::cerr << "----WARN----" << '\n';
#define ll std::cerr << "----1111----" << '\n';
#define rr std::cerr << "----2222----" << '\n';

struct treeNode {
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
    void travel_tree(treeNode *p);
    void decrease_counter(treeNode *p);
    void init_version(treeNode *p, treeNode *vp);
    void recursive_modify_counter(treeNode *p, int diff, bool modify_brother=false);
    void recursive_delete_nodes(treeNode *p, bool delete_brother=false);
    void delete_node();
    bool is_son();
    void rebuild_nodes(treeNode *p);
    void goto_tail();
    void goto_head();
    bool go_to(std::string name);
    bool name_exist(std::string name);
public:
    FileSystem();
    void switch_version(int version_id);
    void commit_version(int model_version=-1);

    std::vector<std::string> list_directory_contents();
    void make_file(std::string name);
    void make_dir(std::string name);
    void remove_file(std::string name);
    void remove_dir(std::string name);

    void change_directory(std::string name);
    void goto_last_dir();
};

#endif