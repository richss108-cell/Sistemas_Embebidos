#ifndef TREE_H
#define TREE_H

#include <stddef.h>

typedef struct {
    char name[256];
    size_t offset;
    size_t size;
} FileEntry;

typedef struct TreeNode {
    FileEntry *files;
    int count;
    struct TreeNode **children;
    int leaf;
} TreeNode;

TreeNode *tree_create(int leaf);
void tree_insert(TreeNode **root, FileEntry entry);
FileEntry *tree_search(TreeNode *root, const char *name);
void tree_list(TreeNode *root);
void tree_free(TreeNode *root);

#endif
