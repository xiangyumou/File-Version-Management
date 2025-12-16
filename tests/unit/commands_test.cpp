#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_logger.h"
#include "mock_node_manager.h"
#include "mock_storage.h"
#include "file_system.h"
#include "commands/touch_command.h"
#include "commands/mkdir_command.h"
#include "commands/cd_command.h"
#include <memory>
#include <vector>

using namespace ffvms;
using namespace ffvms::test;
using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;

class CommandsTest : public ::testing::Test {
protected:
    NiceMock<MockLogger> mock_logger;
    NiceMock<MockNodeManager> mock_node_manager;
    NiceMock<MockStorage> mock_storage;
    std::unique_ptr<FileSystem> fs;
    
    void SetUp() override {
        // Setup initial storage state: empty/fail load to trigger fresh initialization
        EXPECT_CALL(mock_storage, load(_, _, _)).WillRepeatedly(Return(false));
        
        // Initialization triggers creation of "root"
        EXPECT_CALL(mock_node_manager, get_new_node("root")).WillRepeatedly(Return(1));
        EXPECT_CALL(mock_node_manager, get_name(_)).WillRepeatedly(Return("root")); // Catch-all for logging
        EXPECT_CALL(mock_node_manager, increase_counter(_)).Times(testing::AnyNumber());

        fs = std::make_unique<FileSystem>(&mock_logger, &mock_node_manager, &mock_storage);
    }
};

TEST_F(CommandsTest, TouchCommandCreatesFile) {
    TouchCommand cmd;
    std::vector<std::string> params = {"new_file.txt"};
    
    EXPECT_CALL(mock_node_manager, get_new_node("new_file.txt")).WillOnce(Return(2));
    EXPECT_CALL(mock_node_manager, get_name(2)).WillRepeatedly(Return("new_file.txt")); // For verification if needed

    CommandResult result = cmd.execute(*fs, params);
    
    EXPECT_TRUE(result.success);
    // Verify file created?
    // Indirect verification via get_new_node call.
}

TEST_F(CommandsTest, MkdirCommandCreatesDirectory) {
    MkdirCommand cmd;
    std::vector<std::string> params = {"new_dir"};
    
    EXPECT_CALL(mock_node_manager, get_new_node("new_dir")).WillOnce(Return(3));
    
    CommandResult result = cmd.execute(*fs, params);
    
    EXPECT_TRUE(result.success);
}

TEST_F(CommandsTest, CdCommandChangesDirectory) {
    // Navigate to existing directory?
    // Need to create one first or mock existence.
    // Creating one is safer integration test.
    
    // 1. Mkdir
    MkdirCommand mkdir_cmd;
    EXPECT_CALL(mock_node_manager, get_new_node("dir1")).WillOnce(Return(4));
    EXPECT_CALL(mock_node_manager, get_name(4)).WillRepeatedly(Return("dir1"));
    mkdir_cmd.execute(*fs, {"dir1"});
    
    // 2. Cd
    CdCommand cd_cmd;
    CommandResult result = cd_cmd.execute(*fs, {"dir1"});
    
    EXPECT_TRUE(result.success);
}
