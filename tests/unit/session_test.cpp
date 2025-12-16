#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "session.h"
#include "file_system.h"
#include <vector>
#include <string>

// Mock FileSystem if needed, or use real one if it's lightweight.
// FileSystem uses BSTree internally. 
// For Session unit test, we mostly want to test path resolution and state logic.
// However, Session methods call file_system_.navigate_to_path().
// Ideally we should mock FileSystem, but it's a concrete class.
// We can test Session logic that doesn't depend on FS success, or ensure FS is in valid state.

class SessionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize real FileSystem (it constructs a root node)
        // We might need to ensure it's clean.
    }
    
    ffvms::FileSystem fs;
    ffvms::Session session{fs};
};

TEST_F(SessionTest, InitialPathIsRoot) {
    auto path = session.get_current_path();
    // Assuming FS starts at root (empty vector or just root?)
    // FS implementation: BSTree constructor creates root. 
    // get_current_path returns vector of strings.
    // If at root, vector might be empty or contain nothing?
    // Let's check FS behavior.
    EXPECT_TRUE(path.empty()) << "Initial path should be empty (root)";
}

TEST_F(SessionTest, ResolvePathRelative) {
    // Current: /
    // Resolve: a/b -> /a/b
    std::vector<std::string> input = {"a", "b"};
    auto resolved = session.resolve_path(input);
    std::vector<std::string> expected = {"a", "b"};
    EXPECT_EQ(resolved, expected);
}

TEST_F(SessionTest, ResolvePathParent) {
    // Set current manually if possible? 
    // Session::set_current_path calls fs.navigate_to_path.
    // So we need to create dirs in FS to move there.
    
    // Setup: Create /a/b
    EXPECT_TRUE(fs.make_dir("a"));
    EXPECT_TRUE(fs.change_directory("a")); // FS move
    EXPECT_TRUE(fs.make_dir("b"));
    
    // Sync session to /a/b (simulate)
    // Actually we should perform moves via Session to test it integrally.
    // But verify ResolvePath logic separately first? resolve_path is const and depends on current_path_.
}

TEST_F(SessionTest, ChangeDirectoryLogic) {
    // 1. Mkdir /a
    EXPECT_TRUE(fs.make_dir("a"));
    
    // 2. cd a
    EXPECT_TRUE(session.change_directory("a"));
    EXPECT_EQ(session.get_current_path_string(), "/a");
    
    // 3. Mkdir b inside a
    EXPECT_TRUE(fs.make_dir("b"));
    
    // 4. cd b
    EXPECT_TRUE(session.change_directory("b"));
    EXPECT_EQ(session.get_current_path_string(), "/a/b");
    
    // 5. cd ..
    EXPECT_TRUE(session.change_directory(".."));
    EXPECT_EQ(session.get_current_path_string(), "/a");
    
    // 6. cd - (back to /a/b)
    EXPECT_TRUE(session.change_directory("-"));
    EXPECT_EQ(session.get_current_path_string(), "/a/b");
    
    // 7. cd / (absolute)
    EXPECT_TRUE(session.change_directory("/")); // Root?
    // Wait, implementation of absolute path in Session::change_directory checks if path.front() == '/'
    // If path is just "/", input_vector is empty.
    // Logic: target_path loop won't run. empty target_path = root.
    EXPECT_TRUE(session.get_current_path().empty());
}
