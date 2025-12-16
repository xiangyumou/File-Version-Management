/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "interfaces/i_file_manager.h"
#include "interfaces/i_logger.h"
#include "interfaces/i_storage.h"
#include <string>
#include <map>

// Forward declaration
class Saver;
class Logger;

/**
 * @brief File node structure for content storage
 */
struct fileNode {
    std::string content;
    unsigned long long cnt;

    fileNode() = default;
    fileNode(std::string content);
};

/**
 * @brief FileManager class for managing file content with reference counting
 * 
 * Implements IFileManager interface for file content management.
 * Ensures files with same content are stored only once via reference counting.
 */
class FileManager : public ffvms::IFileManager {
private:
    std::string DATA_STORAGE_NAME = "FileManager::map_relation";
    std::map<unsigned long long, fileNode> mp;
    
    // Dependencies (can be injected or use singletons)
    ffvms::IStorage* storage_ = nullptr;
    ffvms::ILogger* logger_ = nullptr;
    
    // Helper to get dependencies
    ffvms::IStorage& get_storage_ref();
    ffvms::ILogger& get_logger_ref();

    unsigned long long get_new_id();
    bool file_exist(unsigned long long fid);
    bool check_file(unsigned long long fid);
    bool save();
    bool load();

public:
    /// Default constructor (uses global singletons)
    FileManager();
    
    /// Constructor with dependency injection
    FileManager(ffvms::IStorage* storage, ffvms::ILogger* logger);
    
    ~FileManager() override;
    
    /// Get global singleton instance
    static FileManager& get_file_manager();
    
    // IFileManager interface implementation
    unsigned long long create_file(const std::string& content) override;
    bool get_content(unsigned long long fid, std::string& content) override;
    bool increase_counter(unsigned long long fid) override;
    bool decrease_counter(unsigned long long fid) override;
    bool update_content(unsigned long long fid, unsigned long long& new_id, 
                        const std::string& content) override;
};

#endif // FILE_MANAGER_H
