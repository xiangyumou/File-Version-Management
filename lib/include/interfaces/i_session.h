/**
 * @file i_session.h
 * @brief Interface for user session management
 */

#ifndef FFVMS_INTERFACES_I_SESSION_H
#define FFVMS_INTERFACES_I_SESSION_H

#include <string>
#include <vector>

// Forward declaration
class FileSystem;

namespace ffvms {

/**
 * @brief Interface for user session
 * 
 * Manages the user's context, including:
 * - Current Working Directory (CWD)
 * - Previous Working Directory (OLDPWD) for 'cd -'
 * - Access to the FileSystem
 */
class ISession {
public:
    virtual ~ISession() = default;

    /**
     * @brief Get the associated FileSystem
     * @return Reference to the FileSystem
     */
    virtual FileSystem& get_file_system() = 0;

    /**
     * @brief Get the current working directory path
     * @return Vector of path components
     */
    virtual std::vector<std::string> get_current_path() const = 0;

    /**
     * @brief Get the current working directory as a string
     * @return Path string (e.g., "/home/user")
     */
    virtual std::string get_current_path_string() const = 0;

    /**
     * @brief Set the current working directory
     * 
     * This should validate that the path exists in the FileSystem.
     * 
     * @param path Vector of path components
     * @return true if successful, false otherwise
     */
    virtual bool set_current_path(const std::vector<std::string>& path) = 0;

    /**
     * @brief Get the previous working directory
     * @return Vector of path components, or empty if none
     */
    virtual std::vector<std::string> get_previous_path() const = 0;
};

} // namespace ffvms

#endif // FFVMS_INTERFACES_I_SESSION_H
