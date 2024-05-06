#include <stdlib.h>
#include <stdio.h>
#include "dynarr.h"
#include "chunk.h"
#include "frame.h"

void init_frame_h(frame_h* frame) {
    init_arr(&frame->chunks, 1);
}

// chunk_h for debugging
void load_frame_h(FILE* fp, frame_h* frame, chunk_h* outer_chunk) {
    fseek(fp, 0, SEEK_SET);

    // initialize the chunks into the array
    chunk_h* chunk;
    while (1) {
        chunk = malloc(sizeof(chunk_h));
        long offset = ftell(fp) + 8; // debugging purpose
        init_chunk_h(fp, chunk);
        ins_arr(&frame->chunks, chunk);
        if (chunk->id == CHUNK_FRAMEEVENTS) {
            printf("offset: 0x%lx, size: 0x%x, flag: %d, end: 0x%lx\n", offset + outer_chunk->fpos, chunk->size, chunk->flags, offset + chunk->size + outer_chunk->fpos);
        }
        if (chunk->id == CHUNK_LAST) {
            break;
        }
    }

    // actually load them now
    for (int i = 0; i < frame->chunks.length; i++) {
        chunk = frame->chunks.items[i];
        load_chunk_h(chunk);
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

void init_frameevent_h(frameevent_h* frameevent) {

}
