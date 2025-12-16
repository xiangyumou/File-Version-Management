/**
 * @file i_command.h
 * @brief Interface for command pattern implementation
 * 
 * Defines the abstract interface for terminal commands, enabling
 * better testability and extensibility.
 */

#ifndef FFVMS_INTERFACES_I_COMMAND_H
#define FFVMS_INTERFACES_I_COMMAND_H

#include <string>
#include <vector>
#include <memory>

// Forward declaration
class FileSystem;

namespace ffvms {

/**
 * @brief Parameter types for command validation
 */
enum class ParamType {
    STRING,  ///< Any string parameter
    INT,     ///< Integer parameter (max 9 digits)
    ULL      ///< Unsigned long long (max 18 digits)
};

/**
 * @brief Result of command execution
 */
struct CommandResult {
    bool success;          ///< Whether command succeeded
    std::string message;   ///< Error or status message
    std::string output;    ///< Command output to display

    /// Create success result
    static CommandResult Ok(const std::string& output = "") {
        return {true, "", output};
    }

    /// Create error result
    static CommandResult Error(const std::string& message) {
        return {false, message, ""};
    }
};

/**
 * @brief Abstract interface for all commands
 * 
 * Each command class implements this interface to provide:
 * - Parameter validation requirements
 * - Execution logic
 * - Help information
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    /**
     * @brief Execute the command
     * @param fs Reference to FileSystem for operations
     * @param params Command parameters
     * @return CommandResult with success/failure and output
     */
    virtual CommandResult execute(FileSystem& fs, 
                                  const std::vector<std::string>& params) = 0;

    /**
     * @brief Get parameter type requirements
     * @return Vector of required parameter types
     */
    virtual std::vector<ParamType> get_param_requirements() const = 0;

    /**
     * @brief Get command name
     * @return Primary name of the command
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get help text for this command
     * @return Description of command usage
     */
    virtual std::string get_help() const = 0;

    /**
     * @brief Get command aliases (alternative names)
     * @return Vector of alias names (empty if no aliases)
     */
    virtual std::vector<std::string> get_aliases() const {
        return {};
    }
};

/**
 * @brief Base class with common command utilities
 */
class CommandBase : public ICommand {
protected:
    /**
     * @brief Check if string contains only digits
     */
    static bool is_all_digits(const std::string& s);

    /**
     * @brief Convert string to unsigned long long
     */
    static unsigned long long str_to_ull(const std::string& s);

    /**
     * @brief Validate parameters against requirements
     * @param params Parameters to validate
     * @param requirements Expected parameter types
     * @return Empty string if valid, error message otherwise
     */
    std::string validate_params(const std::vector<std::string>& params,
                                const std::vector<ParamType>& requirements) const;
};

}  // namespace ffvms

#endif // FFVMS_INTERFACES_I_COMMAND_H
