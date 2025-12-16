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

#include "interfaces/i_logger.h"
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

/**
 * @brief Logger implementation with file and console output
 * 
 * Implements ILogger interface while maintaining backward compatibility
 * with existing code through the get_logger() singleton accessor.
 */
class Logger : public ffvms::ILogger {
private:
    std::string log_file = "log.chm";
    std::ofstream out;
    std::string get_time();
    std::string information_;
    std::string last_message_;

public:
    Logger();
    ~Logger() override;

    // Legacy public member for backward compatibility (deprecated)
    // New code should use get_information() instead
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
     * @deprecated Use the ILogger interface version with LogLevel instead
    */
    void log(std::string content, LOG_LEVEL level = INFO, int line = __LINE__);

    // ILogger interface implementation
    void log(const std::string& content, ffvms::LogLevel level, int line) override;
    const std::string& get_last_message() const override;
    void set_information(const std::string& info) override;
    const std::string& get_information() const override;
};

// Debug macros
#define LOG_DEBUG_1 Logger::get_logger().log("===== 11111 =====", Logger::DEBUG, __LINE__);
#define LOG_DEBUG_2 Logger::get_logger().log("===== 22222 =====", Logger::DEBUG, __LINE__);
#define LOG_WARN Logger::get_logger().log("===== WARN =====", Logger::DEBUG, __LINE__);

#endif // LOGGER_H
