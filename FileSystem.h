#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64
#define MAX_CONTENT_LENGTH 256
#define MAX_CHILDREN 64

/* Node structure to represent both files and directories */
typedef struct Node {
  char name[MAX_NAME_LENGTH]; /* Name of the file or directory */
  int is_file; /* Flag to indicate if the node is a file (1) or directory (0) */
  char content[MAX_CONTENT_LENGTH]; /* Content of the file (if it is a file) */
  struct Node
      *children[MAX_CHILDREN]; /* Array of child nodes (for directories) */
  struct Node *parent;         /* Pointer to the parent node */
  int child_count; /* Number of children (valid only for directories) */
  int level;       /* Level of the node in the tree */
} Node;

/* FileSystem structure to manage the root and current node */
typedef struct FileSystem {
  Node *root;    /* Root directory of the file system */
  Node *current; /* Current node (file or directory) */
} FileSystem;

#endif
