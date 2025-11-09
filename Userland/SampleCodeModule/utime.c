#include <stdint.h>
#include <utime.h>
#include <usyscalls.h>

void sleep(int seconds) {
    sys_timer_wait(seconds);
}