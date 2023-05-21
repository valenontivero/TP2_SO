#include <usyscalls.h>

extern uint64_t syscall(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);


void sys_read(unsigned int fd, char* buffer, unsigned int size) {
    syscall(0, fd, buffer, size, 0, 0);
}
void sys_write(unsigned int fd, const char* buffer, unsigned int size) {
    syscall(1, fd, buffer, size, 0, 0);
}
