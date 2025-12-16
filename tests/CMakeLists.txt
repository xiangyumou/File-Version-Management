# Tests CMakeLists.txt

# Unit test executable
set(TEST_SOURCES
    unit/encryptor_test.cpp
    unit/bs_tree_test.cpp
    unit/file_system_test.cpp
    unit/commands_test.cpp
    unit/session_test.cpp
    unit/cd_command_test.cpp
)

add_executable(ffvms_test ${TEST_SOURCES})

target_link_libraries(ffvms_test PRIVATE
    ffvms_lib
    GTest::gtest_main
    GTest::gmock
)

target_include_directories(ffvms_test PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

include(GoogleTest)
gtest_discover_tests(ffvms_test)
