// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"

#include <processLib.h>
#include <stdint.h>
#include <types.h>
#include <userio.h>
#include <usyscalls.h>
#include <stddef.h>

#define SEM_NAME "sync_sem"

static volatile int32_t global_value = 0;
static volatile uint64_t worker_iterations = 0;
static volatile uint8_t worker_use_sem = 0;

static char *const INC_ARGS[] = {"1", NULL};
static char *const DEC_ARGS[] = {"-1", NULL};

static void slowInc(volatile int32_t *p, int32_t inc) {
  int32_t aux = *p;
  // Voluntary yield to increase race probability
  sys_yield();
  aux += inc;
  *p = aux;
}

static void run_worker(int32_t delta) {
  int64_t semId = -1;
  if (worker_use_sem) {
    semId = sys_sem_open(SEM_NAME, 1);
    if (semId == (int64_t)-1) {
      printf("test_sync: ERROR opening semaphore\n");
      return;
    }
  }

  for (uint64_t i = 0; i < worker_iterations; i++) {
    if (worker_use_sem)
      sys_sem_wait((uint8_t)semId);
    slowInc(&global_value, delta);
    if (worker_use_sem)
      sys_sem_post((uint8_t)semId);
  }

  if (worker_use_sem)
    sys_sem_close((uint8_t)semId);
}

static void race_worker_entry(uint8_t argc, char **argv) {
  (void)argc;
  if (argv == NULL || argv[0] == NULL) {
    run_worker(1);
    return;
  }
  int32_t delta = (int32_t)satoi(argv[0]);
  if (delta == 0)
    delta = 1;
  run_worker(delta);
}

static int spawn_worker(char *const *args, pid_t *pid) {
  uint64_t raw = sys_launch_process((void *)race_worker_entry, DEFAULT_PRIO, 1, (char **)args);
  if (raw == (uint64_t)-1) {
    return -1;
  }
  *pid = (pid_t)raw;
  // Workers must not take the terminal
  sys_process_set_foreground(*pid, 0);
  return 0;
}

static uint64_t run_sync_test(uint64_t argc, char *argv[], uint8_t use_sem) {
  if (argc != 2)
    return -1;

  int64_t pairCount = satoi(argv[0]);
  int64_t iterations = satoi(argv[1]);

  if (pairCount <= 0 || iterations <= 0) {
    return -1;
  }

  if (pairCount * 2 >= MAX_PROCESSES) {
    printf("test_sync: too many processes requested.\n");
    return -1;
  }

  uint64_t total = (uint64_t)pairCount * 2;
  pid_t *pids = (pid_t *)sys_malloc(sizeof(pid_t) * total);
  if (pids == NULL) {
    printf("test_sync: unable to allocate pid array.\n");
    return -1;
  }

  worker_iterations = (uint64_t)iterations;
  worker_use_sem = use_sem;
  global_value = 0;

  for (int64_t i = 0; i < pairCount; i++) {
    if (spawn_worker(DEC_ARGS, &pids[i]) != 0 || spawn_worker(INC_ARGS, &pids[i + pairCount]) != 0) {
      printf("test_sync: ERROR creating worker process\n");
      sys_free(pids);
      return -1;
    }
  }

  for (uint64_t i = 0; i < total; i++) {
    sys_wait(pids[i]);
  }

  printf("Final value: %d\n", global_value);
  sys_free(pids);
  return 0;
}

uint64_t test_synchro(uint64_t argc, char *argv[]) {
  return run_sync_test(argc, argv, 1);
}

uint64_t test_no_synchro(uint64_t argc, char *argv[]) {
  return run_sync_test(argc, argv, 0);
}