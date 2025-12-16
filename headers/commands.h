#ifndef COMMANDS_H
#define COMMANDS_H

#include "parser.h"
#include <stdio.h>
#include <pager.h>
#include <util.h>

extern pid_t bg_processes[10]; //0 for end of array
extern int bg_count;

void add_bg_process(pid_t pid);
void remove_bg_process(pid_t pid);
void clean_finished_processes();

int is_internal_command(const char* req_cmd);

void handle_internal_command(char* command, char args[MAX_ARGS][MAX_ARG_LEN]);
void handle_external_command(char* command, char args[MAX_ARGS][MAX_ARG_LEN], int background);

//Internal Commands
void isEven(int number);
void showPID();
void pager(char alg[MAX_ARG_LEN], char file_path[MAX_ARG_LEN], int frames);

#endif