#include "key.h"

/* this file is full of cursed code. be warned. */


/* ok, hear me out.
 *
 * after loading a chunk's body, if it has the first bit of its flags set, we
 * manipulate the chunk body by:
 * 1. creating a new 1032 byte buffer (possibly a byte or two less)
 * 2. fill this buffer with a hash based on something at [[base+0xb39cc]+0x26c]
 *    where base is the image base of the exe, [.] is a ptr dereference
 *    operator and [base+0xb39cc] seems to be a struct containing vital game
 *    information, including, but not limited to, the game name, path to exe,
 *    copyright, etc.
 * 3. using this hash buffer, manipulate the buffer
 *
 * since this buffer is used to essentially decode the chunk body, i figured
 * it had to be constant. hooking onto the function to populate the hash
 * buffer, calling it, and hashing the hash buffer into an int and repeating
 * after re-opening the game confirmed this.
 *
 * i'm too lazy to figure out where the hash dependencies come from, because
 * a reference search with Ghidra doesn't show any immediate results, but i
 * see from Anaconda that the "editor", "name", and "copyright" are likely used.
 *
 * if trying to reverse other ctf25 games, this has to be revisited lmao
 */
int decode_key[KEY_SIZE_DW] =
{
    208,
    28,
    49,
    157,
    250,
    55,
    39,
    25,
    35,
    160,
    128,
    101,
    43,
    89,
    165,
    103,
    150,
    121,
    245,
    34,
    85,
    65,
    139,
    38,
    45,
    33,
    37,
    40,
    193,
    22,
    19,
    26,
    151,
    166,
    118,
    164,
    47,
    199,
    152,
    94,
    15,
    248,
    64,
    227,
    190,
    192,
    175,
    133,
    129,
    77,
    220,
    255,
    187,
    167,
    156,
    30,
    54,
    117,
    146,
    81,
    100,
    41,
    168,
    236,
    97,
    179,
    66,
    217,
    126,
    223,
    197,
    68,
    56,
    73,
    83,
    240,
    153,
    52,
    214,
    106,
    71,
    113,
    169,
    244,
    23,
    127,
    67,
    46,
    170,
    241,
    186,
    130,
    105,
    116,
    226,
    188,
    237,
    27,
    249,
    189,
    36,
    210,
    176,
    112,
    13,
    114,
    29,
    17,
    74,
    242,
    180,
    59,
    145,
    80,
    184,
    123,
    115,
    231,
    92,
    185,
    159,
    194,
    143,
    219,
    120,
    7,
    154,
    132,
    5,
    122,
    110,
    235,
    216,
    234,
    158,
    58,
    161,
    155,
    228,
    224,
    181,
    84,
    96,
    171,
    218,
    142,
    79,
    87,
    149,
    21,
    53,
    1,
    76,
    191,
    200,
    203,
    91,
    137,
    182,
    212,
    222,
    14,
    254,
    63,
    44,
    61,
    51,
    88,
    124,
    102,
    32,
    211,
    183,
    99,
    78,
    138,
    50,
    178,
    119,
    140,
    24,
    9,
    163,
    162,
    205,
    238,
    198,
    196,
    251,
    90,
    108,
    135,
    72,
    230,
    144,
    173,
    20,
    202,
    177,
    207,
    0,
    107,
    246,
    48,
    2,
    104,
    195,
    69,
    147,
    31,
    243,
    141,
    6,
    213,
    95,
    252,
    62,
    253,
    93,
    221,
    42,
    109,
    247,
    11,
    16,
    70,
    172,
    233,
    174,
    98,
    204,
    12,
    225,
    57,
    8,
    148,
    4,
    125,
    232,
    18,
    10,
    75,
    3,
    215,
    209,
    134,
    201,
    111,
    229,
    206,
    239,
    86,
    131,
    82,
    60,
    136,
    0,
    0,
};

int* get_decode_key() { return decode_key; }


/*
 * i couldn't get the Ghidra decompiled output to compile back into the right
 * assembly. fuck it, we rawdogging this function
 *
 * at base+46d040
 */
__attribute__((naked,cdecl)) void decode_buf(int* key, void* buf, int size) {
    asm
    (
        "push   %ebp\n\t"
        "mov    %ebp,%esp\n\t"
        "mov    %eax, 0x8(%ebp)\n\t"
        "push   %ebx\n\t"
        "mov    %ebx, 0x400(%eax)\n\t"
        "mov    %ecx, 0x404(%eax)\n\t"
        "push   %edi\n\t"
        "xor    %edi,%edi\n\t"
        "cmp    0x10(%ebp),%edi\n\t"
        "jle    LAB_0046d0a5\n\t"
        "push   %esi\n\t"
        "jmp    LAB_0046d063\n\t"
        "mov    %edi,%edi\n\t"
        "LAB_0046d060:\n\t"
        "mov    %ecx, 0x8(%ebp)\n\t"
        "LAB_0046d063:\n\t"
        "inc    %bl\n\t"
        "movzx  %bl,%ebx\n\t"
        "inc    %edi\n\t"
        "mov    %edx, (%eax,%ebx,4)\n\t"
        "add    %cl,%dl\n\t"
        "movzx  %cl,%ecx\n\t"
        "shl    $0x38,%ecx\n\t"
        "shr    $0x38,%ecx\n\t"
        "mov    0x8(%ebp),%ecx\n\t"
        "mov    %ecx, (%eax,%ecx,4)\n\t"
        "mov    %esi, 0x8(%ebp)\n\t"
        "mov    (%eax,%ebx,4),%ecx\n\t"
        "add    %cl,%dl\n\t"
        "movzx  %cl,%ecx\n\t"
        "mov    (%eax,%esi,4),%edx\n\t"
        "mov    %edx, 0xc(%ebp)\n\t"
        "mov    %cl, (%eax,%ecx,4)\n\t"
        "xor    -0x1(%edi,%edx,1),%cl\n\t"
        "cmp    %edi, 0x10(%ebp)\n\t"
        "jl     LAB_0046d060\n\t"
        "mov    0x404(%eax),%esi\n\t"
        "pop    %esi\n\t"
        "pop    %edi\n\t"
        "mov    0x400(%eax),%ebx\n\t"
        "pop    %ebx\n\t"
        "pop    %ebp\n\t"
        "ret\n\t"
        "LAB_0046d0a5:\n\t"
        "pop    %edi\n\t"
        "mov    0x400(%eax),%ebx\n\t"
        "mov    0x404(%eax),%ecx\n\t"
        "pop    %ebx\n\t"
        "pop    %ebp\n\t"
        "ret\n\t"
    );
}

