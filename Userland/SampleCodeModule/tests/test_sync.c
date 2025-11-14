#include <stdint.h>
#include <stdio.h>
#include "syscall.h"
#include "test_util.h"

#define SEM_ID "sem"
int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!my_sem_open(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  if (use_sem)
    my_sem_close(SEM_ID);

  return 0;
}

static uint64_t run_test_sync(uint64_t argc, char *argv[], uint8_t useSemaphore) {
  if (argc != 2)
    return -1;

  int64_t pairCount = satoi(argv[0]);
  int64_t iterations = satoi(argv[1]);

  if (pairCount <= 0 || iterations <= 0)
    return -1;

  uint64_t totalProcesses = (uint64_t)pairCount * 2;
  uint64_t pids[totalProcesses];

  char *iterationsStr = argv[1];
  char useSemStr[2] = {useSemaphore ? '1' : '0', '\0'};

  char *argvDec[] = {iterationsStr, "-1", useSemStr, NULL};
  char *argvInc[] = {iterationsStr, "1", useSemStr, NULL};

  global = 0;

  for (uint64_t i = 0; i < (uint64_t)pairCount; i++) {
    pids[i] = my_create_process("my_process_inc", 3, argvDec);
    pids[i + pairCount] = my_create_process("my_process_inc", 3, argvInc);
  }

  for (uint64_t i = 0; i < (uint64_t)pairCount; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + pairCount]);
  }

  printf("Final value: %d\n", global);
  return 0;
}

uint64_t test_synchro(uint64_t argc, char *argv[]) {
  return run_test_sync(argc, argv, 1);
}

uint64_t test_no_synchro(uint64_t argc, char *argv[]) {
  return run_test_sync(argc, argv, 0);
}