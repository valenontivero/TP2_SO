#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stddef.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

/* Used for sound */
void outb(int memoryMap, int output); 
int inb(int memoryMap);

/* String functions */
int safe_strncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* str);
int strcmp(const char *s1, const char *s2);
void itoa(int value, char *str);
char *my_strcat(char *dest, const char *src);

#endif