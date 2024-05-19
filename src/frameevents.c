#include <stdio.h>
#include <stdlib.h>
#include "frameevents.h"
#include "action.h"
#include "condition.h"
#include "futil.h"

void init_frameevents_h(FILE* fp, frameevents_h* frameevents) {
    int id;
    char buf[8];

    int done = 0;

    init_arr(&frameevents->groups, 1);
    init_arr(&frameevents->num_conditions, 1);
    init_arr(&frameevents->qualifiers, 1);

    while (!done) {
        long pre_pos = ftell(fp);
        fread(&id, 4, 1, fp);

        switch (id) {
            case EVENT_HEADER:
                int i;
                short num_qualifiers;
                qualifier_h* qualifier;

                fread(&frameevents->max_objects, 2, 1, fp);
                fread(&frameevents->max_object_info, 2, 1, fp);
                fread(&frameevents->max_objects, 2, 1, fp);

                for (i = 0; i < 17; i++) {
                    fread(buf, 2, 1, fp);
                    ins_arr(&frameevents->num_conditions, *(short*)buf);
                }


                fread(&num_qualifiers, 2, 1, fp);
                for (i = 0; i < num_qualifiers; i++) {
                    qualifier = malloc(sizeof(qualifier_h));
                    init_qualifier_h(fp, qualifier);
                    ins_arr(&frameevents->qualifiers, qualifier);
                }
                break;
            case EVENT_COUNT:
                fread(buf, 4, 1, fp); // size
                break;
            case EVENT_EXTENSIONDATA:
                fread(buf, 4, 1, fp);
                break;
            case EVENT_DATA:
                long end;
                int size;
                eventgroup_h* eventgroup;

                fread(&size, 4, 1, fp); // size
                end = ftell(fp) + size;

                while (ftell(fp) < end) {
                    eventgroup = malloc(sizeof(eventgroup_h));
                    init_eventgroup_h(fp, eventgroup);
                    ins_arr(&frameevents->groups, eventgroup);
                }

                // ensure we hit the end properly
                fseek(fp, end, SEEK_SET);
                break;
            case EVENT_FOOTER:
                done = 1;
                break;
            case EVENT_FOOTER_ALT:
                done = 1;
                fprintf(stderr, "Reluctantly accepting alt footer. Likely fclose without an extra byte\n");
                fprintf(stderr, "Details: file size: 0x%lx, pre pos: 0x%lx, pos: 0x%lx\n", fsize(fp), pre_pos, ftell(fp));
                break;
            default:
                fprintf(stderr, "Found unknown event id, 0x%x fatal\n", id);
                fprintf(stderr, "Details: file size: 0x%lx, pre pos: 0x%lx, pos: 0x%lx\n", fsize(fp), pre_pos, ftell(fp));
                exit(1);
        }
    }
}

void free_frameevents_h(frameevents_h* frameevents) {
    int i;

    // only shorts are stored in here, so no need to free elts
    for (i = 0; i < frameevents->num_conditions.length; i++) {
        frameevents->num_conditions.items[i] = NULL;
    }
    free_arr(&frameevents->num_conditions);

    for (i = 0; i < frameevents->groups.length; i++) {
        eventgroup_h** eventgroup = &frameevents->groups.items[i];
        free_eventgroup_h(*eventgroup);
        free(*eventgroup);
        *eventgroup = NULL;
    }
    free_arr(&frameevents->groups);

    for (i = 0; i < frameevents->qualifiers.length; i++) {
        qualifier_h** qualifier = &frameevents->qualifiers.items[i];
        free_qualifier_h(*qualifier);
        free(*qualifier);
        *qualifier = NULL;
    }
    free_arr(&frameevents->qualifiers);
}

void init_qualifier_h(FILE* fp, qualifier_h* qualifier) {
    fread(&qualifier->object_info, 2, 1, fp);
    fread(&qualifier->type, 2, 1, fp);
}

void free_qualifier_h(qualifier_h* qualifier) {}

void init_eventgroup_h(FILE* fp, eventgroup_h* eventgroup) {
    long start = ftell(fp);
    // could just read all of this in at once, but i don't want to rely on the
    // how the struct definition is ordered
    fread(&eventgroup->size,           2, 1, fp); // 0x0
    eventgroup->size *= -1;
    fread(&eventgroup->num_conditions, 1, 1, fp); // 0x2
    fread(&eventgroup->num_actions,    1, 1, fp); // 0x3
    fread(&eventgroup->flags,          2, 1, fp); // 0x4
    fread(&eventgroup->field_0x6,      2, 1, fp); // 0x6
    fread(&eventgroup->field_0x8,      4, 1, fp); // 0x8
    fread(&eventgroup->field_0xc,      4, 1, fp); // 0xc

    init_arr(&eventgroup->conditions, eventgroup->num_conditions);
    init_arr(&eventgroup->actions, eventgroup->num_actions);

    int i;
    condition_h* condition;
    action_h* action;

    for (i = 0; i < eventgroup->num_conditions; i++) {
        condition = malloc(sizeof(condition_h));
        init_condition_h(fp, condition);
        ins_arr(&eventgroup->conditions, condition);
    }

    for (i = 0; i < eventgroup->num_actions; i++) {
        action = malloc(sizeof(action_h));
        init_action_h(fp, action);
        ins_arr(&eventgroup->actions, action);
    }

    if (eventgroup->size <= 0) {
        printf("bad\n");
        exit(1);
    }

    // safeguard
    fseek(fp, start + eventgroup->size, SEEK_SET);
}

void free_eventgroup_h(eventgroup_h* eventgroup) {
    int i;

    for (i = 0; i < eventgroup->conditions.length; i++) {
        condition_h** condition = &eventgroup->conditions.items[i];
        free_condition_h(*condition);
        free(*condition);
        *condition = NULL;
    }
    free_arr(&eventgroup->conditions);

    for (i = 0; i < eventgroup->actions.length; i++) {
        action_h** action = &eventgroup->actions.items[i];
        free_action_h(*action);
        free(*action);
        *action = NULL;
    }
    free_arr(&eventgroup->actions);
}

