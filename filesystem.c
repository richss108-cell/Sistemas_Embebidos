#include "filesystem.h"
#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileSystem *fs_create(const char *data_filename) {
    FileSystem *fs = malloc(sizeof(FileSystem));
    fs->index = NULL;
    fs->data_file = fopen(data_filename, "ab+");
    if (!fs->data_file) {
        perror("Error al abrir archivo de datos");
        exit(1);
    }
    return fs;
}

void fs_add_file(FileSystem *fs, const char *filename) {
    FILE *in = fopen(filename, "rb");
    if (!in) {
        perror("Error al abrir archivo");
        return;
    }

    fseek(in, 0, SEEK_END);
    size_t size = ftell(in);
    rewind(in);

    unsigned char *buffer = malloc(size);
    fread(buffer, 1, size, in);
    fclose(in);

    unsigned char *compressed = NULL;
    size_t comp_size = lzw_compress(buffer, size, &compressed);

    fseek(fs->data_file, 0, SEEK_END);
    size_t offset = ftell(fs->data_file);
    fwrite(compressed, 1, comp_size, fs->data_file);
    fflush(fs->data_file);

    FileEntry entry;
    strncpy(entry.name, filename, sizeof(entry.name));
    entry.offset = offset;
    entry.size = comp_size;

    tree_insert(&fs->index, entry);

    free(buffer);
    free(compressed);

    printf("Archivo '%s' agregado y comprimido.\n", filename);
}

void fs_get_file(FileSystem *fs, const char *filename) {
    FileEntry *entry = tree_search(fs->index, filename);
    if (!entry) {
        printf("Archivo no encontrado: %s\n", filename);
        return;
    }

    unsigned char *compressed = malloc(entry->size);
    fseek(fs->data_file, entry->offset, SEEK_SET);
    fread(compressed, 1, entry->size, fs->data_file);

    unsigned char *decompressed = NULL;
    size_t decomp_size = lzw_decompress(compressed, entry->size, &decompressed);

    FILE *out = fopen(filename, "wb");
    fwrite(decompressed, 1, decomp_size, out);
    fclose(out);

    free(compressed);
    free(decompressed);

    printf("Archivo '%s' extraído.\n", filename);
}

void fs_list_files(FileSystem *fs) {
    printf("Archivos en el sistema:\n");
    tree_list(fs->index);
}

void fs_close(FileSystem *fs) {
    tree_free(fs->index);
    fclose(fs->data_file);
    free(fs);
}
