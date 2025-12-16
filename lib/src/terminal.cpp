/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "terminal.h"
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>

bool Terminal::execute(unsigned long long pid, std::vector<std::string> parameter) {
    if (pid >= function_requirement.size()) {
        logger.log("There is no program numbered " + std::to_string(pid) + ". Please check whether the configuration is correct.", Logger::WARNING, __LINE__);
        return false;
    }
    std::vector<PARA_TYPE>& fr = function_requirement[pid];
    if (parameter.size() < fr.size()) {
        logger.log("Parameters are insufficient. " + std::to_string(fr.size()) + " parameters were required but only " + std::to_string(parameter.size()) + " were provided.", Logger::WARNING, __LINE__);
        return false;
    }
    for (size_t i = 0; i < fr.size(); i++) {
        if (fr[i] == INT || fr[i] == ULL) {
            if (!Saver::is_all_digits(parameter[i])) {
                logger.log("The " + std::to_string(i) + "th parameter must be an integer. Check the input.", Logger::WARNING, __LINE__);
                return false;
            }
            if (fr[i] == INT && parameter[i].size() > 9) {
                logger.log("The " + std::to_string(i) + "th argument has a maximum of 9 digits. Check the input.", Logger::WARNING, __LINE__);
                return false;
            }
            if (fr[i] == ULL && parameter[i].size() > 18) {
                logger.log("The " + std::to_string(i) + "th argument has a maximum of 18 digits. Check the input.", Logger::WARNING, __LINE__);
                return false;
            }
        }
    }

    std::string get_content_content;
    std::string tree_content;
    std::vector<std::string> ls_content;
    std::vector<std::pair<unsigned long long, versionNode>> version_content;
    std::string file_name;
    std::string cmd;
    std::string content;
    std::string tmp;
    std::ifstream in;
    std::vector<std::string> path;
    std::vector<std::pair<std::string, std::vector<std::string>>> res;

    switch (pid) {
        case 0:
            if (!add_identifier(parameter[0], Saver::str_to_ull(parameter[1]))) return false;
            else std::cout << "An identifier was successfully added for program " << Saver::str_to_ull(parameter[1]) << ".\n";
            break;
        case 1:
            if (!delete_identifier(parameter[0])) return false;
            else std::cout << "An identifier was successfully deleted.\n";
            break;
        case 2:
            if (!file_system.switch_version(static_cast<int>(Saver::str_to_ull(parameter[0])))) return false;
            else std::cout << "Switched to version " << parameter[0] << '\n';
            break;
        case 3:
            if (!file_system.make_file(parameter[0])) return false;
            break;
        case 4:
            if (!file_system.make_dir(parameter[0])) return false;
            break;
        case 5:
            if (!file_system.change_directory(parameter[0])) return false;
            break;
        case 6:
            for (auto& fn : parameter) {
                if (!file_system.remove_file(fn)) {
                    std::cout << logger.information << '\n';
                }
            }
            break;
        case 7:
            for (auto& fn : parameter) {
                if (!file_system.remove_dir(fn)) {
                    std::cout << logger.information << '\n';
                }
            }
            break;
        case 8:
            if (!file_system.update_name(parameter[0], parameter[1])) return false;
            break;
        case 9:
            if (!file_system.update_content(parameter[0], parameter[1])) return false;
            break;
        case 10:
            if (!file_system.get_content(parameter[0], get_content_content)) return false;
            else std::cout << get_content_content << '\n';
            break;
        case 11:
            if (!file_system.tree(tree_content)) return false;
            std::cout << tree_content << '\n';
            break;
        case 12:
            if (!file_system.goto_last_dir()) return false;
            break;
        case 13:
            if (!file_system.list_directory_contents(ls_content)) return false;
            if (ls_content.empty()) {
                std::cout << "The folder is empty.  QAQ\n";
                break;
            }
            if (parameter.empty() || parameter[0] != "-a") {
                std::sort(ls_content.begin(), ls_content.end());
                for (size_t i = 0; i < ls_content.size(); i++) {
                    if (i != 0 && i % 8 == 0) std::cout << '\n';
                    std::cout << ls_content[i] << "\t";
                }
                std::cout << '\n';
            } else {
                std::cout << "type\t" << "create time\t\t" << "update time\t\t" << "name\n";
                for (auto& item : ls_content) {
                    treeNode::TYPE type;
                    std::string create_time, update_time;
                    if (!file_system.get_type(item, type)) return false;
                    if (!file_system.get_create_time(item, create_time)) return false;
                    if (!file_system.get_update_time(item, update_time)) return false;
                    std::cout << (type == treeNode::FILE ? "file" : "dir") << '\t' << create_time << '\t' << update_time << '\t' << item << '\n';
                }
            }
            break;
        case 14:
            if (parameter.empty()) {
                if (!file_system.create_version("")) return false;
            } else if (parameter.size() == 1) {
                if (Saver::is_all_digits(parameter[0])) {
                    if (!file_system.create_version(Saver::str_to_ull(parameter[0]))) return false;
                } else {
                    if (!file_system.create_version(parameter[0])) return false;
                }
            } else {
                if (Saver::is_all_digits(parameter[0])) {
                    if (!file_system.create_version(Saver::str_to_ull(parameter[0]), parameter[1])) return false;
                } else if (Saver::is_all_digits(parameter[1])) {
                    if (!file_system.create_version(parameter[0], Saver::str_to_ull(parameter[1]))) return false;
                } else {
                    logger.log("create_version requires at least one numeric parameter (version id).", Logger::WARNING, __LINE__);
                    return false;
                }
            }
            break;
        case 15:
            if (!file_system.version(version_content)) return false;
            std::cout << "version id\t" << "information\n";
            for (auto& it : version_content) {
                std::cout << it.first << "\t\t" << (it.second.info.empty() ? "NULL" : it.second.info) << '\n';
            }
            break;
        case 16:
            std::cout << "The current version of the file system is " << file_system.get_current_version() << '\n';
            break;
        case 17:
            if (!initialize()) return false;
            break;
        case 18:
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        case 19:
            file_name = "temp_file_" + parameter[0];
            {
                std::ofstream out(file_name);
                if (file_system.get_content(parameter[0], content)) {
                    out << content;
                }
                out.close();
            }
#ifdef _WIN32
            cmd = "notepad " + file_name;
#else
            cmd = "vim " + file_name;
#endif
            system(cmd.c_str());
            in.open(file_name);
            content.clear();
            while (std::getline(in, tmp)) {
                content += tmp;
                content.push_back('\n');
            }
            in.close();
            remove(file_name.c_str());
            file_system.make_file(parameter[0]);
            if (!file_system.update_content(parameter[0], content)) return false;
            break;
        case 20:
            if (!file_system.get_current_path(path)) return false;
            std::cout << '/';
            for (auto& p : path) {
                std::cout << p << '/';
            }
            std::cout << '\n';
            break;
        case 21:
            if (!file_system.Find(parameter[0], res)) return false;
            std::cout << "name\t" << "path\n";
            for (auto& r : res) {
                std::cout << r.first << '\t' << '/';
                for (auto& s : r.second) {
                    std::cout << s << '/';
                }
                std::cout << '\n';
            }
            break;
    }
    return true;
}

bool Terminal::initialize() {
    if (FIRST_START) {
        clear_data();
    }
    add_identifier("add_identifier", 0);
    add_identifier("delete_identifier", 1);
    add_identifier("switch_version", 2);
    add_identifier("touch", 3);
    add_identifier("mkdir", 4);
    add_identifier("cd", 5);
    add_identifier("rmf", 6);
    add_identifier("rmd", 7);
    add_identifier("update_name", 8);
    add_identifier("update_content", 9);
    add_identifier("cat", 10);
    add_identifier("tree", 11);
    add_identifier("cdl", 12);
    add_identifier("ls", 13);
    add_identifier("create_version", 14);
    add_identifier("version", 15);
    add_identifier("gcv", 16);
    add_identifier("init", 17);
    add_identifier("clear", 18);
    add_identifier("vim", 19);
    add_identifier("pwd", 20);
    add_identifier("find", 21);
    return true;
}

Terminal::Terminal() {
    // add_identifier
    function_requirement.push_back(std::vector<PARA_TYPE>({STR, ULL}));
    // delete_identifier
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // switch_version
    function_requirement.push_back(std::vector<PARA_TYPE>({ULL}));
    // make_file
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // make_dir
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // change_directory
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // remove_file
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // remove_dir
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // update_name
    function_requirement.push_back(std::vector<PARA_TYPE>({STR, STR}));
    // update_content
    function_requirement.push_back(std::vector<PARA_TYPE>({STR, STR}));
    // get_content
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // tree
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // goto_last_dir
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // list_directory_content
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // create_version
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // version
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // get_current_version
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // init
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // clear
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // vim
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));
    // pwd
    function_requirement.push_back(std::vector<PARA_TYPE>());
    // find
    function_requirement.push_back(std::vector<PARA_TYPE>({STR}));

    if (FIRST_START) {
        initialize();
    }
}

int Terminal::run() {
    std::pair<unsigned long long, std::vector<std::string>> cmd;
    while (true) {
        std::cout << "# ";
        cmd = get_command();
        if (cmd.first == NO_COMMAND) {
            if (!cmd.second.empty() && cmd.second.front() == "exit") {
                return 0;
            }
            if (!cmd.second.empty()) {
                std::cout << logger.information << '\n';
            }
        } else {
            if (!execute(cmd.first, cmd.second)) {
                std::cout << logger.information << '\n';
            }
        }
    }
    return 0;
}
