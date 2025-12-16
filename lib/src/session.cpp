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
    if (!file_system_.navigate_to_path(path)) {
        return false;
    }
    // Save current as previous
    previous_path_ = current_path_;
    current_path_ = path;
    return true;
}


bool Session::change_directory(const std::string& path) {
    if (path == "-") {
        if (previous_path_.empty()) return false;
        return set_current_path(previous_path_);
    }
    
    std::vector<std::string> input_vector;
    std::string current_segment;
    std::istringstream iss(path);
    while (std::getline(iss, current_segment, '/')) {
        if (!current_segment.empty()) {
            input_vector.push_back(current_segment);
        }
    }

    std::vector<std::string> target_path;
    if (!path.empty() && path.front() == '/') {
         for (const auto& part : input_vector) {
            if (part == ".") continue;
            if (part == "..") {
                if (!target_path.empty()) target_path.pop_back();
            } else {
                target_path.push_back(part);
            }
         }
    } else {
        target_path = resolve_path(input_vector);
    }
    
    return set_current_path(target_path);
}


std::vector<std::string> Session::get_previous_path() const {
    return previous_path_;
}

std::vector<std::string> Session::resolve_path(const std::vector<std::string>& input_path) const {
    std::vector<std::string> resolved;
    
    // If input path is absolute (implementation detail: how do we represent absolute? 
    // In this system, maybe we assume if it starts with special token? 
    // Actually, vector<string> usually means relative to root if it's the whole path.
    // Let's assume input_path is relative to CWD unless we define a convention.
    // But typically shell sends "params".
    // Let's defer "absolute vs relative" parsing to the command or helper.
    // Wait, the Session should probably handle the "logic" of applying a relative path to CWD.
    
    // For now, let's implement a helper that takes a path relative to CWD and returns the absolute canonical path.
    resolved = current_path_;

    for (const auto& part : input_path) {
        if (part == ".") {
            continue;
        } else if (part == "..") {
            if (!resolved.empty()) {
                resolved.pop_back();
            }
        } else {
            resolved.push_back(part);
        }
    }
    return resolved;
}


} // namespace ffvms
