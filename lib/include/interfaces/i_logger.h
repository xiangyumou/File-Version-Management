/**
 * @file i_logger.h
 * @brief Interface for logging functionality
 * 
 * Defines the abstract interface for logging operations, enabling
 * dependency injection and mock testing.
 */

#ifndef FFVMS_INTERFACES_I_LOGGER_H
#define FFVMS_INTERFACES_I_LOGGER_H

#include "core/types.h"
#include <string>

namespace ffvms {

/**
 * @brief Abstract interface for logging
 * 
 * Implementations can include file logging, console logging, or
 * mock loggers for testing.
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Log a message with specified level
     * @param content The message content
     * @param level The log level (INFO, DEBUG, WARNING, FATAL)
     * @param line Source line number for debugging
     */
    virtual void log(const std::string& content, LogLevel level, int line) = 0;

    /**
     * @brief Get the last logged message
     * @return Reference to the last message string
     */
    virtual const std::string& get_last_message() const = 0;

    /**
     * @brief Set the information/result message
     * @param info The information to store
     */
    virtual void set_information(const std::string& info) = 0;

    /**
     * @brief Get the stored information/result
     * @return The stored information
     */
    virtual const std::string& get_information() const = 0;
};

}  // namespace ffvms

#endif // FFVMS_INTERFACES_I_LOGGER_H
