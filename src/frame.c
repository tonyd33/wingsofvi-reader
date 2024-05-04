#include <stdlib.h>
#include <stdio.h>
#include "dynarr.h"
#include "chunk.h"
#include "frame.h"

void init_frame_h(frame_h* frame) {
    init_arr(&frame->chunks, 1);
}

void load_frame_h(FILE* fp, chunk_h* chunk, frame_h* frame) {
    long data_start = chunk->fpos + 8;
    fseek(fp, data_start, SEEK_SET);

    chunk_h* inner_chunk;
    while (1) {
        inner_chunk = malloc(sizeof(chunk_h));
        init_chunk_h(fp, inner_chunk);
        ins_arr(&frame->chunks, inner_chunk);
        if (inner_chunk->id == CHUNK_FRAMEEVENTS) {
            printf("flags: 0x%x\n", inner_chunk->flags);
        }
        if (inner_chunk->id == CHUNK_LAST) {
            break;
        }
    }
}

void free_frame_h(frame_h* frame) {
    for (int i = 0; i < frame->chunks.length; i++) {
        free(frame->chunks.items[i]);
        frame->chunks.items[i] = NULL;
    }
    free_arr(&frame->chunks);
}
