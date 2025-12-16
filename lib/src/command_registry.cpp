/**
 * @file command_registry.cpp
 * @brief Implementation of CommandRegistry
 */

#include "command_registry.h"
#include <sstream>

namespace ffvms {

void CommandRegistry::register_command(std::unique_ptr<ICommand> cmd) {
    if (!cmd) return;

    std::string name = cmd->get_name();
    
    // Register aliases
    for (const auto& alias : cmd->get_aliases()) {
        aliases_[alias] = name;
    }
    
    // Register command
    commands_[name] = std::move(cmd);
}

ICommand* CommandRegistry::get_command(const std::string& name) {
    // Direct lookup
    auto it = commands_.find(name);
    if (it != commands_.end()) {
        return it->second.get();
    }
    
    // Check aliases
    auto alias_it = aliases_.find(name);
    if (alias_it != aliases_.end()) {
        it = commands_.find(alias_it->second);
        if (it != commands_.end()) {
            return it->second.get();
        }
    }
    
    return nullptr;
}

std::vector<std::string> CommandRegistry::get_all_command_names() const {
    std::vector<std::string> names;
    names.reserve(commands_.size());
    for (const auto& pair : commands_) {
        names.push_back(pair.first);
    }
    return names;
}

std::string CommandRegistry::get_help() const {
    std::ostringstream oss;
    oss << "Available commands:\n";
    oss << "==================\n\n";
    
    for (const auto& pair : commands_) {
        oss << pair.first;
        
        // Show aliases
        auto aliases = pair.second->get_aliases();
        if (!aliases.empty()) {
            oss << " (aliases: ";
            for (size_t i = 0; i < aliases.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << aliases[i];
            }
            oss << ")";
        }
        oss << "\n";
        oss << "  " << pair.second->get_help() << "\n\n";
    }
    
    return oss.str();
}

bool CommandRegistry::has_command(const std::string& name) const {
    if (commands_.find(name) != commands_.end()) return true;
    if (aliases_.find(name) != aliases_.end()) return true;
    return false;
}

}  // namespace ffvms
