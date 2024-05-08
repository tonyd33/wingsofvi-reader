#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include "wchar_util.h"
#include "chunk_string.h"

#define MAX_STRING_SIZE 256

void init_chunk_string_h(FILE* fp, chunk_string_h* chunk_string) {
    wchar_t curr = 0;

    int size = 0;
    long start_pos = ftell(fp);

    do {
        size += 1;
        fread(&curr, 2, 1, fp);
    } while (curr != 0 && size < MAX_STRING_SIZE);

    size = ftell(fp) - start_pos;
    chunk_string->value = malloc(size);
    chunk_string->size = size;


    fseek(fp, start_pos, SEEK_SET);
    fread(chunk_string->value, size, 1, fp);

    char ascii[(size >> 1) + 1];
    ascii_wchar_to_char(ascii, chunk_string->value, size);
    printf("Found chunk string: %s\n", ascii);
}

void free_chunk_string_h(chunk_string_h* chunk_string) {
    free(chunk_string->value);
    chunk_string->value = NULL;
}
