#include <stdlib.h>
#include <stdio.h>
#include "condition.h"
#include "parameter.h"

void init_condition_h(FILE* fp, condition_h* condition) {
    long start = ftell(fp);

    fread(&condition->size,             2,    1, fp); // 0x0
    fread(&condition->type_1,           2,    1, fp); // 0x2
    fread(&condition->type_2,           2,    1, fp); // 0x4
    fread(&condition->object_info,      2,    1, fp); // 0x6
    fread(&condition->object_info_list, 2,    1, fp); // 0x8
    fread(&condition->flags,            1,    1, fp); // 0xa
    fread(&condition->control_flags,    1,    1, fp); // 0xb
    fread(&condition->num_params,       1,    1, fp); // 0xc
    fread(&condition->def_type,         1,    1, fp); // 0xd
    fread(&condition->id,               2,    1, fp); // 0xe

    parameter_h* parameter;
    init_arr(&condition->parameters, condition->num_params);
    for (int i = 0; i < condition->num_params; i++) {
        parameter = malloc(sizeof(parameter_h));
        init_parameter_h(fp, parameter);
        ins_arr(&condition->parameters, parameter);
    }

    // safeguard
    fseek(fp, start + condition->size, SEEK_SET);
}

void free_condition_h(condition_h* condition) {
    for (int i = 0; i < condition->parameters.length; i++) {
        parameter_h** parameter = &condition->parameters.items[i];
        free_parameter_h(*parameter);
        free(*parameter);
        *parameter = NULL;
    }
    free_arr(&condition->parameters);
}

char* condition_type_1_n1[] = {
    "Always",
    "Never",
    "Compare",
    "RestrictFor",
    "Repeat",
    "Once",
    "NotAlways",
    "CompareGlobalValue",
    "Remark",
    "NewGroup",
    "GroupEnd",
    "GroupActivated",
    "RECORDKEY",
    "MenuSelected",
    "FilesDropped",
    "OnLoop",
    "MenuChecked",
    "MenuEnabled",
    "MenuVisible",
    "CompareGlobalString",
    "CloseSelected",
    "ClipboardDataAvailable",
    "OnGroupActivation",
    "OrFiltered",
    "OrLogical",
    "Chance",
    "ElseIf",
    "CompareGlobalValueIntEqual",
    "CompareGlobalValueIntNotEqual",
    "CompareGlobalValueIntLessEqual",
    "CompareGlobalValueIntLess",
    "CompareGlobalValueIntGreaterEqual",
    "CompareGlobalValueIntGreater",
    "CompareGlobalValueDoubleEqual",
    "CompareGlobalValueDoubleNotEqual",
    "CompareGlobalValueDoubleLessEqual",
    "CompareGlobalValueDoubleLess",
    "CompareGlobalValueDoubleGreaterEqual",
    "CompareGlobalValueDoubleGreater",
    "RunningAs"
};

char* condition_type_1_n2[] = {
    "SampleNotPlaying",
    "SpecificMusicNotPlaying"
    "NoSamplesPlaying",
    "NoMusicPlaying",
    "MusicFinished",
    "SamplePaused",
    "MusicPaused",
    "ChannelNotPlaying",
    "ChannelPaused",
};

char* condition_type_1_n3[] = {
    "StartOfFrame",
    "EndOfFrame"
    "LEVEL",
    "EndOfApplication",
    "IsObstacle",
    "IsLadder",
    "VsyncEnabled",
    "ApplicationResumed",
    "FrameLoaded",
    "FrameSaved",
};

char* condition_type_1_n4[] = {
    "TimerGreater"
    "TimerLess",
    "TimerEquals",
    "Every",
    "CompareAwayTime",
    "OnTimerEvent",
    "TimerEquals",
    "Every",
};

char* condition_type_1_n5[] = {
    "NoAllObjectsInZone"
    "AllObjectsInZone",
    "CompareObjectCount",
    "PickRandomObjectInZoneOld",
    "PickRandomObject",
    "PickObjectsInZone",
    "PickFromFixed",
    "PickAlterableValue",
    "Unknown"
    "Unknown"
    "PickFlagOn",
    "PickFlagOff",
    "NoAllObjectsInZone",
    "AllObjectsInZone",
    "CompareObjectCount",
    "PickRandomObjectInZone",
    "PickRandomObject",
    "PickObjectsInZone",
    "PickFromFixed",
    "PickAlterableValue",
    "PickFlagOn",
    "PickFlagOff",
    "PickObjectsInLine",
};

char* condition_type_1_n6[] = {
    "KeyPressed"
    "KeyDown",
    "MouseInZone",
    "MouseOnObject",
    "MouseClicked",
    "MouseClickedInZone",
    "ObjectClicked",
    "WhileMousePressed",
    "AnyKeyPressed",
    "MouseVisible",
    "MouseWheelUp",
    "MouseWheelDown",
};

char* condition_type_1_n7[] = {
    "PLAYERPLAYING",
    "CompareScore",
    "NumberOfLives",
    "PlayerKeyPressed",
    "PlayerDied",
    "PlayerKeyDown",
};

char* get_condition_name(condition_h* condition) {
    short type_2_norm = (condition->type_2 * -1) - 1;
    switch (condition->type_1) {
        case -1:
            if (0 <= type_2_norm && type_2_norm <= 39) {
                return condition_type_1_n1[type_2_norm];
            }
            break;
        case -2:
            if (0 <= type_2_norm && type_2_norm <= 8) {
                return condition_type_1_n2[type_2_norm];
            }
            break;
        case -3:
            if (0 <= type_2_norm && type_2_norm <= 9) {
                return condition_type_1_n3[type_2_norm];
            }
            break;
        case -4:
            if (0 <= type_2_norm && type_2_norm <= 7) {
                return condition_type_1_n4[type_2_norm];
            }
            break;
        case -5:
            if (0 <= type_2_norm && type_2_norm <= 22) {
                return condition_type_1_n5[type_2_norm];
            }
            break;
        case -6:
            if (0 <= type_2_norm && type_2_norm <= 11) {
                return condition_type_1_n6[type_2_norm];
            }
            break;
        case -7:
            if (0 <= type_2_norm && type_2_norm <= 5) {
                return condition_type_1_n7[type_2_norm];
            }
            break;
        case 2:
            break;
        case 4:
            break;
        case 7:
            break;
        case 9:
            break;
    }
    return "Unknown condition";
}

