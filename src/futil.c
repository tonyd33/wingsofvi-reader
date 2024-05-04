#include <stdio.h>

size_t fpeek(void* ptr, size_t size, size_t nmemb, FILE* fp) {
    long pos = ftell(fp);
    size_t ret = fread(ptr, size, nmemb, fp);
    fseek(fp, pos, SEEK_SET);
    return ret;
}

long fsize(FILE* fp) {
    long curr = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, curr, SEEK_SET);
    return size;
}
