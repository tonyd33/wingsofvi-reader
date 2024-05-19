#include <stddef.h>
#include <string.h>
#include "map.h"

/* begin staticmap */
void init_staticmap(staticmap* map, compare_key_func compare_key) {
    memset(map->keys, 0, MAX_STATICMAP_ITEMS * sizeof(void*));
    memset(map->values, 0, MAX_STATICMAP_ITEMS * sizeof(void*));
    map->compare_key = compare_key;
    map->size = 0;
}

int get_staticmap_index(staticmap* map, void* elt) {
    for (int i = 0; i < MAX_STATICMAP_ITEMS; i++) {
        if (map->compare_key(map->keys[i], elt)) return i;
    }
    return MAP_ERR_NOTFOUND;
}

int get_staticmap(staticmap* map, void* elt, void** out) {
    int index = get_staticmap_index(map, elt);
    if (index != MAP_ERR_NOTFOUND) {
        *out = map->values[index];
        return MAP_OK;
    }

    return MAP_ERR_NOTFOUND;
}

int ins_staticmap(staticmap* map, void* key, void* value) {
    if (map->size >= MAX_STATICMAP_ITEMS) return MAP_ERR_TOOBIG;

    map->size++;
    map->keys[map->size] = key;
    map->values[map->size] = value;
    return MAP_OK;
}

/* end staticmap */

/* begin dynmap */
void init_dynmap(dynmap* map, compare_key_func compare_key) {
    init_arr(&map->keys, 1);
    init_arr(&map->values, 1);
    map->compare_key = compare_key;
}

int get_dynmap_index(dynmap* map, void* elt) {
    for (int i = 0; i < map->keys.length; i++) {
        if (map->compare_key(map->keys.items[i], elt)) return i;
    }
    return MAP_ERR_NOTFOUND;
}

int get_dynmap(dynmap* map, void* elt, void** out) {
    int index = get_dynmap_index(map, elt);
    if (index != MAP_ERR_NOTFOUND) {
        *out = map->values.items[index];
        return MAP_OK;
    }

    return MAP_ERR_NOTFOUND;
}

void ins_dynmap(dynmap* map, void* key, void* value) {
    int index = get_dynmap_index(map, key);
    if (index == -1) {
        ins_arr(&map->keys, key);
        ins_arr(&map->values, value);
    } else {
        map->keys.items[index] = key;
        map->values.items[index] = value;
    }
}

void free_dynmap(dynmap* map) {
    for (int i = 0; i < map->keys.length; i++) {
        map->keys.items[i] = NULL;
    }
    free_arr(&map->keys);

    for (int i = 0; i < map->values.length; i++) {
        map->values.items[i] = NULL;
    }
    free_arr(&map->values);

    map->compare_key = NULL;
}
/* end dynmap */
