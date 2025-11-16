// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <processLib.h>
#include <types.h>
#include <usyscalls.h>
#include <userio.h>
#include <stddef.h>
#include "test_util.h"

#define TOTAL_PROCESSES 3
#define LOWEST 1
#define MEDIUM 3
#define HIGHEST 5

static int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};
static volatile uint64_t max_value = 0;

static void zero_to_max() {
  uint64_t value = 0;
  while (value++ != max_value)
    ;
  printf("PROCESS %d DONE!\n", (int)sys_get_pid());
}

static void zero_to_max_entry(uint8_t argc, char **argv) {
  (void)argc;
  (void)argv;
  zero_to_max();
}

static int spawn_zero_to_max(pid_t *pid) {
  static char name[] = "zero_to_max";
  char *argv[] = {name, NULL};
  uint64_t raw = sys_launch_process((void *)zero_to_max_entry, DEFAULT_PRIO, 1, argv);
  if (raw == (uint64_t)-1) {
    return -1;
  }
  *pid = (pid_t)raw;
  // Keep computation-only children out of the foreground
  sys_process_set_foreground(*pid, 0);
  return 0;
}

static void wait_all(pid_t *pids) {
  for (int i = 0; i < TOTAL_PROCESSES; i++) {
    sys_wait(pids[i]);
  }
}

uint64_t test_prio(uint64_t argc, char *argv[]) {
  if (argc != 1) {
    printf("test_priority: invalid argument count (%d)\n", (int)argc);
    return -1;
  }

  if (argv == NULL || argv[0] == NULL) {
    printf("test_priority: missing limit argument (argv[0] is NULL)\n");
    return -1;
  }

  max_value = satoi(argv[0]);
  if ((int64_t)max_value <= 0) {
    printf("test_priority: limit must be > 0 (was %d)\n", (int)max_value);
    return -1;
  }

  printf("SAME PRIORITY...\n");

  pid_t pids[TOTAL_PROCESSES];
  for (int i = 0; i < TOTAL_PROCESSES; i++) {
    if (spawn_zero_to_max(&pids[i]) != 0) {
      printf("test_priority: ERROR creating process\n");
      return -1;
    }
  }

  wait_all(pids);

  printf("SAME PRIORITY, THEN CHANGE IT...\n");
  for (int i = 0; i < TOTAL_PROCESSES; i++) {
    if (spawn_zero_to_max(&pids[i]) != 0) {
      printf("test_priority: ERROR creating process\n");
      return -1;
    }
    sys_process_nice(pids[i], prio[i]);
    printf("  PROCESS %d NEW PRIORITY: %d\n", pids[i], prio[i]);
  }

  wait_all(pids);

  printf("SAME PRIORITY, THEN CHANGE IT WHILE BLOCKED...\n");
  for (int i = 0; i < TOTAL_PROCESSES; i++) {
    if (spawn_zero_to_max(&pids[i]) != 0) {
      printf("test_priority: ERROR creating process\n");
      return -1;
    }
    sys_process_block(pids[i]);
    sys_process_nice(pids[i], prio[i]);
    printf("  PROCESS %d NEW PRIORITY: %d\n", pids[i], prio[i]);
  }

  for (int i = 0; i < TOTAL_PROCESSES; i++) {
    sys_process_unblock(pids[i]);
  }

  wait_all(pids);
  return 0;
}