/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include "saver.h"
#include "logger.h"
#include <string>
#include <vector>
#include <map>
#include <utility>

// Constants
constexpr unsigned long long NO_COMMAND = 0x3f3f3f3fULL;

class CommandInterpreter {
    std::map<unsigned long long, unsigned long long> mp;
    Saver& saver = Saver::get_saver();
    Logger& logger = Logger::get_logger();
    std::string DATA_STORAGE_NAME = "CommandInterpreter::map_relation";

    static unsigned long long get_hash(std::string s);
    bool identifier_exist(unsigned long long iid);
    std::string escape(char ch);
    std::vector<std::string> separator(std::string& s);
    bool load();
    bool save();

public:
    CommandInterpreter();
    ~CommandInterpreter();
    bool FIRST_START = false;
    bool add_identifier(std::string identifier, unsigned long long pid);
    bool delete_identifier(std::string identifier);
    std::pair<unsigned long long, std::vector<std::string>> get_command();
    bool clear_data();
};

#endif // COMMAND_INTERPRETER_H
