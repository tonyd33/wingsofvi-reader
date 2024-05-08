#include <stdio.h>
#include <stdlib.h>
#include "parameter.h"

enum parameter_code {
    PARAM_OBJECT,
    PARAM_TIME,
    PARAM_SHORT,
    PARAM_INT,
    PARAM_SAMPLE,
    PARAM_CREATE,
    PARAM_EVERY,
    PARAM_KEY,
    PARAM_EXPRESSION,
    PARAM_POS,
    PARAM_ZONE,
    PARAM_COLOR,
    PARAM_CLICK,
    PARAM_PROGRAM,
    PARAM_REMARK,
    PARAM_GROUP,
    PARAM_GROUPPOINTER,
    PARAM_FILENAME,
    PARAM_STRING,
    PARAM_COMPARETIME,
    PARAM_TWOSHORTS,
    PARAM_CHARACTERENCODING,
    PARAM_UNKNOWN,
};

int simplify_parameter_code(short code) {
    switch (code) {
        // object
        case 1:
            return PARAM_OBJECT;
        // time
        case 2:
            return PARAM_TIME;
        // short
        case 3:
        case 4:
        case 10:
        case 11:
        case 12:
        case 17:
        case 26:
        case 31:
        case 43:
        case 49:
        case 50:
        case 57:
        case 58:
        case 60:
        case 61:
            return PARAM_SHORT;
        // int
        case 5:
        case 25:
        case 29:
        case 34:
        case 48:
        case 56:
            return PARAM_INT;
        // sample
        case 6:
        case 7:
        case 35:
        case 36:
            return PARAM_SAMPLE;
        // create
        case 9:
        case 21:
            return PARAM_CREATE;
        // every
        case 13:
            return PARAM_EVERY;
        // keyparameter
        case 14:
        case 44:
            return PARAM_KEY;
        // expressionparameter
        case 15:
        case 22:
        case 23:
        case 27:
        case 28:
        case 45:
        case 46:
        case 52:
        case 53:
        case 54:
        case 59:
        case 62:
            return PARAM_EXPRESSION;
        // position
        case 16:
            return PARAM_POS;
        // zone
        case 19:
            return PARAM_ZONE;
        // color
        case 24:
            return PARAM_COLOR;
        // click
        case 32:
            return PARAM_CLICK;
        // program
        case 33:
            return PARAM_PROGRAM;
        // remark
        case 37:
            return PARAM_REMARK;
        // group
        case 38:
            return PARAM_GROUP;
        // grouppointer
        case 39:
            return PARAM_GROUPPOINTER;
        // filename
        case 40:
        case 63:
            return PARAM_FILENAME;
        // string
        case 41:
        case 64:
            return PARAM_STRING;
        // comparetime
        case 42:
            return PARAM_COMPARETIME;
        // twoshorts
        case 47:
        case 51:
            return PARAM_TWOSHORTS;
        // characterencoding
        case 65:
        case 66:
            return PARAM_CHARACTERENCODING;
        default:
            return PARAM_UNKNOWN;
    }
}

void init_parameter_h(FILE* fp, parameter_h* parameter) {
    // TODO: Implement
    parameter->parameter_d = NULL;
}

void free_parameter_h(parameter_h* parameter) {
    // TODO: Implement
    if (parameter->parameter_d != NULL) {
        switch (simplify_parameter_code(parameter->code)) {
            case PARAM_OBJECT:
                break;
            case PARAM_TIME:
                break;
            case PARAM_SHORT:
                break;
            case PARAM_INT:
                break;
            case PARAM_SAMPLE:
                break;
            case PARAM_CREATE:
                break;
            case PARAM_EVERY:
                break;
            case PARAM_KEY:
                break;
            case PARAM_EXPRESSION:
                break;
            case PARAM_POS:
                break;
            case PARAM_ZONE:
                break;
            case PARAM_COLOR:
                break;
            case PARAM_CLICK:
                break;
            case PARAM_PROGRAM:
                break;
            case PARAM_REMARK:
                break;
            case PARAM_GROUP:
                break;
            case PARAM_GROUPPOINTER:
                break;
            case PARAM_FILENAME:
                break;
            case PARAM_STRING:
                break;
            case PARAM_COMPARETIME:
                break;
            case PARAM_TWOSHORTS:
                break;
            case PARAM_CHARACTERENCODING:
                break;
            case PARAM_UNKNOWN:
                fprintf(stderr, "Unknown parameter code0x%x\n", parameter->code);
                exit(1);
        }
    }
}

