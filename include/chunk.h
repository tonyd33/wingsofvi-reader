#pragma once

#include <stdio.h>
#include "dynarr.h"

// start chunk ids {{{
#define CHUNK_PREVIEW                      4386
#define CHUNK_APPMINIHEADER                8738
#define CHUNK_APPHEADER                    8739
#define CHUNK_APPNAME                      8740
#define CHUNK_APPAUTHOR                    8741
#define CHUNK_APPMENU                      8742
#define CHUNK_EXTPATH                      8743
#define CHUNK_EXTENSIONS                   8744
#define CHUNK_FRAMEITEMS                   8745
#define CHUNK_GLOBALEVENTS                 8746
#define CHUNK_FRAMEHANDLES                 8747
#define CHUNK_EXTDATA                      8748
#define CHUNK_ADDITIONAL_EXTENSION         8749
#define CHUNK_APPEDITORFILENAME            8750
#define CHUNK_APPTARGETFILENAME            8751
#define CHUNK_APPDOC                       8752
#define CHUNK_OTHEREXTS                    8753
#define CHUNK_GLOBALVALUES                 8754
#define CHUNK_GLOBALSTRINGS                8755
#define CHUNK_EXTENSIONS2                  8756
#define CHUNK_APPICON_16x16x8              8757
#define CHUNK_DEMOVERSION                  8758
#define CHUNK_SECNUM                       8759
#define CHUNK_BINARYFILES                  8760
#define CHUNK_APPMENUIMAGES                8761
#define CHUNK_ABOUTTEXT                    8762
#define CHUNK_COPYRIGHT                    8763
#define CHUNK_GLOBALVALUENAMES             8764
#define CHUNK_GLOBALSTRINGNAMES            8765
#define CHUNK_MVTEXTS                      8766
#define CHUNK_FRAMEITEMS_2                 8767
#define CHUNK_EXEONLY                      8768
#define CHUNK_PROTECTION                   8770
#define CHUNK_SHADERS                      8771
#define CHUNK_APPHEADER2                   8773
#define CHUNK_FRAME                        13107
#define CHUNK_FRAMEHEADER                  13108
#define CHUNK_FRAMENAME                    13109
#define CHUNK_FRAMEPASSWORD                13110
#define CHUNK_FRAMEPALETTE                 13111
#define CHUNK_FRAMEITEMINSTANCES           13112
#define CHUNK_FRAMEFADEINFRAME             13113
#define CHUNK_FRAMEFADEOUTFRAME            13114
#define CHUNK_FRAMEFADEIN                  13115
#define CHUNK_FRAMEFADEOUT                 13116
#define CHUNK_FRAMEEVENTS                  13117
#define CHUNK_FRAMEPLAYHEADER              13118
#define CHUNK_ADDITIONAL_FRAMEITEM         13119
#define CHUNK_ADDITIONAL_FRAMEITEMINSTANCE 13120
#define CHUNK_FRAMELAYERS                  13121
#define CHUNK_FRAMEVIRTUALRECT             13122
#define CHUNK_DEMOFILEPATH                 13123
#define CHUNK_RANDOMSEED                   13124
#define CHUNK_FRAMELAYEREFFECTS            13125
#define CHUNK_BLURAYFRAMEOPTIONS           13126
#define CHUNK_MVTTIMERBASE                 13127
#define CHUNK_MOSAICIMAGETABLE             13128
#define CHUNK_FRAMEEFFECTS                 13129
#define CHUNK_FRAME_IPHONE_OPTIONS         13130
#define CHUNK_OBJINFOHEADER                17476
#define CHUNK_OBJINFONAME                  17477
#define CHUNK_OBJECTSCOMMON                17478
#define CHUNK_OBJECTUNKNOWN                17479
#define CHUNK_OBJECTEFFECTS                17480
#define CHUNK_IMAGESOFFSETS                21845
#define CHUNK_FONTSOFFSETS                 21846
#define CHUNK_SOUNDSOFFSETS                21847
#define CHUNK_MUSICSOFFSETS                21848
#define CHUNK_IMAGES                       26214
#define CHUNK_FONTS                        26215
#define CHUNK_SOUNDS                       26216
#define CHUNK_MUSICS                       26217
#define CHUNK_LAST                         32639
// end chunk ids }}}

// start chunk status codes {{{
#define CHUNK_ERR_NO_IMPL (-18)
#define CHUNK_OK 1879
#define CHUNK_ERR (-17087)
// end chunk status codes }}}


/* handle to chunk */
typedef struct {
    long fpos;
    short id;
    short flags;

    void* data;

    int inflated;  // if data has/needs to be decompressed
    int size;      // size of data in memory. may differ if inflated.
    int orig_size; // see above

    void* chunk_d; // ptr to a definite chunk
} chunk_h;

/* initializes a chunk. assumes fp is at the start of a chunk. loads the data
   of subchunks recursively. */
void init_chunk_h(FILE* fp, chunk_h* chunk);

void free_chunk_h(chunk_h* chunk);
