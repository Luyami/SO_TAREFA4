#include <parser.h>

void parse_command(char* input, char command[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN] , int* background, int* success){
    *success = 0;
    *background = 0;

    int argsCount = 0;

    char* token;
    int token_len;
    token = strtok(input, " "); //Getting command (first token)
    if (!token) return;

    token_len = strlen(token);

    if (token_len >= MAX_CMD_LEN) return;
    strcpy(command, token);
    
    while (token != NULL && argsCount < MAX_ARGS){
        token = strtok(NULL, " "); //Getting args
        if (!token) break;

        token_len = strlen(token);

        if (token_len >= MAX_ARG_LEN) return;

        if (strcmp(token, "&") == 0) *background = 1; //background checker
        else{ //is arg
            strcpy(args[argsCount], token);
            ++argsCount;
        }
    }

    *success = 1;
}