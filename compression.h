#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>

size_t lzw_compress(const unsigned char *input, size_t length, unsigned char **output);
size_t lzw_decompress(const unsigned char *input, size_t length, unsigned char **output);

#endif
