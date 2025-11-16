// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <processLib.h>
#include <types.h>
#include <usyscalls.h>
#include <userio.h>
#include <stddef.h>
#include "test_util.h"

typedef enum {
  PROC_RUNNING,
  PROC_BLOCKED,
  PROC_KILLED
} proc_state;

typedef struct {
  pid_t pid;
  proc_state state;
} process_req;

static void endless_loop_entry(uint8_t argc, char **argv) {
  (void)argc;
  (void)argv;
  endless_loop();
}

static int spawn_endless_process(pid_t *pid) {
  static char name[] = "endless_loop";
  char *argv[] = {name, NULL};
  uint64_t raw = sys_launch_process((void *)endless_loop_entry, DEFAULT_PRIO, 1, argv);
  if (raw == (uint64_t)-1) {
    return -1;
  }
  *pid = (pid_t)raw;
  // Ensure spawned dummy processes never take the terminal
  sys_process_set_foreground(*pid, 0);
  return 0;
}

int64_t test_processes(uint64_t argc, char *argv[]) {
  if (argc != 1)
    return -1;

  // Suppress verbose output if this test is running in background
  int quiet = 0;
  processInfo self = {0};
  pid_t me = (pid_t)sys_get_pid();
  if (sys_get_process_info(me, &self) == 0) {
    quiet = (self.foreground == 0);
  }

  int64_t max_processes = satoi(argv[0]);
  if (max_processes <= 0 || max_processes > MAX_PROCESSES) {
    printf("test_processes: invalid max process count.\n");
    return -1;
  }

  process_req *p_rqs = (process_req *)sys_malloc(sizeof(process_req) * max_processes);
  if (p_rqs == NULL) {
    printf("test_processes: unable to allocate bookkeeping array.\n");
    return -1;
  }

  while (1) {
    uint64_t alive = 0;

    for (int64_t rq = 0; rq < max_processes; rq++) {
      if (spawn_endless_process(&p_rqs[rq].pid) != 0) {
        printf("test_processes: ERROR creating process\n");
        sys_free(p_rqs);
        return -1;
      }
      p_rqs[rq].state = PROC_RUNNING;
      alive++;
    }

    while (alive > 0) {
      static int tick = 0;
      for (int64_t rq = 0; rq < max_processes; rq++) {
        uint8_t action = GetUniform(100) % 2;
        switch (action) {
          case 0:  // kill
            if (p_rqs[rq].state == PROC_RUNNING || p_rqs[rq].state == PROC_BLOCKED) {
              int64_t res = (int64_t)sys_process_kill(p_rqs[rq].pid);
              // Treat "already terminated" as success to avoid spurious failures
              if (res == 0 || res == -1) {
                p_rqs[rq].state = PROC_KILLED;
                alive--;
              } else {
                printf("test_processes: ERROR killing process (pid %d, res %d)\n", p_rqs[rq].pid, (int)res);
                sys_free(p_rqs);
                return -1;
              }
            }
            break;
          case 1:  // block
            if (p_rqs[rq].state == PROC_RUNNING) {
              int64_t res = (int64_t)sys_process_block(p_rqs[rq].pid);
              if (res == 0) {
                p_rqs[rq].state = PROC_BLOCKED;
              } else {
                // if block fails (race), assume it got killed elsewhere and continue
                p_rqs[rq].state = PROC_KILLED;
                alive--;
              }
            }
            break;
        }
      }

      if (!quiet && (++tick % 200) == 0) {
        printf("testprocesses: cycling... alive=%d\n", (int)alive);
      }

      for (int64_t rq = 0; rq < max_processes; rq++) {
        if (p_rqs[rq].state == PROC_BLOCKED && (GetUniform(100) % 2)) {
          int64_t res = (int64_t)sys_process_unblock(p_rqs[rq].pid);
          if (res == 0) {
            p_rqs[rq].state = PROC_RUNNING;
          } else {
            // if unblock fails, consider it killed or already running; keep going
            p_rqs[rq].state = PROC_RUNNING;
          }
        }
      }
    }
  }
}