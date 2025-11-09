#ifndef SHELL_H
#define SHELL_H

#include <types.h>

extern void divideByZero();
extern void invalidOpcode();

#define MAX_ARGS 10

typedef struct {
    char *cmd1;
    char *args1[MAX_ARGS];
    char *cmd2;
    char *args2[MAX_ARGS];
    int hasPipe;
    int isBackground;
} ParsedCommand;


int commandMatch(char * str1, char * command, int count);

void analizeBuffer(char * buffer, int count);

pid_t launchShell();

#endif