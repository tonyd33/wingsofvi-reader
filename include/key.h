#pragma once
#define KEY_SIZE_DW 258

int* get_decode_key();

void decode_buf(int* key, void* buf, int size);
