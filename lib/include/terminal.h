/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "file_system.h"
#include "command_interpreter.h"
#include "logger.h"
#include "saver.h"
#include <vector>
#include <string>

class Terminal : private CommandInterpreter {
private:
    FileSystem file_system;
    Logger& logger = Logger::get_logger();

    enum PARA_TYPE {
        STR = 0, INT, ULL
    };

    std::vector<std::vector<PARA_TYPE>> function_requirement;
    bool execute(unsigned long long pid, std::vector<std::string> parameter);
    bool initialize();

public:
    Terminal();
    int run();
};

#endif // TERMINAL_H
