#include <processLib.h>
#include <types.h>
#include <usyscalls.h>
#include <userio.h>
#include <colors.h>

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
        return;
    }
    uint64_t res;
    if (info.state == BLOCKED) {
        res = sys_process_unblock(pid);
    } else {
        res = sys_process_block(pid);
    }
    if (res != 0) {
        printColor("block: operation failed.\n", RED);
    }
}