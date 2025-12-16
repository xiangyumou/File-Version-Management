#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "commands/cd_command.h"
#include "interfaces/i_session.h"
#include "logger.h"

// Mock Session
class MockSession : public ffvms::ISession {
public:
    MOCK_METHOD(ffvms::FileSystem&, get_file_system, (), (override));
    MOCK_METHOD(std::vector<std::string>, get_current_path, (), (const, override));
    MOCK_METHOD(std::string, get_current_path_string, (), (const, override));
    MOCK_METHOD(bool, set_current_path, (const std::vector<std::string>& path), (override));
    MOCK_METHOD(std::vector<std::string>, get_previous_path, (), (const, override));
    MOCK_METHOD(bool, change_directory, (const std::string& path), (override));
};

class CdCommandTest : public ::testing::Test {
protected:
    MockSession session;
    ffvms::CdCommand cmd;
};

TEST_F(CdCommandTest, ChangeDirectorySuccess) {
    std::vector<std::string> params = {"new_dir"};
    
    EXPECT_CALL(session, change_directory("new_dir"))
        .Times(1)
        .WillOnce(testing::Return(true));
        
    auto result = cmd.execute(session, params);
    EXPECT_TRUE(result.is_ok());
}

TEST_F(CdCommandTest, ChangeDirectoryFailure) {
    std::vector<std::string> params = {"invalid_dir"};
    
    EXPECT_CALL(session, change_directory("invalid_dir"))
        .Times(1)
        .WillOnce(testing::Return(false));
        
    auto result = cmd.execute(session, params);
    EXPECT_FALSE(result.is_ok());
}

TEST_F(CdCommandTest, ChangeDirectoryPrevious) {
    std::vector<std::string> params = {"-"};
    
    EXPECT_CALL(session, change_directory("-"))
        .Times(1)
        .WillOnce(testing::Return(true));
        
    auto result = cmd.execute(session, params);
    EXPECT_TRUE(result.is_ok());
}

TEST_F(CdCommandTest, InvalidParams) {
    // No params
    std::vector<std::string> params = {};
    auto result = cmd.execute(session, params);
    EXPECT_FALSE(result.is_ok()); // Validation should fail
    
    // Too many params
    params = {"a", "b"};
    result = cmd.execute(session, params);
    EXPECT_FALSE(result.is_ok());
}
