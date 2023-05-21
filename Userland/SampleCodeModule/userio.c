#include <usyscalls.h>

unsigned int strlen(char* str);

void print(char* string) {
    sys_write(1, string, strlen(string));
}

unsigned int strlen(char* str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}