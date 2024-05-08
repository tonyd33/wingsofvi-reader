#include <stddef.h>

/* converts a wchar string to char string. assumes wchars are valid ascii chars.
   worse version of existing functions, probably. i've had a lot of trouble
   with printing wchars and pretty much all the wchars i've encountered are
   valid ascii, so fk it, i'm just doing this myself. */
void ascii_wchar_to_char(char* out, wchar_t* in, int wchar_size);
