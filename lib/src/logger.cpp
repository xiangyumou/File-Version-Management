/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "logger.h"
#include <sstream>
#include <iomanip>

std::string Logger::get_time() {
    auto now = std::time(nullptr);
    auto* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Logger::Logger() : out(log_file, std::ios_base::app) {
}

Logger::~Logger() {
    if (out.is_open()) {
        out.close();
    }
}

Logger& Logger::get_logger() {
    static Logger logger;
    return logger;
}

// Legacy log function for backward compatibility
void Logger::log(std::string content, LOG_LEVEL level, int line) {
    information = std::string(' ' + content);
    last_message_ = information;
    std::string app_tm = "(" + get_time() + ")" + information;
    
    if (level == INFO) {
        out << "level: INFO " << '\n' << app_tm << std::endl;
    } else if (level == DEBUG) {
        out << "level: DEBUG " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
        std::cerr << "line: " << line << ' ' << app_tm << std::endl;
    } else if (level == WARNING) {
        out << "level: WARNING " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
    } else {
        out << "level: FATAL " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
        std::cerr << "level: FATAL " << '\n' << "line: " << line << ' ' << app_tm << std::endl;
    }
}

// ILogger interface implementation
void Logger::log(const std::string& content, ffvms::LogLevel level, int line) {
    // Map ffvms::LogLevel to LOG_LEVEL for internal implementation
    LOG_LEVEL legacy_level;
    switch (level) {
        case ffvms::LogLevel::INFO: legacy_level = INFO; break;
        case ffvms::LogLevel::DEBUG: legacy_level = DEBUG; break;
        case ffvms::LogLevel::WARNING: legacy_level = WARNING; break;
        case ffvms::LogLevel::FATAL: legacy_level = FATAL; break;
        default: legacy_level = INFO;
    }
    log(std::string(content), legacy_level, line);
}

const std::string& Logger::get_last_message() const {
    return last_message_;
}

void Logger::set_information(const std::string& info) {
    information_ = info;
    information = info;  // Also set legacy member
}

const std::string& Logger::get_information() const {
    return information_;
}
