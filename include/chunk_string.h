typedef struct {
    int size; // SIZE, not LENGTH. length would be size / 2 because wchar
    wchar_t* value;
} chunk_string_h;


void init_chunk_string_h(FILE* fp, chunk_string_h* chunk_string);

void free_chunk_string_h(chunk_string_h* chunk_string);
