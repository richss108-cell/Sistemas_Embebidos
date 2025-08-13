#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

#include "tree.h"

typedef struct {
    TreeNode *index;
    FILE *data_file;
} FileSystem;

FileSystem *fs_create(const char *data_filename);
void fs_add_file(FileSystem *fs, const char *filename);
void fs_get_file(FileSystem *fs, const char *filename);
void fs_list_files(FileSystem *fs);
void fs_close(FileSystem *fs);

#endif
