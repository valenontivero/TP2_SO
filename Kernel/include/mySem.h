#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "PCBQueueADT.h"
#include <processManager.h>

#define MAX_SEMAPHORES 32
#define MAX_NAME_LEN 32

void _cli(void);
void _sti(void);

typedef volatile int8_t spinlock_t;

typedef struct {
    int value;
    PCBQueueADT waiters;
    spinlock_t lock;  
} Semaphore;

typedef struct {
    int inUse;
    char name[MAX_NAME_LEN];
    Semaphore sem;
    spinlock_t lock;
    uint8_t refCount;  // Contador de referencias para semáforos compartidos
} NamedSemaphore;

uint8_t sem_init(Semaphore* sem, uint8_t initial_value);
uint8_t sem_open(const char* name, uint8_t initial_value);
int sem_wait(uint8_t id);
int sem_post(uint8_t id);
void sem_destroy(uint8_t id);
void releaseHeldSemaphores(PCB* process);
int sem_unregister_waiting_process(uint8_t id, PCB *process);
void sem_decrement_ref_count(uint8_t id);

#endif