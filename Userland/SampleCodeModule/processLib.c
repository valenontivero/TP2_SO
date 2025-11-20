#include <processLib.h>
#include <types.h>
#include <usyscalls.h>
#include <userio.h>
#include <colors.h>
#include <uStrings.h>   

pid_t createProcess(void (*entryPoint)(uint8_t, char **), uint8_t prio ,uint8_t argc, char** argv) {
    return sys_launch_process((void*) entryPoint, prio, argc, argv);
}

void putInFg(pid_t pid1, pid_t pid2) {
    sys_put_in_fg(pid1, pid2);
}


uint64_t blockProcess(pid_t pid) {
    processInfo info;
    if (sys_get_process_info(pid, &info) != 0) {
        printColor("block: process not found or terminated.\n", RED);
        return (uint64_t)-1;
    }
    uint64_t res = (info.state == BLOCKED)
                       ? sys_process_unblock(pid)
                       : sys_process_block(pid);
    if (res != 0) {
        printColor("block: operation failed.\n", RED);
    }
    return res;
}

uint64_t killProcess(pid_t pid) {
    processInfo info;
    if (sys_get_process_info(pid, &info) != 0) {
        printColor("kill: process not found or terminated.\n", RED);
        return (uint64_t)-1;
    }
    if (pid == 0 || strcmp(info.name, "shell") == 0) {
        printColor("kill: cannot terminate shell or init process.\n", RED);
        return (uint64_t)-1;
    }
    uint64_t res = sys_process_kill(pid, sys_get_pid());
    if (res != 0) {
        printColor("kill: could not terminate process.\n", RED);
    }
    return res;
}