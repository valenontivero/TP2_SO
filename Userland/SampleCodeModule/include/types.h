#ifndef __TYPES_H__
#define __TYPES_H__
#include <stdint.h>


typedef uint16_t pid_t; // Has to match with the one in processManager.h

#define PROCESS_MAX_NAME_LEN 40
#define MAX_PROCESSES 256
#define PRIORITY_LEVELS 8

typedef enum{
    READY = 0,
	RUNNING,
	BLOCKED,
	TERMINATED 
} State;

typedef struct memoryData {
    uint32_t free;
    uint32_t used;
    uint32_t total;
} memoryData;

typedef struct processInfo {
    char name[PROCESS_MAX_NAME_LEN];
    pid_t pid;
    State state;
    uint8_t priority;
    uint8_t foreground;
    void* stackBase;
    void* stackPointer;
} processInfo;

#endif