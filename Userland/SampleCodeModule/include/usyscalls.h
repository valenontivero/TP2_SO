#ifndef USYSCALLS_H
#define USYSCALLS_H

void sys_read(unsigned int fd, char* buffer, unsigned int size);

void sys_write(unsigned int fd, const char* buffer, unsigned int size);

#endif