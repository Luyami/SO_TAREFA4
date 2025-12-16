#include <commands.h>

int main(){
    char input[MAX_CMD_LEN + MAX_ARGS * MAX_ARG_LEN] = {0}; 
    char command[MAX_CMD_LEN] = {0};
    char args[MAX_ARGS][MAX_ARG_LEN] = {0};
    int background;  
    int success;

    printf("lu-Shell iniciado (PID: %d)\n", getpid()); 
    printf("Digite 'exit' para sair e 'internals' para lista de comandos internos\n\n"); 

    while (1){ 
        clean_finished_processes();

        printf("lushell> "); 
        fflush(stdout); 

        if (fgets(input, sizeof(input), stdin) != NULL){
            size_t len = strlen(input);
            //Removing '\n'
            if (len > 0 && input[len - 1] == '\n'){
                input[len - 1] = '\0';
            }
        }

        parse_command(input, command, args, &background, &success);

        if (success){ //Valid command format
            int internal_command = is_internal_command(command);
            
            if (internal_command){ //Internal Command
                if (strcmp(input, "exit") == 0) break; //Exit shell

                //If its a more complex command
                handle_internal_command(command, args);
            }
            else{ //External Command
                handle_external_command(command, args, background);
            }
        }   

        //Cleaning arrays for next inputs
        memset(command, 0, sizeof(command));
        memset(args, 0, sizeof(args));
    }   
    
    return 0;
}