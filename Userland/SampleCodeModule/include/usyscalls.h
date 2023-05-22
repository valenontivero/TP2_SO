#ifndef USYSCALLS_H
#define USYSCALLS_H

#include <stdint.h>

uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size);

uint64_t sys_write(unsigned int fd, const char* buffer, unsigned int size);

uint64_t sys_write_color(unsigned int fd, const char* buffer, unsigned int size, uint64_t color);

#endif