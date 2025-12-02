
#include "FileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to create a new node (file or directory) */
Node *create_node(const char name[MAX_NAME_LENGTH], int is_file, Node *parent) {
  Node *node = (Node *)malloc(sizeof(Node));
  int i;
  strcpy(node->name, name);
  node->is_file = is_file;
  if (is_file) {
    node->content[0] = '\0'; /* Initialize file content to an empty string */
  } else {
    node->child_count = 0; /* Initialize directory's child count */
    for (i = 0; i < MAX_CHILDREN; i++) {
      node->children[i] = NULL;
    }
  }
  node->parent = parent;
  return node;
}

/* Function to add a child node to a directory */
void add_child(Node *parent, Node *child) {
  if (parent->child_count < MAX_CHILDREN) {
    parent->children[parent->child_count++] = child;
    child->parent = parent;
  } else {
    printf("Directory limit reached\n");
  }
}

/* Function to find a node by name */
Node *find_child(Node *root, const char *name) {
  int i;
  for (i = 0; i < root->child_count; i++) {
    if (strcmp(root->children[i]->name, name) == 0) {
      return root->children[i];
    }
  }
  return NULL;
}

/* Function to remove a child node by name */
void remove_child(Node *parent, const char *name) {
  int i, j;
  for (i = 0; i < parent->child_count; i++) {
    if (strcmp(parent->children[i]->name, name) == 0) {
      free(parent->children[i]);
      for (j = i; j < parent->child_count - 1; j++) {
        parent->children[j] = parent->children[j + 1];
      }
      parent->child_count--;
      return;
    }
  }
  printf("No such file or directory: %s\n", name);
}

/* Function to print the directory tree */
void print_tree(Node *node, int level) {
  int i, j;
  for (i = 0; i < level; i++) {
    printf("  ");
  }
  printf("%s%s\n", node->name, node->is_file ? "" : "/");
  for (j = 0; j < node->child_count; j++) {
    print_tree(node->children[j], level + 1);
  }
}

/* Function to go backwards in the directory tree */
Node *back_track(Node *node, int levels) {
  while (levels > 0 && node->parent != NULL) {
    node = node->parent;
    levels--;
  }
  return node;
}

/* Recursive function to copy a node (file or directory) */
Node *copy_node(Node *current, const char *name, Node *parent) {
  int i;
  Node *copy = create_node(name, current->is_file, parent);
  if (current->is_file) {
    strcpy(copy->content, current->content);
  } else {
    for (i = 0; i < current->child_count; i++) {
      Node *child_copy =
          copy_node(current->children[i], current->children[i]->name, copy);
      copy->children[copy->child_count++] = child_copy;
    }
  }
  return copy;
}

/* Function to check for duplicate names in the current directory */
int has_duplicate_name(Node *current, const char *name) {
  int i;
  for (i = 0; i < current->child_count; i++) {
    if (strcmp(current->children[i]->name, name) == 0) {
      return 1;
    }
  }
  return 0;
}
