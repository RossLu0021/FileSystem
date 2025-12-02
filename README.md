# Fake File System

A simulation of a file system implemented in C. This project demonstrates the use of pointers, structs, and command line arguments to manage a hierarchical data structure.

## Features

- **File & Directory Management**: Create, remove, copy, and move files and directories.
- **Navigation**: Change directories (`cd`), list contents (`ls`), print working directory (`pwd`).
- **Persistence**: Save and load the filesystem state to/from a file.
- **Recursive Operations**: Recursively remove directories (`rm -r`).
- **Visualization**: View the directory tree structure (`tree`).

## Compilation

To compile the project, simply run:

```bash
make
```

This will generate the `lab3` executable.

## Usage

Run the program:

```bash
./lab3
```

### Available Commands

| Command | Description | Usage |
| :--- | :--- | :--- |
| `mkdir` | Create a new directory | `mkdir <dirname>` |
| `cd` | Change directory | `cd <dirname>` or `cd ..` |
| `ls` | List contents of current directory | `ls` |
| `pwd` | Print current working directory | `pwd` |
| `file` | Create a new file | `file <filename>` (prompts for content) |
| `cat` | Display file content | `cat <filename>` |
| `cp` | Copy a file or directory | `cp <source> <dest>` |
| `mv` | Move or rename a file or directory | `mv <source> <dest>` |
| `rm` | Remove a file (or directory with `-r`) | `rm <name>` or `rm -r <dirname>` |
| `rmdir` | Remove an empty directory | `rmdir <dirname>` |
| `tree` | Display the directory tree | `tree` |
| `save` | Save filesystem to file | `save <filename>` |
| `load` | Load filesystem from file | `load <filename>` |
| `help` | List available commands | `help` |
| `exit` | Exit the program | `exit` |

## Example Session

```bash
mkdir documents
cd documents
file notes.txt
# (Enter content: "Meeting at 10am")
cp notes.txt backup.txt
ls
# Output: notes.txt backup.txt
cd ..
tree
# Displays the tree structure
save fs_backup.dat
exit
```
