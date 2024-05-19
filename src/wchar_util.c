#include <stdlib.h>
#include "wchar_util.h"

#define MAX_STRING_SIZE 256

void ascii_wchar_to_char(char* out, wchar_t* in, int wchar_size) {
    for (int i = 0; i < wchar_size; i += 2) {
        out[(i >> 1)] = *(char*)((int)in + i);
    }
}

void init_wstring_h(FILE* fp, wstring_h* wstring) {
    wchar_t curr = 0;

    int size = 0;
    long start_pos = ftell(fp);

    do {
        size += 1;
        fread(&curr, 2, 1, fp);
    } while (curr != 0 && size < MAX_STRING_SIZE);

    size = ftell(fp) - start_pos;
    wstring->value = malloc(size);
    wstring->size = size;


    fseek(fp, start_pos, SEEK_SET);
    fread(wstring->value, size, 1, fp);

    /* char ascii[(size >> 1) + 1]; */
    /* ascii_wchar_to_char(ascii, string->value, size); */
    /* printf("Found string: %s (len %d)\n", ascii, size); */
}

void free_wstring_h(wstring_h* wstring) {
    free(wstring->value);
    wstring->value = NULL;
}
