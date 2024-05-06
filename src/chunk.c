#include <stdlib.h>
#include "zlib.h"
#include "zlib_util.h"
#include "futil.h"
#include "chunk.h"
#include "dynarr.h"
#include "frame.h"
#include "key.h"

void init_chunk_h(FILE* fp, chunk_h* chunk) {
    void* buf;
    chunk->fpos = ftell(fp);
    fread(&chunk->id, 2, 1, fp);
    fread(&chunk->flags, 2, 1, fp);
    fread(&chunk->size, 4, 1, fp);

    // copy the data into the buffer and treat it as a file ptr
    chunk->data = malloc(chunk->size);
    fread(chunk->data, chunk->size, 1, fp);
    chunk->inflated = chunk->flags == 0;
    chunk->chunk_d = NULL;
}

int inflate_chunk(chunk_h* chunk) {
    FILE* source;
    FILE* dest;
    long infl_size;
    void* new_data;
    int ret;
    if (chunk->inflated) return CHUNK_OK;

    if (chunk->flags & 1 != 0 && chunk->id == CHUNK_FRAMEEVENTS) {
        int* decode_key = get_decode_key();
        decode_buf(decode_key, (unsigned int)chunk->data + 4, chunk->size - 4);
    }

    /*
    source = fmemopen(chunk->data, chunk->size, "r");
    dest = tmpfile(); // don't know how big the output is gonna be

    ret = inflate_file(source, dest);
    if (ret != Z_OK) return CHUNK_ERR;

    infl_size = fsize(dest);
    chunk->data = realloc(chunk->data, infl_size);
    fseek(dest, 0, SEEK_SET);
    fread(chunk->data, infl_size, 1, dest);

    printf("Orig size: %d, inflated size: %ld", chunk->size, infl_size);

    fclose(source);
    fclose(dest);

    chunk->inflated = 1;
    */

    return CHUNK_OK;
}


int load_chunk_h(chunk_h* chunk) {
    int ret = 0;
    FILE* fp;

    ret = inflate_chunk(chunk);
    if (ret != CHUNK_OK) {
        /*
        fprintf(stderr,
                "Failed decompressing chunk. id: %d, flags: 0x%x\n",
                chunk->id,
                chunk->flags);
        */
        return ret;
    } else if (!chunk->inflated) {
        /*
        fprintf(stderr,
                "Successfully decompressed chunk. id: %d, flags: 0x%x\n",
                chunk->id,
                chunk->flags);
        */
    }

    if (chunk->chunk_d != NULL) return 0;

    fp = fmemopen(chunk->data, chunk->size, "r");
    switch (chunk->id) {
        case CHUNK_FRAME:
            chunk->chunk_d = malloc(sizeof(frame_h));
            init_frame_h(chunk->chunk_d);
            load_frame_h(fp, chunk->chunk_d, chunk);
            break;
        case CHUNK_FRAMEEVENTS:
            break;
        default:
            // fprintf(stderr, "Load not implemented for chunk id: %d\n", chunk->id);
            break;
    }
    fclose(fp);

    return ret;
}

void free_chunk_h(chunk_h* chunk) {
    if (chunk->data != NULL) {
        free(chunk->data);
        chunk->data = NULL;
    }

    if (chunk->chunk_d != NULL) {
        switch (chunk->id) {
            case CHUNK_FRAME:
                free_frame_h(chunk->chunk_d);
                free(chunk->chunk_d);
                break;
            default:
                break;
        }
    }
}
