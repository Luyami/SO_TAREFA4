#include <pager.h>
#include <stdio.h>

static void print_stats(const char* name, int frames, int refs, int pageFaults, int evictions){
    printf("Algoritmo: %s\n", name);
    printf("Frames: %i\n", frames);
    printf("Referências: %i\n", refs);
    printf("Falta de Páginas: %i\n", pageFaults);
    printf("Taxa de Faltas: %f\n", refs ? (float)pageFaults/refs : 0.0f);
    printf("Evicções: %i\n", evictions);
}

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

    print_stats("FIFO", frames, refs, pageFaults, evictions);
}

void pager_lru(char* content, size_t content_size, int frames){
    if (frames > MAX_FRAMES) {printf("Pager: WE DON'T HAVE THAT MUCH FRAMES!\n"); return;}

    //array init
    char f_a[MAX_FRAMES] = {-1};
    int f_a_t[MAX_FRAMES] = {0}; //time tracker
    int f_a_q = 0; //active frames
    int time = 0;

    char *p = content;
    char *end;

    int pageFaults = 0;
    int evictions = 0;
    int refs = 0;

    while (*p != '\0') {
        long f = strtol(p, &end, 10);

        if (p != end){
            ++refs;
            ++time;

            int isActive = isFrameActive(f, f_a, 0, f_a_q - 1);

            if (isActive == 0){ //page fault (not active)
                ++pageFaults;

                if (f_a_q == frames){ //array is full
                    ++evictions;

                    //find LRU
                    int lru_idx = 0;
                    int lru_time = f_a_t[0];

                    for (int i = 1; i < frames; ++i){
                        if (f_a_t[i] < lru_time){
                            lru_time = f_a_t[i];
                            lru_idx = i;
                        }
                    }

                    f_a[lru_idx] = f;
                    f_a_t[lru_idx] = time;
                }
                else{
                    f_a[f_a_q] = f;
                    f_a_t[f_a_q] = time;
                    ++f_a_q;
                }
            }
            else{ //update time
                for (int i = 0; i < f_a_q; ++i){
                    if (f_a[i] == f){
                        f_a_t[i] = time;
                        break;
                    }
                }
            }

            p = end;
        } 
        else{
            p++;
        }
    }

    print_stats("LRU", frames, refs, pageFaults, evictions);
}

void pager_opt(char* content, size_t content_size, int frames){
    if (frames > MAX_FRAMES) {printf("Pager: WE DON'T HAVE THAT MUCH FRAMES!\n"); return;}

    //array init
    char f_a[MAX_FRAMES] = {-1};
    int f_a_q = 0; //active frames

    char *p = content;
    char *end;

    int pageFaults = 0;
    int evictions = 0;
    int refs = 0;

    //store all references in an array for future use
    long references[content_size];
    int ref_count = 0;

    char *temp_p = content;
    char *temp_end;

    while (*temp_p != '\0') {
        long f = strtol(temp_p, &temp_end, 10);

        if (temp_p != temp_end){
            references[ref_count++] = f;
            temp_p = temp_end;
        } 
        else{
            temp_p++;
        }
    }

    int current_ref_idx = 0;

    while (*p != '\0') {
        long f = strtol(p, &end, 10);

        if (p != end){
            ++refs;

            int isActive = isFrameActive(f, f_a, 0, f_a_q - 1);

            if (isActive == 0){ //page fault (not active)
                ++pageFaults;

                if (f_a_q == frames){ //array is full
                    ++evictions;

                    //find OPT
                    int opt_idx = -1;
                    int opt_distance = -1;

                    for (int i = 0; i < frames; ++i){
                        int found_in_future = 0;
                        for (int j = current_ref_idx + 1; j < ref_count; ++j){
                            if (f_a[i] == references[j]){
                                found_in_future = 1;
                                int distance = j - current_ref_idx;
                                if (distance > opt_distance){
                                    opt_distance = distance;
                                    opt_idx = i;
                                }
                                break;
                            }
                        }
                        if (!found_in_future){ //not found in future refs, so we can evict this one
                            opt_idx = i;
                            break;
                        }
                    }

                    f_a[opt_idx] = f;
                }
                else{
                    f_a[f_a_q] = f;
                    ++f_a_q;
                }
            }
            ++current_ref_idx;
            p = end;
        }
        else{
            p++;
        }
    }
    
    print_stats("OPT", frames, refs, pageFaults, evictions);
}



void pager_clock(char* content, size_t content_size, int frames){
    if (frames > MAX_FRAMES) {printf("Pager: WE DON'T HAVE THAT MUCH FRAMES!\n"); return;}

    //array init
    char f_a[MAX_FRAMES] = {-1};
    int f_a_r[MAX_FRAMES] = {1}; //reference bits
    int f_a_q = 0; //active frames
    int clock_hand = 0;

    char *p = content;
    char *end;

    int pageFaults = 0;
    int evictions = 0;
    int refs = 0;

    while (*p != '\0') {
        long f = strtol(p, &end, 10);
        // printf("Referência à página %ld\n", f);
        // printf("Estado dos frames: ");
        // for (int i = 0; i < f_a_q; ++i){
        //     printf("%d(r=%d) ", f_a[i], f_a_r[i]);
        // }
        // printf("\n\n");

        if (p != end){
            ++refs;

            int isActive = isFrameActive(f, f_a, 0, f_a_q - 1);

            if (isActive == 0){ //page fault (not active)
                ++pageFaults;

                if (f_a_q == frames){ //array is full
                    ++evictions;

                    //find frame to evict
                    while (1){
                        if (f_a_r[clock_hand] == 0){ // reference bit is 0,
                            //evict this one
                            f_a[clock_hand] = f;
                            f_a_r[clock_hand] = 1;
                            clock_hand = (clock_hand + 1) % frames; //move clock hand 0 -> frames-1 for next time
                            break;
                        } else {
                            f_a_r[clock_hand] = 0;
                            clock_hand = (clock_hand + 1) % frames; //move clock hand 0 -> frames-1 for next time
                        }
                    }
                }
                else{ // there is still space
                    f_a[f_a_q] = f;
                    f_a_r[f_a_q] = 1;
                    ++f_a_q;
                }
            }
            else{ //set reference bit
                for (int i = 0; i < f_a_q; ++i){
                    if (f_a[i] == f){
                        f_a_r[i] = 1;
                        break;
                    }
                }
            }

            p = end;
        } 
        else{
            p++;
        }
    }

    print_stats("CLOCK", frames, refs, pageFaults, evictions);
}

void pager_nru(char* content, size_t content_size, int frames){
    if (frames > MAX_FRAMES) {printf("Pager: WE DON'T HAVE THAT MUCH FRAMES!\n"); return;}

    //array init
    char f_a[MAX_FRAMES] = {-1};
    int f_a_r[MAX_FRAMES] = {0}; //reference bits (R)
    int f_a_m[MAX_FRAMES] = {0}; //modified bits (M)
    int f_a_q = 0; //active frames

    char *p = content;
    char *end;

    int pageFaults = 0;
    int evictions = 0;
    int refs = 0;

    int clock_counter = 0; //to reset R bits periodically
    const int CLOCK_TICK_RATE = 5; //reset R bits every 5 references

    while (*p != '\0') {
        long f = strtol(p, &end, 10);
        printf("Referência à página %ld\n", f);
         

        if (p != end){
            ++refs;
            ++clock_counter;
            if (clock_counter >= CLOCK_TICK_RATE){
                //reset R bits
                for (int i = 0; i < f_a_q; ++i){
                    f_a_r[i] = 0;
                }
                clock_counter = 0;
            }

            int isActive = isFrameActive(f, f_a, 0, f_a_q - 1);

            if (isActive == 0){ //page fault (not active)
                ++pageFaults;

                if (f_a_q == frames){ //array is full
                    ++evictions;

                    //find NRU
                    int nru_idx = -1;

                    //classes 0 to 3
                    // 0: R=0, M=0
                    // 1: R=0, M=1
                    // 2: R=1, M=0
                    // 3: R=1, M=1
                    for (int class = 0; class <=3; ++class){
                         for (int i = 0; i < frames; ++i){
                            if (f_a_r[i] == (class / 2) && f_a_m[i] == (class % 2)){
                                nru_idx = i;
                                break;
                            }
                        }
                        if (nru_idx != -1) break; //found a page to evict
                    }

                    f_a[nru_idx] = f;
                    f_a_r[nru_idx] = 1; //reset reference bit
                    f_a_m[nru_idx] = 0; //reset modified bit
                }
                else{
                    f_a[f_a_q] = f;
                    f_a_r[f_a_q] = 1;
                    f_a_m[f_a_q] = 0;
                    ++f_a_q;
                }
            }
            else{ //set reference bit
                for (int i = 0; i < f_a_q; ++i){
                    if (f_a[i] == f){
                        f_a_r[i] = 1;
                        // bit M is not modified in this simulation
                        break;
                    }
                }
            }

            p = end;
        } 
        else{
            p++;
        }
        printf("Estado dos frames: ");
        for (int i = 0; i < f_a_q; ++i){
            printf("%d(r=%d,m=%d) ", f_a[i], f_a_r[i], f_a_m[i]);
        }
        printf("\n\n"); 
    }

    print_stats("NRU", frames, refs, pageFaults, evictions);
}