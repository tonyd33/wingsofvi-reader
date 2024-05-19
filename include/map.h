#pragma once

#include "dynarr.h"

#define MAX_STATICMAP_ITEMS 1024

#define MAP_OK 1
#define MAP_ERR_TOOBIG (-87)
#define MAP_ERR_NOTFOUND (-1)

typedef int (*compare_key_func)(void*, void*);

typedef struct {
    int size;
    void** keys[MAX_STATICMAP_ITEMS];
    void** values[MAX_STATICMAP_ITEMS];
    compare_key_func compare_key;
} staticmap;

void init_staticmap(staticmap* map, compare_key_func compare_key);

int get_staticmap(staticmap* map, void* elt, void** out);
int ins_staticmap(staticmap* map, void* key, void* value);

typedef struct {
    // size is built into keys/values
    dynarr keys;
    dynarr values;
    compare_key_func compare_key;
} dynmap;

void init_dynmap(dynmap* map, compare_key_func compare_key);
void free_dynmap(dynmap* map);

int get_dynmap(dynmap* map, void* elt, void** out);
void ins_dynmap(dynmap* map, void* key, void* value);


