/**
 * @file i_file_manager.h
 * @brief Interface for file content management
 * 
 * Defines the abstract interface for managing file content storage,
 * including reference counting for deduplication.
 */

#ifndef FFVMS_INTERFACES_I_FILE_MANAGER_H
#define FFVMS_INTERFACES_I_FILE_MANAGER_H

#include <string>

namespace ffvms {

/**
 * @brief Abstract interface for file content management
 * 
 * Manages file content with reference counting to support deduplication.
 * Implementations handle the actual storage mechanism.
 */
class IFileManager {
public:
    virtual ~IFileManager() = default;

    /**
     * @brief Create a new file with given content
     * @param content The file content
     * @return Unique file identifier
     */
    virtual unsigned long long create_file(const std::string& content) = 0;

    /**
     * @brief Get file content by ID
     * @param fid The file identifier
     * @param content Output parameter for content
     * @return true if successful, false if file not found
     */
    virtual bool get_content(unsigned long long fid, std::string& content) = 0;

    /**
     * @brief Increase the reference counter for a file
     * @param fid The file identifier
     * @return true if successful
     */
    virtual bool increase_counter(unsigned long long fid) = 0;

    /**
     * @brief Decrease the reference counter for a file
     * @param fid The file identifier
     * @return true if successful
     * @note File is deleted when counter reaches zero
     */
    virtual bool decrease_counter(unsigned long long fid) = 0;

    /**
     * @brief Update file content (creates new file, decreases old counter)
     * @param fid The current file identifier
     * @param new_id Output parameter for new file identifier
     * @param content The new content
     * @return true if successful
     */
    virtual bool update_content(unsigned long long fid, 
                                unsigned long long& new_id, 
                                const std::string& content) = 0;
};

}  // namespace ffvms

#endif // FFVMS_INTERFACES_I_FILE_MANAGER_H
