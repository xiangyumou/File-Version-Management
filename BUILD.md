# Build and Test Instructions

## Prerequisites
- **CMake**: Version 3.15 or higher.
- **C++ Compiler**: GCC (MinGW), Clang, or MSVC supporting C++17.
- **Ninja** (Optional): For faster builds.

## Building the Project

1.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

2.  **Configure the project:**
    ```bash
    # For MinGW/Unix Makefiles
    cmake -G "MinGW Makefiles" ..
    # OR for Visual Studio
    cmake -G "Visual Studio 16 2019" ..
    # OR for Ninja
    cmake -G Ninja ..
    ```

3.  **Build:**
    ```bash
    cmake --build .
    ```

    The executable `ffvms` (or `ffvms.exe`) will be created in `build/bin`.

## Running Tests

The project uses GoogleTest for unit testing.

1.  **Configure with tests enabled (Default is ON):**
    ```bash
    cmake -DBUILD_TESTING=ON ..
    ```

2.  **Build the tests:**
    ```bash
    cmake --build .
    ```
    This will compile `ffvms_test` executable.

3.  **Run the tests:**
    
    You can use CTest (recommended):
    ```bash
    ctest --output-on-failure
    ```
    
    Or run the test executable directly:
    ```bash
    ./bin/ffvms_test
    # On Windows PowerShell:
    .\bin\ffvms_test.exe
    ```

## Troubleshooting

### Path Encoding Issues (Windows)
If your project path contains non-ASCII characters (e.g., Chinese characters like `我的资料库`), CMake or the underlying build tool (like Ninja or MinGW Make) might fail with "File not found" or "Syntax incorrect" errors.

**Solution:**
Move the project to a path with only English characters (e.g., `C:\Projects\File-Version-Management`) and try building again.
