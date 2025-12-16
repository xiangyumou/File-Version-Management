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

void Logger::log(std::string content, LOG_LEVEL level, int line) {
    information = std::string(' ' + content);
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
