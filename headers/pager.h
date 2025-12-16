#ifndef PAGER_H
#define PAGER_H

#define MAX_FRAMES 128

#include <util.h>

void pager_fifo(char* content, size_t content_size, int frames);
void pager_lru(char* content, size_t content_size, int frames);
void pager_opt(char* content, size_t content_size, int frames);
void pager_clock(char* content, size_t content_size, int frames);
void pager_nru(char* content, size_t content_size, int frames);

#endif