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

void print_stats(pagination_stats stats){
    printf("Algoritmo: %s\n", stats.name);
    printf("Frames: %i\n", stats.frames);
    printf("Referências: %i\n", stats.refs);
    printf("Falta de Páginas: %i\n", stats.pageFaults);
    float faultRate = stats.refs ? (float)stats.pageFaults / stats.refs : 0.0f;
    printf("Taxa de Faltas: %.2f%%\n", faultRate * 100.0f);
    printf("Evicções: %i\n", stats.evictions);
    printf("frames_ids: ");
    for (int i = 0; i < stats.frames; ++i){
        printf("%d ", i);
    }
    printf("\n");
    printf("pages_ids: ");
    for (int i = 0; i < stats.frames; ++i){
        printf("%d ", stats.activeFrames[i]);
    }
    printf("\n");
}