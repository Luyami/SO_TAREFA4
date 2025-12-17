#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 128

typedef struct pagination_stats{
    const char* name;
    int frames;
    int refs;
    int pageFaults;
    int evictions;
    int activeFrames[MAX_FRAMES];
} pagination_stats;

char* read_file(const char* path, size_t* out_size);
void print_stats(pagination_stats stats);

#endif