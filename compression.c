#include "compression.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LZW_MAX_DICT_SIZE 4096
#define LZW_CLEAR_CODE 256
#define LZW_END_CODE 257

typedef struct {
    unsigned char *data;
    size_t length;
} LZWEntry;

// Compresión
size_t lzw_compress(const unsigned char *input, size_t length, unsigned char **output) {
    LZWEntry *dict = malloc(LZW_MAX_DICT_SIZE * sizeof(LZWEntry));
    for (int i = 0; i < 256; i++) {
        dict[i].data = malloc(1);
        dict[i].data[0] = i;
        dict[i].length = 1;
    }

    size_t dict_size = 258;
    size_t out_capacity = length * 2;
    uint16_t *out_codes = malloc(out_capacity * sizeof(uint16_t));

    size_t out_len = 0;
    size_t pos = 0;
    unsigned char *current = malloc(1);
    size_t curr_len = 0;

    while (pos < length) {
        size_t new_len = curr_len + 1;
        unsigned char *new_str = malloc(new_len);
        memcpy(new_str, current, curr_len);
        new_str[curr_len] = input[pos];

        int found = -1;
        for (size_t i = 0; i < dict_size; i++) {
            if (dict[i].length == new_len && memcmp(dict[i].data, new_str, new_len) == 0) {
                found = i;
                break;
            }
        }

        if (found != -1) {
            free(current);
            current = new_str;
            curr_len = new_len;
        } else {
            int code = -1;
            for (size_t i = 0; i < dict_size; i++) {
                if (dict[i].length == curr_len && memcmp(dict[i].data, current, curr_len) == 0) {
                    code = i;
                    break;
                }
            }
            out_codes[out_len++] = code;

            if (dict_size < LZW_MAX_DICT_SIZE) {
                dict[dict_size].data = new_str;
                dict[dict_size].length = new_len;
                dict_size++;
            } else {
                free(new_str);
            }

            free(current);
            current = malloc(1);
            current[0] = input[pos];
            curr_len = 1;
        }

        pos++;
    }

    if (curr_len > 0) {
        int code = -1;
        for (size_t i = 0; i < dict_size; i++) {
            if (dict[i].length == curr_len && memcmp(dict[i].data, current, curr_len) == 0) {
                code = i;
                break;
            }
        }
        out_codes[out_len++] = code;
    }

    *output = malloc(out_len * sizeof(uint16_t));
    memcpy(*output, out_codes, out_len * sizeof(uint16_t));

    for (size_t i = 0; i < dict_size; i++) {
        free(dict[i].data);
    }
    free(dict);
    free(out_codes);
    free(current);

    return out_len * sizeof(uint16_t);
}

// Descompresión
size_t lzw_decompress(const unsigned char *input, size_t length, unsigned char **output) {
    uint16_t *codes = (uint16_t *)input;
    size_t code_count = length / sizeof(uint16_t);

    LZWEntry *dict = malloc(LZW_MAX_DICT_SIZE * sizeof(LZWEntry));
    for (int i = 0; i < 256; i++) {
        dict[i].data = malloc(1);
        dict[i].data[0] = i;
        dict[i].length = 1;
    }

    size_t dict_size = 258;
    unsigned char *result = NULL;
    size_t result_size = 0;

    uint16_t prev_code = codes[0];
    result = realloc(result, result_size + dict[prev_code].length);
    memcpy(result + result_size, dict[prev_code].data, dict[prev_code].length);
    result_size += dict[prev_code].length;

    for (size_t i = 1; i < code_count; i++) {
        uint16_t code = codes[i];
        unsigned char *entry_data;
        size_t entry_len;

        if (code < dict_size) {
            entry_data = dict[code].data;
            entry_len = dict[code].length;
        } else {
            entry_len = dict[prev_code].length + 1;
            entry_data = malloc(entry_len);
            memcpy(entry_data, dict[prev_code].data, dict[prev_code].length);
            entry_data[entry_len - 1] = dict[prev_code].data[0];
        }

        result = realloc(result, result_size + entry_len);
        memcpy(result + result_size, entry_data, entry_len);
        result_size += entry_len;

        if (dict_size < LZW_MAX_DICT_SIZE) {
            dict[dict_size].length = dict[prev_code].length + 1;
            dict[dict_size].data = malloc(dict[dict_size].length);
            memcpy(dict[dict_size].data, dict[prev_code].data, dict[prev_code].length);
            dict[dict_size].data[dict[dict_size].length - 1] = entry_data[0];
            dict_size++;
        }

        prev_code = code;
    }

    *output = result;

    for (size_t i = 0; i < dict_size; i++) {
        free(dict[i].data);
    }
    free(dict);

    return result_size;
}
