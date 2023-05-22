#include <usyscalls.h>
#include <stdint.h>

extern uint64_t sys_call(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void sys_read(unsigned int fd, char* buffer, unsigned int size) {
    sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}
void sys_write(unsigned int fd, const char* buffer, unsigned int size) {
    sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}
