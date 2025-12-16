#include <commands.h>

pid_t bg_processes[MAX_JOBS] = {0};
int bg_count = 0;

void add_bg_process(pid_t pid){ 
    if (bg_count < 10){ 
        bg_processes[bg_count++] = pid; 
    } 
} 
 
void remove_bg_process(pid_t pid){
    for (int i = 0; i < bg_count; i++){
        if (bg_processes[i] == pid){
            printf("[%d]+ Done (PID %d)\n", i+1, pid);

            for (int j = i; j < bg_count - 1; j++){
                bg_processes[j] = bg_processes[j+1];
            }

            bg_processes[bg_count - 1] = 0; 
            bg_count--;

            return;
        }
    }
}

void clean_finished_processes(){ 
    int status; 
    pid_t pid; 
     
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0){ 
        for (int i = 0; i < bg_count; i++) { 
            if (bg_processes[i] == pid){ 
                printf("[%d]+ Done\n", i+1); 

                for (int j = i; j < bg_count - 1; j++){ 
                    bg_processes[j] = bg_processes[j+1]; 
                } 
                bg_count--; 
                break; 
            } 
        } 
    }
}

int is_internal_command(const char* req_cmd){
   int isInternal = 0;
   
   int i;
   for (i = 0; i < INTERNAL_COMMANDS_SIZE; ++i){
        const char* cmd = INTERNAL_COMMANDS[i];

        if (strcmp(cmd, req_cmd) == 0) {isInternal = 1; break;}
   }

   return isInternal;
}

void handle_internal_command(char* command, char args[MAX_ARGS][MAX_ARG_LEN]){
    int isInternal = is_internal_command(command);
    if (!isInternal) return;

    if (strcmp(command,"iseven") == 0){
        char* r; 
        int num = strtol(args[0], &r, 10);

        if (*r != '\0' || strcmp(args[0], "\0") == 0){
            printf("Invalid arguments for iseven command: I NEED AN INTEGER!\n");
        }
        else{
            isEven(num);
        }
    }

    else if (strcmp(command,"pid") == 0){
        showPID();
    }

    else if (strcmp(command,"pager") == 0){
        if (strcmp(args[0], "--algo") != 0) {printf("--algo flag missing!\n"); return;}
        if (strcmp(args[2], "--frames") != 0) {printf("--frames flag missing\n"); return;}
        if (strcmp(args[4], "--trace") != 0) {printf("--trace flag missing\n"); return;}
        
        char* r; 
        int num = strtol(args[3], &r, 10);

        if (*r != '\0' || strcmp(args[3], "\0") == 0){
            printf("Invalid arguments for pager command: Frames number must be an integer!\n");
            return;
        }

        pager(args[1], args[5], num);
    }

    else if (strcmp(command,"jobs") == 0){ 
        if (bg_count == 0){ 
            printf("Nenhum processo em background!\n"); 
        } else{ 
            printf("Processos em background:\n"); 
            for (int i = 0; i < bg_count; i++){ 
                printf("[%d] %d Running\n", i+1, bg_processes[i]); 
            } 
        } 
    } 

    else if (strcmp(command, "wait") == 0){ 
        printf("Aguardando processos em background...\n"); 
        while (bg_count > 0) { 
            int status; 
            pid_t pid = wait(&status);
            remove_bg_process(pid);
        } 
        printf("Todos os processos terminaram\n"); 
    }

    else if (strcmp(command, "internals") == 0){
        int i;
        for (i = 0; i < INTERNAL_COMMANDS_SIZE; ++i){
            const char* cmd = INTERNAL_COMMANDS[i];

            if (i < INTERNAL_COMMANDS_SIZE - 1) printf("%s ", cmd);
            else                                printf("%s\n", cmd);
        }
    }
}

void handle_external_command(char* command, char args[MAX_ARGS][MAX_ARG_LEN], int background){
    if (background && bg_count >= MAX_JOBS){
        printf("I can't handle any more jobs ): Sorry\n");
    }
    
    int pid = fork();

    if (pid == 0){
        //building argv
        int argc = 0;
        while (argc < MAX_ARGS && args[argc][0] != '\0') argc++;

        char* argv[argc + 2];
        argv[0] = command;
        for (int i = 0; i < argc; i++) argv[i + 1] = args[i];
        argv[argc + 1] = NULL;

        execvp(command, argv);
        exit(1); //something bad happened i guess ): 
    }
    else if (pid > 0){
        int status;
        if (!background) wait(&status);
        else{
            printf("[%d] %d\n", bg_count + 1, pid); 
            add_bg_process(pid);
        };
    }
    else{
        printf("Fork failed. Bring the spoons!\n");
    }
}

//Internal Commands
void isEven(int number){
    int result = number%2 == 0;

    if (result) printf("Yes, it's even!\n");
    else        printf("Uhh, it's odd...\n");
}

void showPID(){
    printf("PID: %d\n", getpid()); 
}

void pager(char alg[MAX_ARG_LEN], char file_path[MAX_ARG_LEN], int frames){
    size_t f_size;
    char* content = read_file(file_path, &f_size);

    if (!content) {printf("Pager: Could not read specified file!\n"); return;}

    if (strcmp(alg, "fifo") == 0){
        pager_fifo(content, f_size, frames);
    } else if (strcmp(alg, "lru") == 0){
        pager_lru(content, f_size, frames);
    } else if (strcmp(alg, "opt") == 0){
        pager_opt(content, f_size, frames);
    } else if (strcmp(alg, "clock") == 0){
        pager_clock(content, f_size, frames);
    } else if (strcmp(alg, "nru") == 0){
        pager_nru(content, f_size, frames);
    }
    else printf("Pager: Specified algorithm is not available!\n");
}