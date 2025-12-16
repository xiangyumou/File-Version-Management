#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_logger.h"
#include "mock_node_manager.h"
#include "mock_storage.h"
#include "file_system.h"
#include <memory>

using namespace ffvms;
using namespace ffvms::test;
using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::DoAll;
using ::testing::SetArgReferee;

class FileSystemTest : public ::testing::Test {
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
        EXPECT_CALL(mock_node_manager, get_name(1)).WillRepeatedly(Return("root"));
        
        // Node 1 increases counter (init_version -> recursive_increase)
        // Also HEAD_NODE has link -1, so expect increase_counter(-1) too.
        EXPECT_CALL(mock_node_manager, increase_counter(_)).Times(testing::AnyNumber());
        EXPECT_CALL(mock_node_manager, get_name(_)).WillRepeatedly(Return("mock_node"));

        fs = std::make_unique<FileSystem>(&mock_logger, &mock_node_manager, &mock_storage);
    }
    
    // Helper to simulate switching to a version where "root" -> "dir1"
    // But testing make_dir is easier start.
};

TEST_F(FileSystemTest, InitializationCreatesVersion) {
    // Assert initialization logic implicitly verified by SetUp expectations
    EXPECT_EQ(fs->get_current_version(), 1001); // Default first version
}

TEST_F(FileSystemTest, MakeDirCreatesDirectory) {
    // fs uses real BSTree.
    // path requires HEAD_NODE structure?
    // FileSystem::make_dir:
    // 1. name_exist check.
    // 2. goto_tail.
    // 3. new treeNode(DIR).
    // 4. link = get_new_node(name).
    // 5. rebuild_nodes.
    
    // Expectations
    EXPECT_CALL(mock_node_manager, get_new_node("test_dir")).WillOnce(Return(2));
    EXPECT_CALL(mock_node_manager, increase_counter(2)).Times(testing::AnyNumber()); // Rebuild might increase?
    
    // NOTE: BSTree::name_exist relies on list_directory_contents which relies on goto_head.
    // Since we start fresh, path is [root, head] (created by switch_version).
    // Root has son Head.
    // Head next is null.
    // name_exist returns false.
    
    EXPECT_TRUE(fs->make_dir("test_dir"));
    
    // Verify it exists now?
    // mock_node_manager.get_name(2) needed for future checks
    EXPECT_CALL(mock_node_manager, get_name(2)).WillRepeatedly(Return("test_dir"));
    
    // Verify verify with tree dump or check?
    // fs->find?
    std::string content;
    fs->tree(content);
    // tree calls travel_tree -> get_name(1) (root) -> get_name(2) (test_dir).
}

TEST_F(FileSystemTest, MakeFileCreatesFile) {
    EXPECT_CALL(mock_node_manager, get_new_node("test_file.txt")).WillOnce(Return(3));
    EXPECT_CALL(mock_node_manager, increase_counter(3)).Times(testing::AnyNumber());
    
    EXPECT_TRUE(fs->make_file("test_file.txt"));
}

TEST_F(FileSystemTest, RemoveFileDeletesFile) {
    // Create file first
    EXPECT_CALL(mock_node_manager, get_new_node("file_to_del")).WillOnce(Return(4));
    EXPECT_CALL(mock_node_manager, get_name(4)).WillRepeatedly(Return("file_to_del"));
    EXPECT_TRUE(fs->make_file("file_to_del"));
    
    // Remove it
    // remove_file calls go_to("file_to_del").
    // go_to needs name_exist.
    // name_exist needs get_name(4) -> "file_to_del".
    EXPECT_CALL(mock_node_manager, delete_node(4)).Times(1); 
    // Wait, delete_node is called when count reaches 0.
    // rebuild_nodes might keep it alive if version logic? 
    // Current count 1?
    
    EXPECT_TRUE(fs->remove_file("file_to_del"));
}
