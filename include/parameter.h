#pragma once

typedef struct {
    short size;
    short code;
    void* parameter_d;
} parameter_h;

void init_parameter_h(FILE* fp, parameter_h* parameter);
void free_parameter_h(parameter_h* parameter);
