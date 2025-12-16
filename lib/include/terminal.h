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
#include "command_registry.h"
#include "interfaces/i_logger.h"
#include <string>
#include <vector>

class Terminal {
private:
    FileSystem file_system;
    CommandInterpreter interpreter_;
    ffvms::CommandRegistry registry_;
    ffvms::ILogger* logger_ = nullptr;

    ffvms::ILogger& get_logger_ref();
    void register_commands();

public:
    Terminal();
    int run();
};

#endif // TERMINAL_H
