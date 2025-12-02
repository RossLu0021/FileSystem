
#include "FileSystem.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to display help */
void help(void) {
  printf("Available commands:\n");
  printf("cd: Change directory\n");
  printf("ls: List directory contents\n");
  printf("pwd: Print working directory\n");
  printf("cat: Display file contents\n");
  printf("cp: Copy file or directory\n");
  printf("mv: Move or rename file or directory\n");
  printf("rm: Remove file\n");
  printf("mkdir: Create new directory\n");
  printf("rmdir: Remove directory\n");
  printf("file: Create a new file with content\n");
  printf("search: Search for a file or directory\n");
  printf("greet: System just saying hi \n");
  printf("tree: Display the directory tree\n");
  printf("dirprint: Print directory structure\n");
  printf("help: List available commands\n");
  printf("exit: Exit the file system\n");
}

/* Function to change directory */
void cd(FileSystem *fs, const char *name) {
  if (strcmp(name, "..") == 0) {
    if (fs->current->parent != NULL) {
      fs->current = fs->current->parent;
    }
  } else {
    Node *target = find_child(fs->current, name);
    if (target == NULL) {
      printf("Directory or file not found: %s\n", name);
      return;
    }
    if (target->is_file) {
      printf("%s is a file, moving to its parent directory.\n", name);
      if (target->parent != NULL) {
        fs->current = target->parent;
        printf("Moved to parent directory: %s\n", fs->current->name);
      }
    } else {
      fs->current = target;
    }
  }
}

/* Function to display file contents */
void cat(Node *current) {
  if (current->is_file) {
    printf("%s\n", current->content);
  } else {
    printf("Not a file.\n");
  }
}

/* Function to list the contents of the current directory */
void ls(Node *node) {
  int i;
  for (i = 0; i < node->child_count; i++) {
    printf("%s%s\n", node->children[i]->name,
           node->children[i]->is_file ? "" : "/");
  }
}

/* Function to print the current working directory path */
void pwd(Node *node) {
  if (node->parent != NULL) {
    pwd(node->parent);
  }
  printf("/%s", node->name);
}

/* Function to add a file */
void file(Node *current, const char *name) {
  if (current->child_count < MAX_CHILDREN) {
    Node *file = create_node(name, 1, current);
    printf("Enter content for the file: ");
    /* getchar(); Remove this as fgets in main loop consumes the newline */
    fgets(file->content, MAX_CONTENT_LENGTH, stdin);
    /* Remove newline from content if present */
    file->content[strcspn(file->content, "\n")] = 0;
    current->children[current->child_count++] = file;
  } else {
    printf("Cannot add more files, directory is full.\n");
  }
}

/* Function to remove a file or directory (recursive) */
void rm_recursive(Node *node) {
  int i;
  if (!node->is_file) {
    for (i = 0; i < node->child_count; i++) {
      rm_recursive(node->children[i]);
    }
  }
  /* We don't free node here because it's done in remove_child or by caller if
     it's not a child removal but a full tree nuke. Actually, remove_child frees
     the node. But if we are recursing, we need to free children first. Wait,
     remove_child removes ONE child from parent. If we want to remove a
     directory recursively, we just need to empty it first? Or does remove_child
     handle it? remove_child just frees the node. If the node has children, they
     are leaked! So remove_child needs to be recursive or we need a recursive
     helper.
  */
}

void free_node_recursive(Node *node) {
  int i;
  if (!node->is_file) {
    for (i = 0; i < node->child_count; i++) {
      free_node_recursive(node->children[i]);
    }
  }
  free(node);
}

/* Function to remove a file */
void rm(Node *current, const char *name, int recursive) {
  Node *target = find_child(current, name);
  int i, j;
  if (target == NULL) {
    printf("File or directory not found: %s\n", name);
    return;
  }

  if (target->is_file) {
    remove_child(current, name);
  } else {
    if (recursive) {
      /* We need to remove the child from current's list, AND free it
       * recursively */
      /* remove_child only frees the node itself, not children. */
      /* We need to implement a proper recursive free. */

      /* Let's manually remove it from the list first, then free it recursively
       */
      for (i = 0; i < current->child_count; i++) {
        if (strcmp(current->children[i]->name, name) == 0) {
          /* Found it. Now free it recursively. */
          free_node_recursive(current->children[i]);

          /* Shift remaining children */
          for (j = i; j < current->child_count - 1; j++) {
            current->children[j] = current->children[j + 1];
          }
          current->child_count--;
          return;
        }
      }
    } else {
      printf("%s is a directory, use rmdir or rm -r to remove directories.\n",
             name);
    }
  }
}

/* Function to create a directory */
void mkdir(Node *parent, const char *name) {
  Node *new_dir = create_node(name, 0, parent);
  add_child(parent, new_dir);
}

/* Function to remove a directory */
void rmdir(Node *node, const char *name) {
  Node *dir = find_child(node, name);
  if (dir != NULL && dir->child_count == 0) {
    remove_child(node, name);
  } else {
    printf("Directory is not empty or does not exist\n");
  }
}

/* Function to copy a node (file or directory) */
/* Function to copy a node (file or directory) */
void cp(Node *current, const char *name, const char *dest_name) {
  char new_name[MAX_NAME_LENGTH];
  Node *target = find_child(current, name);
  Node *copy;

  if (target == NULL) {
    printf("File or directory not found: %s\n", name);
    return;
  }

  if (dest_name == NULL) {
    if (snprintf(new_name, MAX_NAME_LENGTH, "copy_of_%s", target->name) >=
        MAX_NAME_LENGTH) {
      printf("New name too long.\n");
      return;
    }
  } else {
    strncpy(new_name, dest_name, MAX_NAME_LENGTH - 1);
    new_name[MAX_NAME_LENGTH - 1] = '\0'; /* Ensure null-termination */
  }

  if (has_duplicate_name(current, new_name)) {
    printf("Copy already exists: %s\n", new_name);
    return;
  }

  copy = copy_node(target, new_name, current);
  add_child(current, copy);
}

/* Function to move or rename a file or directory */
/* Function to move or rename a file or directory */
void mv(Node *current, const char *name, const char *new_name) {
  Node *target = find_child(current, name);
  if (target == NULL) {
    printf("File or directory not found: %s\n", name);
    return;
  }

  if (has_duplicate_name(current, new_name)) {
    printf("Destination already exists: %s\n", new_name);
    return;
  }

  strncpy(target->name, new_name, MAX_NAME_LENGTH - 1);
  target->name[MAX_NAME_LENGTH - 1] = '\0'; /* Ensure null-termination */
}

/* Custom Command: Function to search for a file or directory */
void search(Node *current, const char *name) {
  int i;
  if (strcmp(current->name, name) == 0) {
    printf("Found: /%s\n", current->name);
    return;
  }
  for (i = 0; i < current->child_count; i++) {
    search(current->children[i], name);
  }
}

void greet_human(void) {
  char command[3];
  printf("Hello There thank you for using me, Hope I will be a great help");
  printf(" You are Human right?\n");

  /* Check the first letter only because that tells all we need to know */
  printf("Please enter Y for Yes or N for No: ");

  /* Fix buffer overflow: limit input width */
  scanf("%2s", command);

  /* These took a while , credit: https://ascii.co.uk/art/doors &&
   * https://www.asciiart.eu/weapons/shields*/

  if (command[0] == 'N' || command[0] == 'n') {
    printf("I do not serve Robots, only humans\n");
    printf("I will now self-destruct\n");
    printf("                /¯¯\\\n");
    printf("               \\__/\n");
    printf("                 ||\n");
    printf("                 ||\n");
    printf("                |  |\n");
    printf("                |  |\n");
    printf("                |  |\n");
    printf("                |  |\n");
    printf("                |  |\n");
    printf("                |  |\n");
    printf("  .         --.----.--.\n");
    printf("          .-----\\__/-----.\n");
    printf("   ___---¯¯////¯¯|\\/|¯¯\\\\\\\\---___\n");
    printf("/¯¯ __O_--////   |  |   \\\\\\\\--_O__ ¯¯\\\n");
    printf("| O?¯      ¯¯¯    |  |    ¯¯¯      ¯?O |\n");
    printf("|  '    _.-.      |  |      .-._    '  |\n");
    printf("|O|    ?..?      ./  \\.      ?..?    |O|\n");
    printf("| |     '?. .-.  | /\\ |  .-. .?'     | |\n");
    printf("| ---__  ¯?__?  /|\\¯¯/|\\  ?__?¯  __--- |\n");
    printf("|O     \\         ||\\/ |         /     O|\n");
    printf("|       \\  /¯?_  ||   |  _?¯\\  /       |\n");
    printf("|       / /    - ||   | -    \\ \\       |\n");
    printf("|O   __/  | __   ||   |   __ |  \\__   O|\n");
    printf("| ---     |/  -_/||   |\\_-  \\|     --- |\n");
    printf("|O|            \\ ||   | /            |O|\n");
    printf("\\ '              ||   |        ^~DLF ' /\n");
    printf(" \\O\\    _-¯?.    ||   |    .?¯-_    /O/\n");
    printf("  \\ \\  /  /¯¯¯?  ||   |  ?¯¯¯\\  \\  / /\n");
    printf("   \\O\\/   |      ||   |      |   \\/O/\n");
    printf("    \\     |      ||   |      |     /\n");
    printf("     '.O  |_     ||   |     _|  O.'\n");
    printf("        '._O'.__/||   |\\__.O_.\n");
    printf("           '._ O ||   | O _.'\n");
    printf("              '._||   |_.\n");
    printf("                 ||   |\n");
    printf("                 ||   |\n");
    printf("                 | \\/ |\n");
    printf("                 |  | |\n");
    printf("                  \\ |/\n");
    printf("                   \\/\n");
  } else {
    printf("Good good Just checking\n");
    printf(" You Can Enter\n");
    printf("   /|\n");
    printf("  / |\n");
    printf(" /__|___\n");
    printf("|       |\n");
    printf("|       |\n");
    printf("|       |\n");
    printf("|       +`\n");
    printf("|       |\n");
    printf("|_______|\n");
  }
}

/* Custom Command: Function to display the directory tree */
void tree(Node *node, int level) {
  int i, j;
  for (i = 0; i < level; i++) {
    printf("  ");
  }
  printf("%s%s\n", node->name, node->is_file ? "" : "/");
  for (j = 0; j < node->child_count; j++) {
    tree(node->children[j], level + 1);
  }
}

/* Custom Command: Function to print directory structure */
void dirprint(FileSystem *fs) {
  int i;
  Node *current = fs->current;
  Node *parent = current->parent;

  printf("Current Directory: %s\n", current->name);
  printf("Parent Directory: %s\n", parent ? parent->name : "None");
  printf("Siblings: ");
  if (parent) {
    for (i = 0; i < parent->child_count; i++) {
      if (parent->children[i] != current) {
        printf("%s ", parent->children[i]->name);
      }
    }
  }
  printf("\nPath to root:\n");
  pwd(current);
  printf("\n");
}

void save_node(Node *node, FILE *fp) {
  int i;
  /* Format: Name, IsFile, Content (if file), ChildCount */
  fprintf(fp, "%s\n%d\n", node->name, node->is_file);
  if (node->is_file) {
    fprintf(fp, "%s\n", node->content);
  } else {
    fprintf(fp, "%d\n", node->child_count);
    for (i = 0; i < node->child_count; i++) {
      save_node(node->children[i], fp);
    }
  }
}

void save_filesystem(FileSystem *fs, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("Error opening file for saving.\n");
    return;
  }
  save_node(fs->root, fp);
  fclose(fp);
  printf("Filesystem saved to %s\n", filename);
}

Node *load_node(FILE *fp, Node *parent) {
  char name[MAX_NAME_LENGTH];
  int is_file;
  char buffer[MAX_CONTENT_LENGTH];
  int child_count;
  Node *node;
  int i; /* Moved 'i' declaration here to maintain scope and correctness. */

  if (fscanf(fp, "%s", name) != 1)
    return NULL;
  if (fscanf(fp, "%d", &is_file) != 1)
    return NULL;

  node = create_node(name, is_file, parent);

  if (is_file) {
    /* Consume newline after reading integer */
    fgetc(fp);
    if (fgets(buffer, MAX_CONTENT_LENGTH, fp) != NULL) {
      buffer[strcspn(buffer, "\n")] = 0; /* Remove trailing newline */
      strcpy(node->content, buffer);
    }
  } else {
    if (fscanf(fp, "%d", &child_count) != 1)
      return NULL;
    /* Consume newline after reading integer */
    fgetc(fp);
    for (i = 0; i < child_count; i++) {
      Node *child = load_node(fp, node);
      if (child != NULL) {
        add_child(node, child);
      } else {
        /* Handle error during child loading, potentially free already loaded
           children. For simplicity, we'll just return NULL and let the caller
           handle it. */
        free_node_recursive(node); /* Free partially loaded node */
        return NULL;
      }
    }
  }
  return node;
}

void load_filesystem(FileSystem *fs, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file for loading.\n");
    return;
  }

  /* Free existing tree to prevent memory leaks */
  if (fs->root != NULL) {
    free_node_recursive(fs->root);
  }

  {
    Node *new_root = load_node(fp, NULL);
    if (new_root != NULL) {
      fs->root = new_root;
      fs->current = fs->root;
      printf("Filesystem loaded from %s\n", filename);
    } else {
      printf("Error loading filesystem. Initializing with a new root.\n");
      fs->root = create_node("root", 0, NULL);
      fs->current = fs->root;
    }
  }
  fclose(fp);
}
