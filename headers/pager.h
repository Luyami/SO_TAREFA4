#ifndef PAGER_H
#define PAGER_H

#define MAX_FRAMES 128

#include <util.h>


pagination_stats pager_fifo(char* content, size_t content_size, int frames);
pagination_stats pager_lru(char* content, size_t content_size, int frames);
pagination_stats pager_opt(char* content, size_t content_size, int frames);
pagination_stats pager_clock(char* content, size_t content_size, int frames);
pagination_stats pager_nru(char* content, size_t content_size, int frames);
pagination_stats pager_lfu(char* content, size_t content_size, int frames);
pagination_stats pager_mfu(char* content, size_t content_size, int frames);
void pager_all(char* content, size_t content_size);

#endif