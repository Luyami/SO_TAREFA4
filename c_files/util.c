#include <util.h>

char* read_file(const char* path, size_t* out_size){
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    if (size < 0) { fclose(f); return NULL; }
    rewind(f);

    char* data = malloc(size + 1);   //+1 for '\0'
    if (!data) { fclose(f); return NULL; }

    fread(data, 1, size, f);
    data[size] = '\0';

    fclose(f);

    if (out_size) *out_size = size;
    return data;
}