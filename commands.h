#ifndef COMMANDS_H
#define COMMANDS_H

#include "FileSystem.h"

void cat(Node *current);
void cd(FileSystem *fs, const char *name);
void dirprint(FileSystem *fs);
void file(Node *current, const char *name);
void greet_human(void);
void help(void);
void ls(Node *node);
void cp(Node *current, const char *name, const char *dest_name);
void mv(Node *current, const char *name, const char *new_name);
void rm(Node *current, const char *name, int recursive);
void mkdir(Node *parent, const char *name);
void rmdir(Node *node, const char *name);
void pwd(Node *node);
void file(Node *current, const char *name);
void search(Node *current, const char *name);
void tree(Node *node, int level);
void save_filesystem(FileSystem *fs, const char *filename);
void load_filesystem(FileSystem *fs, const char *filename);

#endif
