#include <stdlib.h>
#include "zlib.h"
#include "zlib_util.h"
#include "chunk.h"
#include "dynarr.h"

void init_chunk_h(FILE* fp, chunk_h* chunk) {
    chunk->fpos = ftell(fp);
    fread(&chunk->id, 2, 1, fp);
    fread(&chunk->flags, 2, 1, fp);
    fread(&chunk->size, 4, 1, fp);
    chunk->data = NULL;

    fseek(fp, chunk->size, SEEK_CUR);
}


/*
int load_chunk_h(FILE* fp, chunk_h* chunk) {
    int ret = 0;
    chunk->data = tmpfile();

    // + 8 to ignore id, flags, size
    fseek(fp, chunk->fpos + 8, SEEK_SET);
    if (chunk->flags & 1) {
        ret = inflate_chunk(fp, chunk->data, chunk->size);
    }

    return ret;
}
*/

void free_chunk_h(chunk_h* chunk) {
    if (chunk->data != NULL) {
        free(chunk->data);
        chunk->data = NULL;
    }
    free(chunk);
}
