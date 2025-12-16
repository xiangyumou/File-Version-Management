#include "file_system.h"
#include "session.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Mock FileSystem if needed, or use real one if it's lightweight.
// FileSystem uses BSTree internally.
// For Session unit test, we mostly want to test path resolution and state
// logic. However, Session methods call file_system_.navigate_to_path(). Ideally
// we should mock FileSystem, but it's a concrete class. We can test Session
// logic that doesn't depend on FS success, or ensure FS is in valid state.

class SessionTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize real FileSystem (it constructs a root node)
    // We might need to ensure it's clean.
  }

  FileSystem fs;
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
  // Test resolve_path with ".." from a non-root path
  // Manually set session path (simulating being in /a/b)
  session.set_current_path({"a", "b"});

  // Resolve ".." should give us /a
  std::vector<std::string> input = {".."};
  auto resolved = session.resolve_path(input);
  std::vector<std::string> expected = {"a"};
  EXPECT_EQ(resolved, expected);
}

TEST_F(SessionTest, ChangeDirectoryBasic) {
  // Test basic path string formatting
  // Root path
  EXPECT_EQ(session.get_current_path_string(), "/");
  EXPECT_TRUE(session.get_current_path().empty());

  // Test resolve_path with multiple components
  std::vector<std::string> input = {"a", "b", "c"};
  auto resolved = session.resolve_path(input);
  std::vector<std::string> expected = {"a", "b", "c"};
  EXPECT_EQ(resolved, expected);

  // Test resolve_path with ".."
  session.set_current_path({"a", "b"});
  std::vector<std::string> input2 = {"..", "c"};
  auto resolved2 = session.resolve_path(input2);
  std::vector<std::string> expected2 = {"a", "c"};
  EXPECT_EQ(resolved2, expected2);
}
