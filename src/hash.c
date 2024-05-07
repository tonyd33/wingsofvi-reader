unsigned int hash_buf(const char* s, unsigned int len) {
    unsigned int h = 0;
    while (len) {
        h = 31 * h + *s++;
        --len;
    }
    return h;
}

