/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
private:
    std::string log_file = "log.chm";
    std::ofstream out;
    std::string get_time();

public:
    Logger();
    ~Logger();

    /**
     * Unless there is no other processing method, the User-oriented and 
     * developer-oriented functions in the program only returns true or false 
     * to indicate whether the requested operation is successfully executed.
     * 
     * If the execution is successful and the function has a return value, 
     * then the return value will be stored in this variable; if it is not 
     * successfully executed, the reason for not being successfully executed 
     * will be stored in this variable.
    */
    std::string information;

    /**
     * This function can be used in multiple places to obtain a logger.
     * Log management is implemented by the Logger class.
    */
    static Logger& get_logger();

    /**
    * An explanation of the four types of log registration:
    * INFO: Normal logs are used to record what operations the program has performed.
    * DEBUG: This level is mainly used when debugging programs.
    * WARNING: This level is used when the user does something wrong.
    * FATAL: This level records information that may cause the program to crash.
    */
    enum LOG_LEVEL {
        INFO = 0, DEBUG, WARNING, FATAL
    };

    /**
     * Use this function to record logs. 
    */
    void log(std::string content, LOG_LEVEL level = INFO, int line = __LINE__);
};

// Debug macros
#define LOG_DEBUG_1 Logger::get_logger().log("===== 11111 =====", Logger::DEBUG, __LINE__);
#define LOG_DEBUG_2 Logger::get_logger().log("===== 22222 =====", Logger::DEBUG, __LINE__);
#define LOG_WARN Logger::get_logger().log("===== WARN =====", Logger::DEBUG, __LINE__);

#endif // LOGGER_H
