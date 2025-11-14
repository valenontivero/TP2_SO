#ifndef TEST_SYSCALL_WRAPPERS_H
#define TEST_SYSCALL_WRAPPERS_H

#include <stdint.h>

int64_t my_create_process(const char *name, uint64_t argc, char *argv[]);
int64_t my_kill(int64_t pid);
int64_t my_block(int64_t pid);
int64_t my_unblock(int64_t pid);
int64_t my_nice(int64_t pid, uint64_t priority);
int64_t my_wait(int64_t pid);
int64_t my_getpid(void);

int64_t my_sem_open(const char *name, uint64_t initial_value);
int64_t my_sem_wait(const char *name);
int64_t my_sem_post(const char *name);
int64_t my_sem_close(const char *name);

void my_yield(void);

#endif

