#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define B_TREE_ORDER 3

static TreeNode *tree_create_node(int leaf) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->files = malloc((B_TREE_ORDER - 1) * sizeof(FileEntry));
    node->children = malloc(B_TREE_ORDER * sizeof(TreeNode *));
    node->count = 0;
    node->leaf = leaf;
    for (int i = 0; i < B_TREE_ORDER; i++) {
        node->children[i] = NULL;
    }
    return node;
}

TreeNode *tree_create(int leaf) {
    return tree_create_node(leaf);
}

FileEntry *tree_search(TreeNode *root, const char *name) {
    int i = 0;
    while (i < root->count && strcmp(name, root->files[i].name) > 0) i++;

    if (i < root->count && strcmp(name, root->files[i].name) == 0) {
        return &root->files[i];
    }

    if (root->leaf) return NULL;
    return tree_search(root->children[i], name);
}

static void tree_split_child(TreeNode *parent, int index) {
    TreeNode *child = parent->children[index];
    TreeNode *new_child = tree_create_node(child->leaf);
    new_child->count = B_TREE_ORDER / 2 - 1;

    for (int j = 0; j < new_child->count; j++) {
        new_child->files[j] = child->files[j + B_TREE_ORDER / 2];
    }

    if (!child->leaf) {
        for (int j = 0; j < B_TREE_ORDER / 2; j++) {
            new_child->children[j] = child->children[j + B_TREE_ORDER / 2];
        }
    }

    child->count = B_TREE_ORDER / 2 - 1;

    for (int j = parent->count; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = new_child;

    for (int j = parent->count - 1; j >= index; j--) {
        parent->files[j + 1] = parent->files[j];
    }
    parent->files[index] = child->files[B_TREE_ORDER / 2 - 1];
    parent->count++;
}

static void tree_insert_nonfull(TreeNode *node, FileEntry entry) {
    int i = node->count - 1;

    if (node->leaf) {
        while (i >= 0 && strcmp(entry.name, node->files[i].name) < 0) {
            node->files[i + 1] = node->files[i];
            i--;
        }
        node->files[i + 1] = entry;
        node->count++;
    } else {
        while (i >= 0 && strcmp(entry.name, node->files[i].name) < 0) i--;
        i++;
        if (node->children[i]->count == B_TREE_ORDER - 1) {
            tree_split_child(node, i);
            if (strcmp(entry.name, node->files[i].name) > 0) i++;
        }
        tree_insert_nonfull(node->children[i], entry);
    }
}

void tree_insert(TreeNode **root, FileEntry entry) {
    if (*root == NULL) {
        *root = tree_create(1);
    }
    if ((*root)->count == B_TREE_ORDER - 1) {
        TreeNode *new_root = tree_create(0);
        new_root->children[0] = *root;
        tree_split_child(new_root, 0);
        int i = 0;
        if (strcmp(entry.name, new_root->files[0].name) > 0) i++;
        tree_insert_nonfull(new_root->children[i], entry);
        *root = new_root;
    } else {
        tree_insert_nonfull(*root, entry);
    }
}

void tree_list(TreeNode *root) {
    if (root) {
        for (int i = 0; i < root->count; i++) {
            printf("%s (offset: %zu, size: %zu)\n", root->files[i].name, root->files[i].offset, root->files[i].size);
        }
        if (!root->leaf) {
            for (int i = 0; i <= root->count; i++) {
                tree_list(root->children[i]);
            }
        }
    }
}

void tree_free(TreeNode *root) {
    if (root) {
        if (!root->leaf) {
            for (int i = 0; i <= root->count; i++) {
                tree_free(root->children[i]);
            }
        }
        free(root->files);
        free(root->children);
        free(root);
    }
}
