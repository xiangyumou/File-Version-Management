cmake_minimum_required(VERSION 3.15)
project(ffvms 
    VERSION 1.1.0
    DESCRIPTION "File & Folder Version Management System"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Option to build tests
option(BUILD_TESTING "Build unit tests" ON)

# Platform-specific settings
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Source files
set(SOURCES
    main.cpp
    lib/src/logger.cpp
    lib/src/encryptor.cpp
    lib/src/saver.cpp
    lib/src/file_manager.cpp
    lib/src/node_manager.cpp
    lib/src/bs_tree.cpp
    lib/src/version_manager.cpp
    lib/src/command_interpreter.cpp
    lib/src/file_system.cpp
    lib/src/terminal.cpp
    lib/src/session.cpp
)

# Library sources (without main.cpp for testing)
set(LIB_SOURCES
    lib/src/logger.cpp
    lib/src/encryptor.cpp
    lib/src/saver.cpp
    lib/src/file_manager.cpp
    lib/src/node_manager.cpp
    lib/src/bs_tree.cpp
    lib/src/version_manager.cpp
    lib/src/command_interpreter.cpp
    lib/src/file_system.cpp
    lib/src/terminal.cpp
    lib/src/session.cpp
    lib/src/command_base.cpp
    lib/src/command_registry.cpp
    lib/src/commands/touch_command.cpp
    lib/src/commands/mkdir_command.cpp
    lib/src/commands/cd_command.cpp
    lib/src/commands/cat_command.cpp
    lib/src/commands/ls_command.cpp
    lib/src/commands/rmf_command.cpp
    lib/src/commands/rmd_command.cpp
    lib/src/commands/update_name_command.cpp
    lib/src/commands/update_content_command.cpp
    lib/src/commands/tree_command.cpp
    lib/src/commands/cdl_command.cpp
    lib/src/commands/pwd_command.cpp
    lib/src/commands/find_command.cpp
    lib/src/commands/create_version_command.cpp
    lib/src/commands/switch_version_command.cpp
    lib/src/commands/version_command.cpp
    lib/src/commands/gcv_command.cpp
    lib/src/commands/clear_command.cpp
    lib/src/commands/vim_command.cpp
    lib/src/commands/help_command.cpp
)

# Create static library for testing
add_library(ffvms_lib STATIC ${LIB_SOURCES})
target_include_directories(ffvms_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/include
)

# Create executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ffvms_lib)

# Include directories for main executable
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/include
)

# Compiler-specific warnings
if(MSVC)
    target_compile_options(ffvms_lib PRIVATE /W4)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(ffvms_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Testing configuration
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
endif()

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Build Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Tests: ${BUILD_TESTING}")
message(STATUS "")
