#include "wchar_util.h"

void ascii_wchar_to_char(char* out, wchar_t* in, int wchar_size) {
    for (int i = 0; i < wchar_size; i += 2) {
        out[(i >> 1)] = *(char*)((int)in + i);
    }
}
