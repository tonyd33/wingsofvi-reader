#pragma once
#include "dynarr.h"
#include "chunk.h"

typedef struct {
    dynarr chunks;
} frame_h;


void init_frame_h(FILE* fp, frame_h* frame);

void free_frame_h(frame_h* frame);
