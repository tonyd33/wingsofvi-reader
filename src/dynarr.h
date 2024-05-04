#pragma once

typedef struct {
    void** items;
    size_t length;
    size_t _size;
} dynarr;

void init_arr(dynarr* arr, size_t initial);
void ins_arr(dynarr* arr, void* elt);
void free_arr(dynarr* arr);
