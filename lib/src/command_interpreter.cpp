/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "command_interpreter.h"
#include "logger.h"
#include <iostream>
#include <algorithm>

CommandInterpreter::CommandInterpreter() : logger_(nullptr) {}

CommandInterpreter::CommandInterpreter(ffvms::ILogger* logger) : logger_(logger) {}

ffvms::ILogger& CommandInterpreter::get_logger_ref() {
    if (logger_) return *logger_;
    return Logger::get_logger();
}

std::string CommandInterpreter::escape(char ch) {
    static std::vector<std::pair<char, std::string>> fr_to({
        {'s', " "},
        {'t', "\t"},
        {'\\', "\\"}
    });
    for (auto& it : fr_to) {
        if (it.first == ch) return it.second;
    }
    return "";
}

std::vector<std::string> CommandInterpreter::separator(std::string& s) {
    std::vector<std::string> res;
    std::string tmp;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] != ' ') tmp.push_back(s[i]);
        if (s[i] == ' ' || i == s.size() - 1) {
            if (!tmp.empty()) {
                res.push_back(tmp);
                tmp.clear();
            }
        }
    }
    return res;
}

std::vector<std::string> CommandInterpreter::parse_input() {
    std::string cmd;
    std::getline(std::cin, cmd);
    
    std::vector<std::string> separated_cmd = separator(cmd);
    std::vector<std::string> escaped_cmd(separated_cmd.size());
    
    for (size_t i = 0; i < separated_cmd.size(); i++) {
        std::string& current_cmd = separated_cmd[i];
        for (size_t j = 0; j < current_cmd.size(); j++) {
            if (current_cmd[j] != '\\') {
                escaped_cmd[i].push_back(current_cmd[j]);
            } else if (j == current_cmd.size() - 1) {
                break;
            } else {
                escaped_cmd[i] += escape(current_cmd[j + 1]);
                j++;
            }
        }
    }
    
    return escaped_cmd;
}
