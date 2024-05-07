#include <stdlib.h>
#include "zlib.h"
#include "zlib_util.h"
#include "futil.h"
#include "chunk.h"
#include "dynarr.h"
#include "frame.h"
#include "key.h"
#include "hash.h"

#define VERBOSE_DECODE 0

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

int inflate_chunk_3(chunk_h* chunk) {
    int ret;
    int key[KEY_SIZE_DW];
    char* tmp_data;
    FILE* source, * dest;
    void* decompressed_data;
    int compressed_size, decompressed_size;
    unsigned int hash_before, hash_after;
    int decode_verify_res;

    get_decode_key(key);

    // don't ruin the original data if we mess it up
    tmp_data = malloc(chunk->size);
    memcpy(tmp_data, chunk->data, chunk->size);

    // idk either. see base+0x24fc3 to base+0x24fd7
    tmp_data[4] = tmp_data[4] ^ (char)(chunk->id >> 8) ^ (char)(chunk->id);

    hash_before = hash_buf(&tmp_data[4], chunk->size - 4);
    decode_buf(key, &tmp_data[4], chunk->size - 4);
    hash_after = hash_buf(&tmp_data[4], chunk->size - 4);

#if VERBOSE_DECODE == 1
    decode_verify_res = verify_decode(hash_before, hash_after);
    switch (decode_verify_res) {
        case DECODE_OK:
            printf("Decode successful for chunk id: %d, flags: 0x%x\n", chunk->id, chunk->flags);
            break;
        case DECODE_ERR_UNKNOWN:
            printf("Decode info not found for chunk id: %d, flags: 0x%x\n", chunk->id, chunk->flags);
            break;
        case DECODE_ERR_MISMATCH:
            printf("Decode mismatch chunk id: %d, flags: 0x%x\n", chunk->id, chunk->flags);
            break;
        default:
            printf("Decode unknown error for chunk id: %d\n, flags: 0x%x", chunk->id, chunk->flags);
            break;
    }
#endif

    decompressed_size = *(int*)tmp_data;
    compressed_size = *(int*)(&tmp_data[4]);

    source = fmemopen(&tmp_data[8], compressed_size, "r");
    decompressed_data = malloc(decompressed_size);
    dest = fmemopen(decompressed_data, decompressed_size, "w");

    ret = inflate_file(source, dest);

    if (ret != Z_OK) {
        ret = CHUNK_ERR;
    } else {
        ret = CHUNK_OK;

        // set new info
        free(chunk->data);
        chunk->data = decompressed_data;
        chunk->size = decompressed_size;
        chunk->inflated = 1;
    }

    // cleanup
    fclose(source);
    fclose(dest);
    free(tmp_data);

    return ret;
}

int inflate_chunk(chunk_h* chunk) {
    int ret;
    int decode_start, decode_size;
    if (chunk->inflated) return CHUNK_OK;

    if (chunk->flags & 0b0001 == 0) {
        return CHUNK_ERR_NO_IMPL;
    } else if (chunk->flags & 0b0010 == 0) {
        return CHUNK_ERR_NO_IMPL;
    } else {
        // debug
        printf("flags: 0x%x\n", chunk->flags);
        if (chunk->id != CHUNK_FRAMEEVENTS) return CHUNK_ERR_NO_IMPL;
        ret = inflate_chunk_3(chunk);
    }

    return ret;
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
