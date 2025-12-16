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

#include "interfaces/i_logger.h"
#include <string>
#include <vector>

/**
 * @brief Handles reading and parsing of user input
 * 
 * Refactored to remove command mapping logic (now in CommandRegistry).
 * Focuses solely on parsing standard input into tokens, handling escapes.
 */
class CommandInterpreter {
private:
    ffvms::ILogger* logger_ = nullptr;
    
    // Dependencies helper
    ffvms::ILogger& get_logger_ref();

    std::string escape(char ch);
    std::vector<std::string> separator(std::string& s);

public:
    CommandInterpreter();
    explicit CommandInterpreter(ffvms::ILogger* logger);
    ~CommandInterpreter() = default;

    /**
     * @brief Parse input from stdin
     * @return Vector of strings, where index 0 is command name
     *         Returns empty vector if input is empty
     */
    std::vector<std::string> parse_input();
};

#endif // COMMAND_INTERPRETER_H
