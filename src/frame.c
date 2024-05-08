#include <stdlib.h>
#include <stdio.h>
#include "dynarr.h"
#include "chunk.h"
#include "frame.h"

void init_frame_h(FILE* fp, frame_h* frame) {
    long start;
    init_arr(&frame->chunks, 1);

    fseek(fp, 0, SEEK_SET);

    // initialize the chunks into the array
    chunk_h* chunk;
    while (1) {
        start = ftell(fp);
        chunk = malloc(sizeof(chunk_h));
        init_chunk_h(fp, chunk);
        ins_arr(&frame->chunks, chunk);
        if (chunk->id == CHUNK_LAST) {
            break;
        }
    }
}

void free_frame_h(frame_h* frame) {
    for (int i = 0; i < frame->chunks.length; i++) {
        free_chunk_h(frame->chunks.items[i]);
        free(frame->chunks.items[i]);
        frame->chunks.items[i] = NULL;
    }
    free_arr(&frame->chunks);
}
