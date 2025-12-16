/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "command_interpreter.h"
#include <iostream>

unsigned long long CommandInterpreter::get_hash(std::string s) {
    unsigned long long hash = 0, seed = 13331;
    for (auto& ch : s) {
        hash = hash * seed + ch;
    }
    return hash;
}

bool CommandInterpreter::identifier_exist(unsigned long long iid) {
    return mp.count(iid);
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

bool CommandInterpreter::save() {
    vvs data;
    for (auto& it : mp) {
        data.push_back(std::vector<std::string>());
        data.back().push_back(std::to_string(it.first));
        data.back().push_back(std::to_string(it.second));
    }
    return saver.save(DATA_STORAGE_NAME, data);
}

bool CommandInterpreter::load() {
    vvs data;
    if (!saver.load(DATA_STORAGE_NAME, data)) return false;
    mp.clear();
    for (auto& pr : data) {
        if (pr.size() != 2) {
            logger.log("Command interpreter: The mapping should be a pair, and there are no pairs in the data. Please check whether the data is complete.", Logger::WARNING, __LINE__);
            return false;
        }
        unsigned long long indntifier_hash = 0, pid = 0;
        for (auto& ch : pr[0]) {
            indntifier_hash = indntifier_hash * 10 + ch - '0';
        }
        for (auto& ch : pr[1]) {
            pid = pid * 10 + ch - '0';
        }
        if (identifier_exist(indntifier_hash)) {
            logger.log("Command interpreter: There are multiple identifiers in the data, please check whether the data is correct.", Logger::WARNING, __LINE__);
            return false;
        }
        mp[indntifier_hash] = pid;
    }
    return true;
}

CommandInterpreter::CommandInterpreter() {
    if (!load()) FIRST_START = true;
}

CommandInterpreter::~CommandInterpreter() {
    save();
}

bool CommandInterpreter::add_identifier(std::string identifier, unsigned long long pid) {
    unsigned long long identifier_hash = get_hash(identifier);
    if (identifier_exist(identifier_hash)) {
        logger.log("Identifier " + identifier + " already exists. Please delete the original to add a new one.", Logger::WARNING, __LINE__);
        return false;
    }
    mp[identifier_hash] = pid;
    return true;
}

bool CommandInterpreter::delete_identifier(std::string identifier) {
    unsigned long long identifier_hash = get_hash(identifier);
    if (!identifier_exist(identifier_hash)) {
        logger.log("Identifier " + identifier + " does not exist.", Logger::WARNING, __LINE__);
        return false;
    }
    mp.erase(mp.find(identifier_hash));
    return true;
}

std::pair<unsigned long long, std::vector<std::string>> CommandInterpreter::get_command() {
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
    if (escaped_cmd.empty()) {
        return std::make_pair(NO_COMMAND, std::vector<std::string>());
    }
    unsigned long long identifier_hash = get_hash(escaped_cmd.front());
    if (!identifier_exist(identifier_hash)) {
        logger.log("Command not found: " + escaped_cmd.front(), Logger::WARNING, __LINE__);
        return std::make_pair(NO_COMMAND, escaped_cmd);
    } else {
        escaped_cmd.erase(escaped_cmd.begin());
        return std::make_pair(mp[identifier_hash], escaped_cmd);
    }
}

bool CommandInterpreter::clear_data() {
    mp.clear();
    return true;
}
