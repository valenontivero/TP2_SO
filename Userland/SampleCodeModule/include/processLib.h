#ifndef PROCESS_LIB_H
#define PROCESS_LIB_H
#include <types.h>

#define DEFAULT_PRIO 1

pid_t createProcess(void (*entryPoint)(uint8_t, char **), uint8_t prio ,uint8_t argc, char** argv);

void putInFg(pid_t pid1, pid_t pid2);

#endif