/**
 * @file platform.h
 * @brief Platform abstraction layer for OS-specific operations
 * 
 * Provides a unified interface for operations that differ between
 * Windows, macOS, and Linux.
 */

#ifndef FFVMS_CORE_PLATFORM_H
#define FFVMS_CORE_PLATFORM_H

#include <string>
#include <cstdlib>

namespace ffvms::platform {

/**
 * @brief Clear the terminal screen
 */
inline void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

/**
 * @brief Get the default text editor command for the current platform
 * @return The editor command ("notepad" on Windows, "vim" on Unix)
 */
inline std::string get_editor_command() {
#ifdef _WIN32
    return "notepad";
#else
    return "vim";
#endif
}

/**
 * @brief Get the path separator for the current platform
 * @return The path separator character
 */
inline char path_separator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

/**
 * @brief Check if the current platform is Windows
 */
inline constexpr bool is_windows() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

}  // namespace ffvms::platform

#endif // FFVMS_CORE_PLATFORM_H
