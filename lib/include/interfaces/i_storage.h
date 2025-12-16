/**
 * @file i_storage.h
 * @brief Interface for persistent storage operations
 * 
 * Defines the abstract interface for saving and loading data,
 * enabling different storage backends and mock testing.
 */

#ifndef FFVMS_INTERFACES_I_STORAGE_H
#define FFVMS_INTERFACES_I_STORAGE_H

#include <string>
#include <vector>

namespace ffvms {

// Type alias for 2D string vector (used for structured data storage)
using DataTable = std::vector<std::vector<std::string>>;

/**
 * @brief Abstract interface for data persistence
 * 
 * Implementations can include encrypted file storage, database storage,
 * or mock storage for testing.
 */
class IStorage {
public:
    virtual ~IStorage() = default;

    /**
     * @brief Save data with a given name/key
     * @param name The identifier for the data
     * @param content The data to save (2D string table)
     * @return true if save succeeded, false otherwise
     */
    virtual bool save(const std::string& name, const DataTable& content) = 0;

    /**
     * @brief Load data by name/key
     * @param name The identifier for the data
     * @param content Output parameter for loaded data
     * @param mandatory_access If true, failure to load is an error
     * @return true if load succeeded, false otherwise
     */
    virtual bool load(const std::string& name, DataTable& content, 
                      bool mandatory_access = false) = 0;

    /**
     * @brief Check if a string contains only digits
     * @param s The string to check
     * @return true if all characters are digits
     */
    static bool is_all_digits(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }

    /**
     * @brief Convert string to unsigned long long
     * @param s The string to convert
     * @return The numeric value
     */
    static unsigned long long str_to_ull(const std::string& s) {
        unsigned long long result = 0;
        for (char c : s) {
            result = result * 10 + (c - '0');
        }
        return result;
    }
};

}  // namespace ffvms

#endif // FFVMS_INTERFACES_I_STORAGE_H
