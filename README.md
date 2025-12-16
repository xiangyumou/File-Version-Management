# File Version Management System (FFVMS)

A robust, CMake-based C++ application for efficient file and folder version management. This system allows you to manage multiple versions of a file system structure, enabling branching, history tracking, and deduplicated storage.

## Features

- **Efficient Storage**: Deduplicates file content; identical files (by content) are stored only once.
- **Versioning**: Create new versions inheriting from previous ones. Modifications in a new version do not affect the history.
- **Tree Navigation**: N-ary tree structure for managing complex directory hierarchies.
- **Cross-Platform**: Built with CMake, supporting Windows (MSVC/MinGW) and Linux/macOS.
- **Persistence**: Encrypted storage for metadata and file content.

## Build Instructions

### Prerequisites
- C++17 compliant compiler (GCC, Clang, MSVC)
- CMake 3.15+
- Ninja (optional, recommended for fast builds)

### Building the Project

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

The project includes a comprehensive test suite using GoogleTest.

```bash
cd build
ctest --output-on-failure
```

### Running the Application

```bash
./bin/ffvms          # Linux/Mac
.\bin\ffvms.exe      # Windows
```

> **Note for Windows Users**: The terminal uses UTF-8 encoding. If you see garbled characters, run `chcp 65001` in your console before running the program.

## Command Reference

The system provides a shell-like interface. Below are the available commands:

| Command | Description | Example |
|---------|-------------|---------|
| `create_version` | Create a new version (optionally from a base version) | `create_version 0 "Initial Commit"` |
| `switch_version` | Switch to a specific version ID | `switch_version 1001` |
| `version` | List all available versions | `version` |
| `gcv` | Get Current Version ID | `gcv` |
| `ls` | List directory contents | `ls -a` |
| `cd` | Change directory | `cd my_folder` |
| `mkdir` | Create a new directory | `mkdir new_dir` |
| `touch` | Create a new file | `touch file.txt` |
| `rmf` | Remove a file | `rmf file.txt` |
| `rmd` | Remove a directory | `rmd dir_name` |
| `update_name` | Rename a file or directory | `update_name old new` |
| `update_content` | Update file content | `update_content file.txt "Hello World"` |
| `cat` | Display file content | `cat file.txt` |
| `tree` | Show directory structure tree | `tree` |
| `pwd` | Show current working directory | `pwd` |
| `find` | Search for files/folders by name | `find pattern` |
| `help` | Show available commands | `help` |
| `clear` | Clear the terminal screen | `clear` |
| `vim` | Open file in system editor (vim/notepad) | `vim file.txt` |

## Architecture

The system uses a modular architecture with Dependency Injection and the Command Pattern.
See [docs/architecture.md](docs/architecture.md) for a detailed technical overview.
