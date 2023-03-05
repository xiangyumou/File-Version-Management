# File & Folder Version Management System

## Introduction

This project was previously called "File Version Management".

Usually people choose to backup a folder by copying the entire folder. In this case, a large number of files or folders that have not been modified are also backed up. In the overall view, the same unmodified file is saved several times, which causes waste of storage space. This system is designed to solve that problem.

The system ensures that each file with the same content is saved only once (even if the names are different). It is also possible to create a new version and have it inherit from a previous version. The new version can access all the content of the previous version, but the two versions were independent of each other. In other words, adding, deleting, or modifying files or folders in the new version will not have any effect on the version it inherits.

## Usage

The system is written in MacOS system. Make sure you have g++ or another compiler that can compile cpp files installed on your computer before using it. You can compile the `main.c` with g++ to generate an executable file. If you do not know this very well, then you can first execute `g++ main.c -o ffvms` to generate the executable file `ffvms`, and then execute `sudo ./ffvms` can start to use the system.

Please note that if your computer system is not MacOS, some of the features of this system may not work properly on your computer.

The system operates via the command line.

The following describes the commands that will be used in the system.

### Commands in the system

| Command ID | Command                | Note                                                         | Example                                                      |
| ---------- | ---------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 0          | add_identifier         | You can see that each command has an ID. you can nickname any command in this system, including this one, through this command. | At first you can only use `add_identifier` to invoke the command. But if you want to use `happyday` to call the command, then you can do it by executing `add_identifier 0 happyday`, where 0 is the ID of the command. After that, every time you execute the `happyday` command, it is equivalent to executing the `add_identifier` command. |
| 1          | delete_identifier      |                                                              |                                                              |
| 2          | switch_version         |                                                              |                                                              |
| 3          | make_file              |                                                              |                                                              |
| 4          | make_dir               |                                                              |                                                              |
| 5          | change_directory       |                                                              |                                                              |
| 6          | remove_file            |                                                              |                                                              |
| 7          | remove_dir             |                                                              |                                                              |
| 8          | update_name            |                                                              |                                                              |
| 9          | update_content         |                                                              |                                                              |
| 10         | get_content            |                                                              |                                                              |
| 11         | tree                   |                                                              |                                                              |
| 12         | goto_last_dir          |                                                              |                                                              |
| 13         | list_directory_content |                                                              |                                                              |
| 14         | create_version         |                                                              |                                                              |
| 15         | version                |                                                              |                                                              |
| 16         | get_current_version    |                                                              |                                                              |
| 17         | init                   |                                                              |                                                              |
| 18         | clear                  |                                                              |                                                              |
| 19         | vim                    |                                                              |                                                              |
| 20         | get_current_path       |                                                              |                                                              |
| 21         | find                   |                                                              |                                                              |

