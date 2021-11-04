#include "lib/file_system.cpp"

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