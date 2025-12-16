/**
 * @file i_node_manager.h
 * @brief Interface for node (file/folder metadata) management
 * 
 * Defines the abstract interface for managing file and folder metadata
 * nodes, including names, timestamps, and content references.
 */

#ifndef FFVMS_INTERFACES_I_NODE_MANAGER_H
#define FFVMS_INTERFACES_I_NODE_MANAGER_H

#include <string>

namespace ffvms {

/**
 * @brief Abstract interface for node metadata management
 * 
 * Manages nodes that represent files and folders in the virtual
 * file system, including their names, timestamps, and content references.
 */
class INodeManager {
public:
    virtual ~INodeManager() = default;

    /**
     * @brief Check if a node exists
     * @param id The node identifier
     * @return true if node exists
     */
    virtual bool node_exist(unsigned long long id) = 0;

    /**
     * @brief Create a new node with given name
     * @param name The node name
     * @return Unique node identifier
     */
    virtual unsigned long long get_new_node(const std::string& name) = 0;

    /**
     * @brief Delete a node
     * @param idx The node identifier
     */
    virtual void delete_node(unsigned long long idx) = 0;

    /**
     * @brief Update the content of a node
     * @param idx The node identifier
     * @param content The new content
     * @return New node identifier (may change due to content deduplication)
     */
    virtual unsigned long long update_content(unsigned long long idx, 
                                              const std::string& content) = 0;

    /**
     * @brief Update the name of a node
     * @param idx The node identifier
     * @param name The new name
     * @return Node identifier (unchanged)
     */
    virtual unsigned long long update_name(unsigned long long idx, 
                                           const std::string& name) = 0;

    /**
     * @brief Get the content of a node
     * @param idx The node identifier
     * @return The content string
     */
    virtual std::string get_content(unsigned long long idx) = 0;

    /**
     * @brief Get the name of a node
     * @param idx The node identifier
     * @return The name string
     */
    virtual std::string get_name(unsigned long long idx) = 0;

    /**
     * @brief Get the last update time of a node
     * @param idx The node identifier
     * @return Formatted timestamp string
     */
    virtual std::string get_update_time(unsigned long long idx) = 0;

    /**
     * @brief Get the creation time of a node
     * @param idx The node identifier
     * @return Formatted timestamp string
     */
    virtual std::string get_create_time(unsigned long long idx) = 0;

    /**
     * @brief Increase the reference counter for a node
     * @param idx The node identifier
     */
    virtual void increase_counter(unsigned long long idx) = 0;
};

}  // namespace ffvms

#endif // FFVMS_INTERFACES_I_NODE_MANAGER_H
