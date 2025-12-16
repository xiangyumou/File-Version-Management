# System Architecture

## Overview
The File Version Management System (FFVMS) is a C++ application designed to manage versioned file systems efficiently. It allows storage of multiple file versions with deduplication, supporting operations like branching (creating new versions from old ones) and traversing historical states.

## Core Architecture

### 1. Design Patterns
The system has been refactored from a monolithic design to a modular architecture using the following patterns:

- **Command Pattern**: All terminal operations (`touch`, `mkdir`, `cd`, etc.) are encapsulated as individual command classes implementing the `ICommand` interface. This allows for easy extensibility and testing.
- **Composition**: `FileSystem` uses `std::unique_ptr<BSTree>` instead of inheritance, creating a cleaner clear separation of concerns between business logic (file operations) and data structure logic (tree manipulation).
- **Dependency Injection**: Core components (`FileSystem`, `VersionManager`) accept dependencies (`ILogger`, `INodeManager`, `IStorage`) via interfaces, enabling strict unit testing with Mock Objects.
- **Singleton (Legacy)**: `Logger`, `Saver`, and `NodeManager` still provide singleton accessors for backward compatibility, but the core logic primarily uses injected instances.

### 2. Component Diagram

```mermaid
graph TD
    User[User Input] --> Terminal
    Terminal -->|Parses| CI[Command Interpreter]
    Terminal -->|Dispatches| CR[Command Registry]
    
    CR -->|Execute| Cmd[Concrete Commands]
    Cmd -->|Operates On| FS[File System]
    
    FS -->|Store/Load| VM[Version Manager]
    FS -->|Manipulates| Tree[BSTree]
    
    VM -->|Persists| Storage[Saver (IStorage)]
    FS -->|Logs| Log[Logger (ILogger)]
    Tree -->|Node Ops| NM[Node Manager (INodeManager)]
```

### 3. Key Modules

#### Terminal & Commands
- **Terminal**: The main entry point. Reads user input, parses it via `CommandInterpreter`, and executes functionality via `CommandRegistry`.
- **CommandRegistry**: Maps command strings (e.g., "touch") to executable command objects.
- **ICommand**: The interface for all commands. Returns a `CommandResult` struct indicating success/failure and messages.

#### Core Logic
- **FileSystem**: Orchestrates high-level file operations. Manages the current path and interacts with the version system.
- **VersionManager**: Manages the metadata for different versions (`map<id, versionNode>`). Handles saving/loading version history from disk.
- **BSTree**: A custom N-ary tree implementation representing the file structure. Supports operations like `go_to`, `insert`, and `delete`.

#### Infrastructure
- **Logger**: Handles application logging to both console (`std::cerr`) and disk (`log.chm`).
- **Saver**: Provides encrypted persistence. Serializes data structures and saves them to `data.chm` using a custom FFT-based encryption scheme (legacy feature preserved).
- **NodeManager**: Manages the lifecycle and unique identification of tree nodes.

## Build System
The project uses **CMake** for build configuration:
- `lib/`: Contains the core logic compiled as a static library (`ffvms_lib`).
- `tests/`: Contains GoogleTest-based test suites.
- `main.cpp`: The entry point for the executable.

## Testing Strategy
- **Unit Tests**: Isolated testing of `BSTree` and `Encryptor`.
- **Component Tests**: Testing of `FileSystem` and `CommandInterpreter` using Mocks (`MockLogger`, `MockStorage`).
- **Mocking**: `gmock` is used to simulate file I/O and logging during tests, ensuring fast and reliable verification.
