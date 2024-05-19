#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <zlib.h>
#include "chunk.h"
#include "futil.h"
#include "dynarr.h"
#include "frame.h"

#define PACK_HEADER "\x77\x77\x77\x77\x49\x87\x47\x12"
#define GAME_HEADER "PAME"
#define UNICODE_GAME_HEADER "PAMU"
#define CNCV1_VERSION 0x207
#define MMF2_PRODUCT_BUILD 0x302

/* max file name size on Windows (wchar_t)
   https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file?redirectedfrom=MSDN#maximum-path-length-limitation
 */
#define MAX_PACK_FILENAME_SIZE 260

/* Spec: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
 * Goes to the .extra section, or the end of the executable section */
void seek_pe_extra(FILE* fp) {
    char buf[8];
    unsigned short num_sections;

    // check magic
    fread(buf, 2, 1, fp);
    if (strncmp(buf, "MZ", 2) != 0) {
        fprintf(stderr, "Failed to read byte\n");
        exit(1);
    }

    fseek(fp, 60, SEEK_SET);

    fread(buf, 4, 1, fp);
    printf("Header offset: 0x%x\n", *(unsigned int*)buf);
    fseek(fp, *(unsigned int*)buf, SEEK_SET);

    fread(buf, 4, 1, fp);

    if (strncmp(buf, "PE\x00\x00", 4) != 0) {
        fprintf(stderr, "Invalid PE signature\n");
        exit(1);
    }

    // COFF header
    // skip machine
    fseek(fp, 2, SEEK_CUR);
    // number of sections
    fread(&num_sections, 2, 1, fp);
    printf("Number of sections: %d\n", num_sections);

    // skip rest of COFF header
    fseek(fp, 16, SEEK_CUR);

    // skip optional headers
    fseek(fp, 224, SEEK_CUR);

    printf("Section table: 0x%lx\n", ftell(fp));

    unsigned int extra_ptr = 0;
    for (unsigned short i = 0; i < num_sections; i++) {
        // name
        fread(buf, 8, 1, fp);
        if (strncmp(buf, ".extra", 8) == 0) {
            // go to ptr_to_raw_data and read it
            fseek(fp, 12, SEEK_CUR);
            fread(&extra_ptr, 4, 1, fp);
            break;
        } else if (i == num_sections - 1) {
            fseek(fp, 8, SEEK_CUR);
            unsigned int size_of_raw_data;
            unsigned int ptr_to_raw_data;
            fread(&size_of_raw_data, 4, 1, fp);
            fread(&ptr_to_raw_data, 4, 1, fp);
            extra_ptr = size_of_raw_data + ptr_to_raw_data;
            break;
        }
        fseek(fp, 32, SEEK_CUR);
    }

    if (extra_ptr == 0) {
        fprintf(stderr, "Could not find extra data\n");
        exit(1);
    }
    fseek(fp, extra_ptr, SEEK_SET);
}

void seek_pack_file(FILE* fp, int has_bingo) {
    char buf[8];
    short filename_size;
    wchar_t filename[MAX_PACK_FILENAME_SIZE] = {0};
    int pack_size;

    fread(&filename_size, 2, 1, fp); // str len
    if (filename_size >= MAX_PACK_FILENAME_SIZE) {
        fprintf(stderr, "Pack file name is too long\n");
        exit(1);
    }

    // * 2 for wchar_t conversion
    // i cannot get fgetws reading this properly or printing the name properly
    // for some reason. screw it, we don't need to print it
    fread(filename, filename_size * 2, 1, fp);

    if (has_bingo) fread(buf, 4, 1, fp); // bingo

    fread(&pack_size, 4, 1, fp);

    fseek(fp, pack_size, SEEK_CUR); // data would be here, but we're ignoring it
}

void read_game_data(FILE* fp) {
    char buf[8];
    int unicode, cnc = 0;
    int runtime_version;

    fread(buf, 4, 1, fp);
    if (strncmp(buf, UNICODE_GAME_HEADER, 4) == 0) {
        unicode = 1;
    } else if (strncmp(buf, GAME_HEADER, 4) != 0) {
        fprintf(stderr, "Invalid game header\n");
        exit(1);
    }

    fread(buf, 2, 1, fp); // runtime_version or cnc
    if (*(short*)buf == CNCV1_VERSION) {
        fprintf(stderr, "cnc reading not implemented");
        exit(1);
    }

    runtime_version = *(short*)buf; // is runtime_version then
    if (runtime_version != MMF2_PRODUCT_BUILD) {
        fprintf(stderr, "Reading only implemented for mmf2");
        exit(1);
    }

    fread(buf, 2, 1, fp); // runtime_sub_version
    fread(buf, 4, 1, fp); // product_version
    fread(buf, 4, 1, fp); // product_build


    dynarr app_chunks; // outermost level of chunks
    chunk_h* chunk;

    init_arr(&app_chunks, 4);
    while (1) {
        chunk = malloc(sizeof(chunk_h));
        init_chunk_h(fp, chunk);
        ins_arr(&app_chunks, chunk);

        if (chunk->id == CHUNK_LAST) {
            break;
        }
    }

    // here is where i'd do something interesting with the loaded chunks

    for (int i = 0; i < app_chunks.length; i++) {
        free_chunk_h(app_chunks.items[i]);
        free(app_chunks.items[i]);
        app_chunks.items[i] = NULL;
    }
    free_arr(&app_chunks);
}

int check_size(long file_size, long curr, long size) {
    return file_size - curr >= size;
}

void read_pack_data(FILE* fp) {
    char buf[8];
    int header_size, data_size, count;
    long start = ftell(fp);
    long file_size = fsize(fp);
    int has_bingo; // dunno what this is. suggested by Anaconda

    fread(buf, 8, 1, fp); // header
    if (strncmp(buf, PACK_HEADER, 8) != 0) {
        fprintf(stderr, "Invalid pack data header\n");
        exit(1);
    }

    fread(&header_size, 4, 1, fp); // 8-12
    fread(&data_size, 4, 1, fp); // 12-16

    fread(buf, 4, 1, fp); // format_version; 16-20
    fread(buf, 4, 2, fp); // vsync flag, dunno; 20-28
    fread(&count, 4, 1, fp); // count; + 28-32

    long offset = ftell(fp);
    for (int i = 0; i < count; i++) {
        if (!check_size(file_size, ftell(fp), 2)) break;

        fread(buf, 2, 1, fp);
        if (!check_size(file_size, ftell(fp), *(short*)buf)) break;

        fseek(fp, *(short*)buf, SEEK_CUR);
        if (!check_size(file_size, ftell(fp), 4)) break;


        fread(buf, 4, 1, fp);
        if (!check_size(file_size, ftell(fp), *(unsigned int*)buf)) break;

        fseek(fp, *(unsigned int*)buf, SEEK_CUR);
    }

    fread(buf, 4, 1, fp); // header
    has_bingo = strncmp(buf, GAME_HEADER, 4) != 0 &&
        strncmp(buf, UNICODE_GAME_HEADER, 4) != 0;

    fseek(fp, offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        seek_pack_file(fp, has_bingo);
    }
}


void read_exe(FILE* fp) {
    seek_pe_extra(fp);
    printf("Pack data starts at 0x%lx\n", ftell(fp));
    read_pack_data(fp);
    printf("Game data starts at 0x%lx\n", ftell(fp));
    read_game_data(fp);
}
