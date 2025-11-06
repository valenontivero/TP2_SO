#ifndef SHELL_H
#define SHELL_H

#include <types.h>

extern void divideByZero();
extern void invalidOpcode();


int commandMatch(char * str1, char * command, int count);

void analizeBuffer(char * buffer, int count);

pid_t launchShell();

#endif