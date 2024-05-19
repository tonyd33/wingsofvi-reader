#pragma once
#include <wchar.h>
#include "wchar_util.h"
#include "dynarr.h"

typedef struct {
    short size;
    short code;
    void* parameter_d;
} parameter_h;

void init_parameter_h(FILE* fp, parameter_h* parameter);
void free_parameter_h(parameter_h* parameter);

/* begin definite parameters */

/* begin packed definite parameters */
#pragma pack(push, 1)
typedef struct {
    short          object_info_list;
    unsigned short object_info;
    short          object_type;
} param_object_h;

typedef struct {
    int timer;
    int loops;
} param_time_h;

typedef struct {
    short value;
} param_short_h;

typedef struct {
    int value;
} param_int_h;

typedef struct {
    short object_info_parent;
    short flags;
    short x;
    short y;
    short slope;
    short angle;
    int   direction;
    short type_parent;
    short object_info_list;
    short layer;
} param_pos_h;

typedef struct {
    param_pos_h pos;
    short       object_instance;
    short       object_info;
} param_create_h;

typedef struct {
    int delay;
    int compteur;
} param_every_h;

typedef struct {
    short x1, y1, x2, y2;
} param_zone_h;

typedef struct {
    char red;
    char green;
    char blue;
    char pad;
} param_color_h;

typedef struct {
    char button;
    char double_click;
} param_click_h;

typedef struct {
    int   timer;
    int   loops;
    short comparison;
} param_comparetime_h;

typedef struct {
    short value1, value2;
} param_twoshorts_h;

typedef struct {
    param_pos_h    pos;
    unsigned short object_instance;
    unsigned short object_info;
    int            unknown;
    short          speed;
} param_shoot_h;
#pragma pack(pop)
/* end packed definite parameters */

typedef wstring_h param_string_h;
typedef wstring_h param_filename_h;

typedef struct {
    short          handle;
    unsigned short flags;
    wstring_h      name;
} param_sample_h;

void init_param_sample_h(FILE* fp, param_sample_h* param_sample);
void free_param_sample_h(param_sample_h* param_sample);

typedef struct {
    short  comparison;
    dynarr expressions;
} param_expression_h;

void init_param_expression_h(FILE* fp, param_expression_h* param_expression);
void free_param_expression_h(param_expression_h* param_expression);

/* end definite parameters */
