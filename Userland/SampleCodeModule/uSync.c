#include <uSync.h>
#include <types.h>
#include <usyscalls.h>

void wait(pid_t pid){
    sys_wait(pid);
}