#include <stdio.h>
#include <string.h>
#include "filesystem.h"

int main() {
    FileSystem *fs = fs_create("battlefs.dat");
    char command[256], arg[256];

    while (1) {
        printf("battlefs> ");
        if (scanf("%s", command) != 1) break;

        if (strcmp(command, "add") == 0) {
            scanf("%s", arg);
            fs_add_file(fs, arg);
        } else if (strcmp(command, "get") == 0) {
            scanf("%s", arg);
            fs_get_file(fs, arg);
        } else if (strcmp(command, "list") == 0) {
            fs_list_files(fs);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Comando no reconocido.\n");
        }
    }

    fs_close(fs);
    return 0;
}
