#ifndef PARSER_H
#define PARSER_H

#include "definitions.h"

//out args, out command, out background
void parse_command(char* input, char command[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN], int* background, int* success);

#endif