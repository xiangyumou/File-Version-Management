#include "lib/file_system.cpp"
#include "lib/saver.cpp"

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

void print_tree(FileSystem &fs) {
    std::string tree_info;
    fs.tree(tree_info);
    std::cout << tree_info;
}

void test_file_system() {
    std::string content;
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
    fs.commit_version(0);
    print_tree(fs);
    fs.change_directory("1");
    fs.make_dir("1");
    fs.change_directory("5");
    fs.remove_file("9");
    fs.make_file("罗滨是神仙");
    fs.make_file("1");
    fs.make_file("123133");
    fs.make_file("124123");
    fs.update_content("1", "罗滨确实是神仙");
    fs.get_content("1", content);
    fs.commit_version(1);
    fs.change_directory("1");
    fs.remove_dir("5");
    print_tree(fs);
    fs.switch_version(1);
    print_tree(fs);
}

void test_saver() {
    Saver sa;
    vvs a;
    for (int i = 0; i < 5; i++) {
        a.push_back(std::vector<std::string>());
        for (int j = 0; j < 3; j++) {
            if (j % 2) a.back().push_back(std::to_string(i) + "罗滨是神仙");
            else a.back().push_back(std::to_string(i) + "mxy是神仙");
        }
    }
    sa.save("罗滨", a);
    // sa.load("罗滨", a);
    // for (int i = 0; i < a.size(); i++) {
    //     for (int j = 0; j < a[i].size(); j++) {
    //         std::cout << a[i][j] << '\n';
    //     }
    //     puts("");
    // }
}

int main() {
    // test_file_system();
    test_saver();
    
    return 0;
}