#pragma once
#include "dynarr.h"

#define EVENT_HEADER        0x3e3e5245 // ER>>
#define EVENT_COUNT         0x73655245 // ERes
#define EVENT_DATA          0x76655245 // ERev
#define EVENT_EXTENSIONDATA 0x706F5245 // ERop
#define EVENT_FOOTER        0x52453c3c // <<ER
#define EVENT_FOOTER_ALT    0x453c3c   /* <<E; used to give a warning message
                                          on mangled eof byte on fclose */

typedef struct {
    short  max_objects;
    short  max_object_info;
    short  num_players;
    dynarr num_conditions;
    dynarr groups;
    dynarr qualifiers;
} frameevents_h;


void init_frameevents_h(FILE* fp, frameevents_h* frameevents);
void free_frameevents_h(frameevents_h* frameevents);

typedef struct {
    short object_info;
    short type;
} qualifier_h;

void init_qualifier_h(FILE* fp, qualifier_h* qualifier);
/* does nothing bc struct has nothing, but exists for consistency */
void free_qualifier_h(qualifier_h* qualifier);

typedef struct {
    short         size;
    unsigned char num_conditions;
    unsigned char num_actions;
    short         flags;
    short         field_0x6;
    int           field_0x8;
    int           field_0xc;
    dynarr        conditions;
    dynarr        actions;
} eventgroup_h;

void init_eventgroup_h(FILE* fp, eventgroup_h* eventgroup);
void free_eventgroup_h(eventgroup_h* eventgroup);

