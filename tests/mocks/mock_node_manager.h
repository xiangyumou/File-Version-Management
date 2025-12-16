#ifndef MOCK_NODE_MANAGER_H
#define MOCK_NODE_MANAGER_H

#include "gmock/gmock.h"
#include "interfaces/i_node_manager.h"

namespace ffvms {
namespace test {

class MockNodeManager : public INodeManager {
public:
    MOCK_METHOD(bool, node_exist, (unsigned long long), (override));
    MOCK_METHOD(unsigned long long, get_new_node, (const std::string&), (override));
    MOCK_METHOD(void, delete_node, (unsigned long long), (override));
    MOCK_METHOD(unsigned long long, update_content, (unsigned long long, const std::string&), (override));
    MOCK_METHOD(unsigned long long, update_name, (unsigned long long, const std::string&), (override));
    MOCK_METHOD(std::string, get_content, (unsigned long long), (override));
    MOCK_METHOD(std::string, get_name, (unsigned long long), (override));
    MOCK_METHOD(std::string, get_update_time, (unsigned long long), (override));
    MOCK_METHOD(std::string, get_create_time, (unsigned long long), (override));
    MOCK_METHOD(void, increase_counter, (unsigned long long), (override));
};

} // namespace test
} // namespace ffvms

#endif // MOCK_NODE_MANAGER_H
