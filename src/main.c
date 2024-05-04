#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_exe.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s path/to/exe\n", argv[0]);
        printf("Missing executable path\n");
        return 1;
    }

    char* exe = argv[1];

    FILE* fp = fopen(exe, "rb");
    if (fp == 0) {
        printf("Failed to open %s\n", exe);
        return 1;
    }

    read_exe(fp);

    fclose(fp);
}

