#include <stdlib.h>
#include "dynarr.h"

void init_arr(dynarr* arr, size_t initial) {
    arr->items = malloc(initial * sizeof(void*));
    arr->length = 0;
    arr->_size = initial;
}

void ins_arr(dynarr* arr, void* elt) {
    if (arr->length == arr->_size) {
        arr->_size *= 2;
        arr->items = realloc(arr->items, arr->_size * sizeof(void*));
    }
    arr->items[arr->length++] = elt;
}

void free_arr(dynarr* arr) {
    free(arr->items);
    arr->items = NULL;
    arr->length = arr->_size = 0;
}
