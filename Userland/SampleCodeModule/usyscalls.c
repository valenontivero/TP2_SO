#include <usyscalls.h>

extern uint64_t sys_call(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size) {
    return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write(unsigned int fd, const char* buffer, unsigned int size) {
    return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write_color(unsigned int fd, const char* buffer, unsigned int size, uint64_t color) {
    /* uint64_t c = color.r;
    c <<= 2;
    c += color.g;
    c <<= 2;
    c += color.b; */
    return sys_call((uint64_t) 2, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, color, (uint64_t) 0);
}
