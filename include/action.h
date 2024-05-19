#pragma once

#include "dynarr.h"

typedef struct {
    unsigned short size;
    short          type_1;
    short          type_2;
    unsigned short object_info;
    short          object_info_list;
    unsigned char  control_flags;
    unsigned char  flags;
    unsigned char  num_params;
    unsigned char  def_type;
    dynarr         parameters;
} action_h;

void init_action_h(FILE* fp, action_h* action);
void free_action_h(action_h* action);

char* get_action_name(action_h* action);
