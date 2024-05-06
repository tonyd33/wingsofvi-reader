#pragma once
#include "dynarr.h"
#include "chunk.h"

typedef struct {
    dynarr chunks;
} frame_h;

typedef struct {
    int max_objects;
    int max_bject_info;
    int number_of_players;
} frameevent_h;


void init_frame_h(frame_h* frame);

void load_frame_h(FILE* fp, frame_h* frame, chunk_h* chunk);

void free_frame_h(frame_h* frame);
