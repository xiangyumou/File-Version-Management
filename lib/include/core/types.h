/**
 * @file types.h
 * @brief Core type definitions and constants for FFVMS
 * 
 * This header replaces magic numbers with named constants and provides
 * common type definitions used throughout the project.
 */

#ifndef FFVMS_CORE_TYPES_H
#define FFVMS_CORE_TYPES_H

#include <cstdint>
#include <string>

namespace ffvms {

/// @brief Constants for version management
namespace version {
    /// Invalid version ID sentinel value
    constexpr unsigned long long INVALID_ID = 0x3f3f3f3fULL;
    
    /// No model version (create empty version)
    constexpr unsigned long long NO_MODEL = 0x3f3f3f3fULL;
}

/// @brief Constants for command interpretation
namespace command {
    /// Sentinel value indicating no valid command was parsed
    constexpr unsigned long long NO_COMMAND = 0x3f3f3f3fULL;
}

/// @brief Constants for encryption
namespace encryption {
    /// FFT block size (must be power of 2)
    constexpr int FFT_BLOCK_SIZE = 1 << 10;  // 1024
}

/// @brief Constants for node management  
namespace node {
    /// Null node sentinel value
    constexpr unsigned long long NULL_NODE = 0x3f3f3f3f3f3fULL;
}

/// @brief Log level enumeration
enum class LogLevel {
    INFO = 0,
    DEBUG,
    WARNING,
    FATAL
};

/// @brief Node type enumeration
enum class NodeType {
    FILE = 0,
    DIR,
    HEAD_NODE
};

}  // namespace ffvms

#endif // FFVMS_CORE_TYPES_H
