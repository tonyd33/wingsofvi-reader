#include <stdlib.h>
#include "zlib.h"
#include "zlib_util.h"
#include "futil.h"
#include "chunk.h"
#include "dynarr.h"
#include "key.h"
#include "hash.h"
#include "bits.h"
// begin chunk types
#include "frame.h"
#include "frameevents.h"
#include "chunk_string.h"
// end chunk types

#define NEEDS_DECODE(var)   CHECK_BIT(var, 1)
#define NEEDS_INFLATE(var)  CHECK_BIT(var, 0)

/* if nonzero, decoding buffers will only succeed if before and after decode
 * hashes matches those that have been discovered by trying to load many areas
 * of the game, and will output warnings. */
#define STRICT_DECODE 0

int decode_buf_with_check(void* data, unsigned int size);

int inflate_chunk(chunk_h* chunk);

/* loads a chunk into memory and decodes/inflates it and subchunks if
 * necessary.
 * PRECONDITION: fp points to the start of a chunk
 * POSTCONDITION: fp points right after this chunk */
void init_chunk_h(FILE* fp, chunk_h* chunk) {
    void* buf;
    long next;

    chunk->fpos = ftell(fp);
    fread(&chunk->id, 2, 1, fp);
    fread(&chunk->flags, 2, 1, fp);
    fread(&chunk->orig_size, 4, 1, fp);
    chunk->size = chunk->orig_size;

    // copy the data into the buffer and treat it as a file ptr
    chunk->data = malloc(chunk->size);
    fread(chunk->data, chunk->size, 1, fp);
    chunk->inflated = chunk->flags == 0;
    chunk->chunk_d = NULL;

    load_chunk_h(chunk);

    // don't rely on load_chunk_h to go to the right offset. we must guarantee
    // we're at the end of the chunk
    // + 8 is for the id, flags and size header
    next = chunk->fpos + chunk->orig_size + 8;
    fseek(fp, next, SEEK_SET);
}


int load_chunk_h(chunk_h* chunk) {
    int ret = 0;
    FILE* fp; // inner

    ret = inflate_chunk(chunk);
    if (ret != CHUNK_OK) {
        fprintf(stderr,
                "Failed decompressing chunk. id: %d, flags: 0x%x\n",
                chunk->id,
                chunk->flags);
        return ret;
    }

    if (chunk->chunk_d != NULL) return 0;

    fp = fmemopen(chunk->data, chunk->size, "r");
    // !don't forget to add a free handler below when adding something here!
    switch (chunk->id) {
        case CHUNK_FRAME:
            chunk->chunk_d = malloc(sizeof(frame_h));
            init_frame_h(fp, chunk->chunk_d);
            break;
        case CHUNK_FRAMEPASSWORD:
        case CHUNK_FRAMENAME:
            chunk->chunk_d = malloc(sizeof(chunk_string_h));
            init_chunk_string_h(fp, chunk->chunk_d);
            break;
        case CHUNK_FRAMEEVENTS:
            chunk->chunk_d = malloc(sizeof(frameevents_h));
            init_frameevents_h(fp, chunk->chunk_d);
            char* data = chunk->data;
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
                break;
            case CHUNK_FRAMEPASSWORD:
            case CHUNK_FRAMENAME:
                free_chunk_string_h(chunk->chunk_d);
                break;
            case CHUNK_FRAMEEVENTS:
                free_frameevents_h(chunk->chunk_d);
                break;
            default:
                break;
        }

        free(chunk->chunk_d);
        chunk->chunk_d = NULL;
    }
}

int decode_buf_with_check(void* data, unsigned int size) {
    int ret;
    unsigned int hash_before, hash_after;
    int key[KEY_SIZE_DW];

    get_decode_key(key);

    hash_before = hash_buf(data, size);
    decode_buf(key, data, size);
    hash_after = hash_buf(data, size);

    ret = verify_decode(hash_before, hash_after);
#if STRICT_DECODE == 1
    switch (ret) {
        case DECODE_OK:
            break;
        case DECODE_ERR_UNKNOWN:
            fprintf(stderr, "Decode info not found\n");
            break;
        case DECODE_ERR_MISMATCH:
            fprintf(stderr, "Decode mismatch\n");
            break;
        default:
            fprintf(stderr, "Decode unknown error\n");
            break;
    }
#else
    ret = DECODE_OK;
#endif
    return ret;
}

/* the data in the chunk may need further processing before we can properly
 * dissect it. we may have to decode it and/or decompress it with zlib.
 *
 * yeah, this function ain't pretty, but it's better than looking at Ghidra's
 * attempt to decompile this. */
int inflate_chunk(chunk_h* chunk) {
    if (chunk->inflated || chunk->flags == 0) {
        chunk->inflated = 1;
        return CHUNK_OK;
    }

    int ret = CHUNK_OK;
    // set chunk->data to this on success, free upon failure
    char* new_data;

    // if things go wrong, we don't want to corrupt the existing buffer
    // so make a copy
    new_data = malloc(chunk->size);
    memcpy(new_data, chunk->data, chunk->size);

    // 1. decode/unmix first byte if necessary
    if (NEEDS_DECODE(chunk->flags)) {
        int decode_offset = (NEEDS_INFLATE(chunk->flags)) ? 4 : 0;

        // yes, we're checking chunk->id. see base+0x24fc3 to base+0x24fc9.
        // if first bit of id set, we have to unmix the first byte.
        if (chunk->id & 1) {
            new_data[decode_offset] =
                new_data[decode_offset] ^
                (char)(chunk->id >> 8) ^
                (char)(chunk->id);
        }

        ret = decode_buf_with_check(
                &new_data[decode_offset],
                chunk->size - decode_offset
                ) == DECODE_OK ? CHUNK_OK : CHUNK_ERR;
    }

    // 2. if previous state successful and if needs inflate, try inflating.
    //    on success, store the result into new_data.
    if (ret == CHUNK_OK && NEEDS_INFLATE(chunk->flags)) {
        FILE* source;
        FILE* dest;
        char* decompressed_data;
        int compressed_size, decompressed_size;

        decompressed_size = *(int*)new_data;
        compressed_size = *(int*)((int)new_data + 4);

        /* + 1 here and below for EOF byte upon fclose
         *
         * man 3 fmemopen:
         *
         * "When a stream that has been opened for writing is flushed
         * (fflush(3)) or closed (fclose(3)), a null byte is written at the
         * end of the buffer if there is space. The caller should ensure that
         * an extra byte is available in the buffer (and that size counts that
         * byte) to allow for this." */
        decompressed_data = malloc(decompressed_size + 1);

        source = fmemopen(&new_data[8], compressed_size, "r");
        dest = fmemopen(decompressed_data, decompressed_size + 1, "w");

        ret = inflate_file(source, dest);

        if (ret == Z_OK) {
            ret = CHUNK_OK;
            free(new_data);
            new_data = decompressed_data;
            chunk->size = decompressed_size;
        } else {
            ret = CHUNK_ERR;
        }

        fclose(source);
        fclose(dest);
    }

    if (ret == CHUNK_OK) {
        free(chunk->data);
        chunk->data = new_data;
        chunk->inflated = 1;
    } else {
        if (new_data != NULL) free(new_data);
    }

    return ret;
}
