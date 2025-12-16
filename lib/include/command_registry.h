/**
 * @file command_registry.h
 * @brief Registry for command pattern - manages all available commands
 */

#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include "interfaces/i_command.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ffvms {

/**
 * @brief Registry for managing and looking up commands
 * 
 * Provides a central place to register commands and look them up by name.
 * Supports command aliases.
 */
class CommandRegistry {
public:
    CommandRegistry() = default;
    ~CommandRegistry() = default;

    // Non-copyable
    CommandRegistry(const CommandRegistry&) = delete;
    CommandRegistry& operator=(const CommandRegistry&) = delete;

    /**
     * @brief Register a command
     * @param cmd Command to register (takes ownership)
     */
    void register_command(std::unique_ptr<ICommand> cmd);

    /**
     * @brief Get a command by name or alias
     * @param name Command name or alias
     * @return Pointer to command, or nullptr if not found
     */
    ICommand* get_command(const std::string& name);

    /**
     * @brief Get all registered command names
     * @return Vector of primary command names
     */
    std::vector<std::string> get_all_command_names() const;

    /**
     * @brief Get help text for all commands
     * @return Formatted help string
     */
    std::string get_help() const;

    /**
     * @brief Check if a command exists
     * @param name Command name or alias
     * @return true if command exists
     */
    bool has_command(const std::string& name) const;

private:
    /// Map from command name to command instance
    std::map<std::string, std::unique_ptr<ICommand>> commands_;
    
    /// Map from alias to primary command name
    std::map<std::string, std::string> aliases_;
};

}  // namespace ffvms

#endif // COMMAND_REGISTRY_H
