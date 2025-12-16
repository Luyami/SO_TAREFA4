#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
 
//Here CMD is the command, like exit, jobs... and ARGS are the arguments for the CMD

#define MAX_CMD_LEN 256
#define MAX_ARG_LEN 256
#define MAX_ARGS 32 
#define MAX_JOBS 10

static const char* INTERNAL_COMMANDS[] = {"exit", "iseven", "pid", "jobs", "wait", "pager", "internals"};
static const int INTERNAL_COMMANDS_SIZE = sizeof(INTERNAL_COMMANDS) / sizeof(INTERNAL_COMMANDS[0]);

#endif