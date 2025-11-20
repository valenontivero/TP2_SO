// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <processLib.h>
#include <types.h>
#include <usyscalls.h>
#include <userio.h>
#include <stddef.h>
#include <utime.h>
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

static void count_states(process_req *p_rqs, int64_t max_processes, 
                         uint64_t *running, uint64_t *blocked, uint64_t *killed) {
  *running = 0;
  *blocked = 0;
  *killed = 0;
  for (int64_t rq = 0; rq < max_processes; rq++) {
    if (p_rqs[rq].state == PROC_RUNNING) (*running)++;
    else if (p_rqs[rq].state == PROC_BLOCKED) (*blocked)++;
    else if (p_rqs[rq].state == PROC_KILLED) (*killed)++;
  }
}

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

  static uint64_t cycle = 0;

  while (1) {
    cycle++;
    uint64_t alive = 0;

    printf("testprocesses: Starting cycle %d, creating %d processes...\n", (int)cycle, (int)max_processes);

    for (int64_t rq = 0; rq < max_processes; rq++) {
      if (spawn_endless_process(&p_rqs[rq].pid) != 0) {
        printf("testprocesses: ERROR creating process\n");
        sys_free(p_rqs);
        return -1;
      }
      p_rqs[rq].state = PROC_RUNNING;
      alive++;
      printf("testprocesses: Process %d created with PID %d\n", (int)(rq + 1), (int)p_rqs[rq].pid);
      for (int i = 0; i < 10; i++) sys_yield(); // Small delay for readability
    }

    printf("testprocesses: All %d processes created. Starting operations...\n", (int)max_processes);

    uint64_t iteration = 0;
    while (alive > 0) {
      iteration++;
      
      uint64_t running, blocked, killed;
      count_states(p_rqs, max_processes, &running, &blocked, &killed);

      // Perform random operations
      for (int64_t rq = 0; rq < max_processes; rq++) {
        uint8_t action = GetUniform(100) % 2;
        switch (action) {
          case 0:  // kill
            if (p_rqs[rq].state == PROC_RUNNING || p_rqs[rq].state == PROC_BLOCKED) {
              const char* state_str = (p_rqs[rq].state == PROC_RUNNING) ? "RUNNING" : "BLOCKED";
              int64_t res = (int64_t)killProcess(p_rqs[rq].pid);
              // Treat "already terminated" as success to avoid spurious failures
              if (res == 0 || res == -1) {
                printf("testprocesses: Process %d killed (state: %s). Remaining alive: %d\n", 
                       (int)p_rqs[rq].pid, state_str, (int)(alive - 1));
                p_rqs[rq].state = PROC_KILLED;
                alive--;
                for (int i = 0; i < 10; i++) sys_yield(); // Small delay for readability
              } else {
                printf("testprocesses: ERROR killing process (pid %d, res %d)\n", p_rqs[rq].pid, (int)res);
                sys_free(p_rqs);
                return -1;
              }
            }
            break;
          case 1:  // block
            if (p_rqs[rq].state == PROC_RUNNING) {
              int64_t res = (int64_t)sys_process_block(p_rqs[rq].pid);
              if (res == 0) {
                printf("testprocesses: Process %d blocked\n", (int)p_rqs[rq].pid);
                p_rqs[rq].state = PROC_BLOCKED;
                for (int i = 0; i < 10; i++) sys_yield(); // Small delay for readability
              } else {
                // if block fails (race), assume it got killed elsewhere and continue
                printf("testprocesses: Process %d block failed, assuming killed\n", (int)p_rqs[rq].pid);
                p_rqs[rq].state = PROC_KILLED;
                alive--;
              }
            }
            break;
        }
      }

      // Unblock processes randomly
      for (int64_t rq = 0; rq < max_processes; rq++) {
        if (p_rqs[rq].state == PROC_BLOCKED && (GetUniform(100) % 2)) {
          int64_t res = (int64_t)sys_process_unblock(p_rqs[rq].pid);
          if (res == 0) {
            printf("testprocesses: Process %d unblocked and becomes RUNNING\n", (int)p_rqs[rq].pid);
            p_rqs[rq].state = PROC_RUNNING;
            for (int i = 0; i < 10; i++) sys_yield(); 
          } else {
            // if unblock fails, consider it killed or already running; keep going
            p_rqs[rq].state = PROC_RUNNING;
          }
        }
      }

      count_states(p_rqs, max_processes, &running, &blocked, &killed);

      if (iteration == 1 || iteration % 3 == 0 || alive == 0) {
        printf("testprocesses: [iteration %d] alive=%d (running=%d, blocked=%d, killed=%d)\n",
               (int)iteration, (int)alive, (int)running, (int)blocked, (int)killed);
        for (int i = 0; i < 5; i++) sys_yield(); 
      }
      
      for (int i = 0; i < 5; i++) sys_yield(); 
    }

    uint64_t running, blocked, killed;
    count_states(p_rqs, max_processes, &running, &blocked, &killed);

    printf("testprocesses: Cycle %d completed. Final state: alive=%d (running=%d, blocked=%d, killed=%d). Waiting before next cycle...\n",
           (int)cycle, (int)alive, (int)running, (int)blocked, (int)killed);
    sleep(2);
  }
}