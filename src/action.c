#include <stdio.h>
#include "action.h"
#include "parameter.h"

void init_action_h(FILE* fp, action_h* action) {
    long start = ftell(fp);

    fread(&action->size,             2,    1, fp); // 0x0
    fread(&action->type_1,           2,    1, fp); // 0x2
    fread(&action->type_2,           2,    1, fp); // 0x4
    fread(&action->object_info,      2,    1, fp); // 0x6
    fread(&action->object_info_list, 2,    1, fp); // 0x8
    fread(&action->flags,            1,    1, fp); // 0xa
    fread(&action->control_flags,    1,    1, fp); // 0xb
    fread(&action->num_params,       1,    1, fp); // 0xc
    fread(&action->def_type,         1,    1, fp); // 0xd

    parameter_h* parameter;
    init_arr(&action->parameters, action->num_params);
    for (int i = 0; i < action->num_params; i++) {
        parameter = malloc(sizeof(parameter_h));
        init_parameter_h(fp, parameter);
        ins_arr(&action->parameters, parameter);
    }

    // safeguard
    fseek(fp, start + action->size, SEEK_SET);
}

void free_action_h(action_h* action) {
    for (int i = 0; i < action->parameters.length; i++) {
        parameter_h** parameter = &action->parameters.items[i];
        free_parameter_h(*parameter);
        free(*parameter);
        *parameter = NULL;
    }
    free_arr(&action->parameters);
}


char* get_action_name(action_h* action) {
    switch (action->type_1) {
        case -7:
            break;
        case -6:
            break;
        case -5:
            break;
        case -4:
            break;
        case -3:
            break;
        case -2:
            break;
        case -1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
    }
    return "Unknown action";
}
