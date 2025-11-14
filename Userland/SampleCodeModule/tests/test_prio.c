#include <stdint.h>
#include <stdio.h>
#include "test_util.h"
#include "syscall.h"
#include <uStrings.h>

#define TOTAL_PROCESSES 3

#define LOWEST 1
#define MEDIUM 3
#define HIGHEST 5

#define MINOR_WAIT 1000000
#define OBSERVE_MULTIPLIER 6

static int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

uint64_t test_prio(uint64_t argc, char *argv[]) {
  int64_t pids[TOTAL_PROCESSES] = {0};
  char waitStrs[TOTAL_PROCESSES][16] = {{0}};
  char *procArgs[TOTAL_PROCESSES][2] = {{0}};

  uint64_t factor = 1;
  if (argc == 1) {
    int64_t parsed = satoi(argv[0]);
    if (parsed > 0) {
      factor = (uint64_t)parsed;
    }
  }

  uint64_t waitTicks = MINOR_WAIT * factor;
  uint64_t observeTicks = waitTicks * OBSERVE_MULTIPLIER;

  printf("SPAWNING PROCESSES...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    intToStr((int)waitTicks, waitStrs[i]);
    procArgs[i][0] = waitStrs[i];
    procArgs[i][1] = NULL;
    pids[i] = my_create_process("endless_loop_print", 1, procArgs[i]);
    if (pids[i] < 0) {
      printf("testpriority: ERROR creating process %d\n", (int)i);
      return (uint64_t)-1;
    }
  }

  bussy_wait(observeTicks);

  printf("\nCHANGING PRIORITIES...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_nice(pids[i], prio[i]) != 0) {
      printf("testpriority: ERROR changing priority for %d\n", (int)pids[i]);
    }
  }

  bussy_wait(observeTicks);

  printf("\nBLOCKING...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_block(pids[i]) != 0) {
      printf("testpriority: ERROR blocking %d\n", (int)pids[i]);
    }
  }

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_nice(pids[i], MEDIUM) != 0) {
      printf("testpriority: ERROR reprioritizing %d\n", (int)pids[i]);
    }
  }

  printf("UNBLOCKING...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_unblock(pids[i]) != 0) {
      printf("testpriority: ERROR unblocking %d\n", (int)pids[i]);
    }
  }

  bussy_wait(observeTicks);
  bussy_wait(observeTicks);

  printf("\nKILLING...\n");
  for (uint64_t i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_kill(pids[i]) != 0) {
      printf("testpriority: ERROR killing %d\n", (int)pids[i]);
    }
  }

  bussy_wait(waitTicks);
  printf("DONE.\n");
  return 0;
}