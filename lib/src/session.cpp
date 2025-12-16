/**
 * @file session.cpp
 * @brief Implementation of Session class
 */

#include "session.h"
#include <sstream>

namespace ffvms {

Session::Session(FileSystem& fs) : file_system_(fs) {
    // Initialize with root or ask FS for current default
    // For now, let's assume valid start state or query FS
    file_system_.get_current_path(current_path_);
}

FileSystem& Session::get_file_system() {
    return file_system_;
}

std::vector<std::string> Session::get_current_path() const {
    return current_path_;
}

std::string Session::get_current_path_string() const {
    if (current_path_.empty()) return "/";
    std::ostringstream oss;
    for (const auto& p : current_path_) {
        oss << "/" << p;
    }
    return oss.str();
}

bool Session::set_current_path(const std::vector<std::string>& path) {
    // Save current as previous
    previous_path_ = current_path_;
    current_path_ = path;
    return true;
}

std::vector<std::string> Session::get_previous_path() const {
    return previous_path_;
}

} // namespace ffvms
