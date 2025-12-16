#include <pager.h>

int isFrameActive(char target, char* activeFrames, int start, int end){
    if (start < 0 || end < 0) return 0;

    char c = start;

    while (c < end + 1){
        if (activeFrames[c] == target) return 1;
        
        ++c;
    }

    return 0;
}

void pager_fifo(char* content, size_t content_size, int frames){
    if (frames > MAX_FRAMES) {printf("Pager: WE DON'T HAVE THAT MUCH FRAMES!\n"); return;}

    //queue init
    char f_q[MAX_FRAMES] = {-1};
    int f_q_s = 0; //start idx
    int f_q_e = -1; //end idx
    int f_q_q = 0; //active frames

    char *p = content;
    char *end;

    int pageFaults = 0;
    int evictions = 0;
    int refs = 0;

    while (*p != '\0') {
        long f = strtol(p, &end, 10);

        if (p != end){
            ++refs;

            int isActive = isFrameActive(f, f_q, f_q_s, f_q_e);

            if (isActive == 0){ //page fault (not active)
                ++pageFaults;

                if (f_q_e == MAX_FRAMES - 1){ //queue going out of bounds, so we must realloc elements (:
                    for (int i = 0; i < frames; ++i) f_q[i] = f_q[f_q_s + i];

                    f_q_s = 0;
                    f_q_e = frames - 1;
                }

                if (f_q_q == frames){ //queue is full
                    ++evictions;

                    ++f_q_s;
                    ++f_q_e;
                    f_q[f_q_e] = f;
                }
                else{
                    ++f_q_e;
                    ++f_q_q;

                    f_q[f_q_e] = f;
                }
            }

            p = end;
        } 
        else{
            p++;
        }
    }

    printf("Algoritmo: FIFO\n");
    printf("Frames: %i\n", frames);
    printf("Referências: %i\n", refs);
    printf("Falta de Páginas: %i\n", pageFaults);
    printf("Taxa de Faltas: %f\n", (float)pageFaults/refs);
    printf("Evicções: %i\n", evictions);
}