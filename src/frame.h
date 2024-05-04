#pragma once
#include "dynarr.h"

typedef struct {
    char* name;
    char* password;
    int width;
    int height;
    char background[3]; // RGB
    int flags;
} frame;

typedef struct {
    dynarr chunks;
} frame_h;
