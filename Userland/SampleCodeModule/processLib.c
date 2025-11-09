#include <processLib.h>
#include <types.h>
#include <usyscalls.h>

pid_t createProcess(void (*entryPoint)(uint8_t, char **), uint8_t prio ,uint8_t argc, char** argv) {
    return sys_launch_process((void*) entryPoint, prio, argc, argv);
}

void putInFg(pid_t pid) {
    sys_put_in_fg(pid);
}
