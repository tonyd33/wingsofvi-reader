#pragma once
#include "dynarr.h"

typedef struct {
    unsigned short size;
    short          type_1;
    short          type_2;
    unsigned short object_info;
    short          object_info_list;
    unsigned char  flags;
    unsigned char  control_flags;
    unsigned char  num_params;
    unsigned char  def_type;
    short          id;
    dynarr         parameters;
} condition_h;

void init_condition_h(FILE* fp, condition_h* condition);
void free_condition_h(condition_h* condition);

char* get_condition_name(condition_h* condition);
