#include <usyscalls.h>

extern uint64_t sys_call(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size) {
    return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write(unsigned int fd, const char* buffer, unsigned int size) {
    return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}
