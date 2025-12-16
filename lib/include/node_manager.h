/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "interfaces/i_node_manager.h"
#include "interfaces/i_file_manager.h"
#include "interfaces/i_storage.h"
#include "interfaces/i_logger.h"
#include <string>
#include <map>

// Forward declarations
class FileManager;
class Saver;
class Logger;

/**
 * @brief Node class representing file/folder metadata
 */
class Node {
private:
    ffvms::IFileManager* file_manager_ = nullptr;
    ffvms::IFileManager& get_file_manager_ref();
    
public:
    std::string name;
    std::string create_time;
    std::string update_time;
    unsigned long long fid;

    std::string get_time();
    Node();
    explicit Node(const std::string& name);
    Node(const std::string& name, ffvms::IFileManager* file_manager);
    void update_update_time();
};

/**
 * @brief NodeManager class for managing file/folder nodes
 * 
 * Implements INodeManager interface for node metadata management.
 */
class NodeManager : public ffvms::INodeManager {
private:
    std::map<unsigned long long, std::pair<unsigned long long, Node>> mp;
    std::string DATA_STORAGE_NAME = "NodeManager::map_relation";
    
    // Dependencies
    ffvms::IFileManager* file_manager_ = nullptr;
    ffvms::IStorage* storage_ = nullptr;
    ffvms::ILogger* logger_ = nullptr;
    
    // Helper to get dependencies
    ffvms::IFileManager& get_file_manager_ref();
    ffvms::IStorage& get_storage_ref();
    ffvms::ILogger& get_logger_ref();

    unsigned long long get_new_id();
    bool load();
    bool save();

public:
    /// Default constructor (uses global singletons)
    NodeManager();
    
    /// Constructor with dependency injection
    NodeManager(ffvms::IFileManager* file_manager, ffvms::IStorage* storage, ffvms::ILogger* logger);
    
    ~NodeManager() override;
    
    /// Get global singleton instance
    static NodeManager& get_node_manager();
    
    // INodeManager interface implementation
    bool node_exist(unsigned long long id) override;
    unsigned long long get_new_node(const std::string& name) override;
    void delete_node(unsigned long long idx) override;
    unsigned long long update_content(unsigned long long idx, const std::string& content) override;
    unsigned long long update_name(unsigned long long idx, const std::string& name) override;
    std::string get_content(unsigned long long idx) override;
    std::string get_name(unsigned long long idx) override;
    std::string get_update_time(unsigned long long idx) override;
    std::string get_create_time(unsigned long long idx) override;
    void increase_counter(unsigned long long idx) override;
    
    // Additional non-interface methods
    unsigned long long _get_counter(unsigned long long idx);
};

#endif // NODE_MANAGER_H
