#pragma once
#define KEY_SIZE_DW         258
#define DECODE_ERR_FATAL    (-6539)
#define DECODE_ERR_UNKNOWN  (-740)  // hash not found in table
#define DECODE_ERR_MISMATCH (-871)  // hash after was mismatched
#define DECODE_OK           0

void get_decode_key(int* out);

void decode_buf(int* key, void* buf, int size);

int verify_decode(unsigned int hash_before, unsigned int hash_after);
