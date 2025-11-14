#include "syscall.h"

#include <stddef.h>
#include <processLib.h>
#include <stdint.h>
#include <usyscalls.h>
#include <uStrings.h>

#include "test_util.h"

extern uint64_t my_process_inc(uint64_t argc, char *argv[]);

typedef void (*entry_fn_t)(uint8_t, char **);

static void endless_loop_entry(uint8_t argc, char **argv) {
    (void)argc;
    (void)argv;
    endless_loop();
}

static void endless_loop_print_entry(uint8_t argc, char **argv) {
    uint64_t wait = 0;
    if (argc > 0 && argv != NULL && argv[0] != NULL) {
        wait = (uint64_t)satoi(argv[0]);
    }
    endless_loop_print(wait);
}

static void my_process_inc_entry(uint8_t argc, char **argv) {
    my_process_inc((uint64_t)argc, argv);
}

typedef struct {
    const char *name;
    entry_fn_t fn;
} named_process_t;

static const named_process_t known_processes[] = {
    {"endless_loop", endless_loop_entry},
    {"endless_loop_print", endless_loop_print_entry},
    {"my_process_inc", my_process_inc_entry},
};

#define MAX_TRACKED_SEMS 16
#define SEM_NAME_MAX 32

typedef struct {
    char name[SEM_NAME_MAX];
    uint8_t id;
    uint8_t in_use;
} tracked_sem_t;

static tracked_sem_t sem_table[MAX_TRACKED_SEMS] = {0};

static int find_process_index(const char *name) {
    for (size_t i = 0; i < sizeof(known_processes) / sizeof(known_processes[0]); i++) {
        if (strcmp((char *)known_processes[i].name, (char *)name) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static int find_sem_index(const char *name) {
    for (int i = 0; i < MAX_TRACKED_SEMS; i++) {
        if (sem_table[i].in_use && strcmp(sem_table[i].name, (char *)name) == 0) {
            return i;
        }
    }
    return -1;
}

static int find_free_sem_slot(void) {
    for (int i = 0; i < MAX_TRACKED_SEMS; i++) {
        if (!sem_table[i].in_use) {
            return i;
        }
    }
    return -1;
}

int64_t my_create_process(const char *name, uint64_t argc, char *argv[]) {
    if (name == NULL) {
        return -1;
    }

    int index = find_process_index(name);
    if (index < 0) {
        return -1;
    }

    entry_fn_t entry = known_processes[index].fn;
    pid_t pid = createProcess(entry, DEFAULT_PRIO, (uint8_t)argc, argv);
    return pid < 0 ? -1 : pid;
}

int64_t my_kill(int64_t pid) {
    return (int64_t)sys_process_kill((pid_t)pid);
}

int64_t my_block(int64_t pid) {
    return (int64_t)sys_process_block((pid_t)pid);
}

int64_t my_unblock(int64_t pid) {
    return (int64_t)sys_process_unblock((pid_t)pid);
}

int64_t my_nice(int64_t pid, uint64_t priority) {
    return (int64_t)sys_process_nice((pid_t)pid, (uint8_t)priority);
}

int64_t my_wait(int64_t pid) {
    return (int64_t)sys_wait((pid_t)pid);
}

int64_t my_getpid(void) {
    return (int64_t)sys_get_pid();
}

int64_t my_sem_open(const char *name, uint64_t initial_value) {
    if (name == NULL) {
        return 0;
    }

    int index = find_sem_index(name);
    if (index >= 0) {
        return 1;
    }

    int slot = find_free_sem_slot();
    if (slot < 0) {
        return 0;
    }

    uint64_t id = sys_sem_open(name, (uint8_t)initial_value);
    if (id == (uint64_t)-1) {
        return 0;
    }

    sem_table[slot].in_use = 1;
    sem_table[slot].id = (uint8_t)id;
    int i;
    for (i = 0; i < SEM_NAME_MAX - 1 && name[i] != '\0'; i++) {
        sem_table[slot].name[i] = name[i];
    }
    sem_table[slot].name[i] = '\0';
    return 1;
}

static int get_sem_id(const char *name) {
    int index = find_sem_index(name);
    if (index < 0) {
        return -1;
    }
    return sem_table[index].id;
}

int64_t my_sem_wait(const char *name) {
    int id = get_sem_id(name);
    if (id < 0) {
        return -1;
    }
    return (int64_t)sys_sem_wait((uint8_t)id);
}

int64_t my_sem_post(const char *name) {
    int id = get_sem_id(name);
    if (id < 0) {
        return -1;
    }
    return (int64_t)sys_sem_post((uint8_t)id);
}

int64_t my_sem_close(const char *name) {
    int index = find_sem_index(name);
    if (index < 0) {
        return -1;
    }

    sys_sem_close(sem_table[index].id);
    sem_table[index].in_use = 0;
    sem_table[index].name[0] = '\0';
    return 0;
}

void my_yield(void) {
    sys_timer_wait(0);
}

