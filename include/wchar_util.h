#pragma once
#include <stdio.h>
#include <stddef.h>

/* converts a wchar string to char string. assumes wchars are valid ascii chars.
   worse version of existing functions, probably. i've had a lot of trouble
   with printing wchars and pretty much all the wchars i've encountered are
   valid ascii, so fk it, i'm just doing this myself. */
void ascii_wchar_to_char(char* out, wchar_t* in, int wchar_size);

typedef struct {
    wchar_t* value;
    int      size;
} wstring_h;


void init_wstring_h(FILE* fp, wstring_h* wstring);
void free_wstring_h(wstring_h* wstring);

typedef void (*init_wstring_h_func)(FILE*, wstring_h*);
typedef void (*free_wstring_h_func)(wstring_h*);

