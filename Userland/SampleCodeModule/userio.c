#include <usyscalls.h>

unsigned int strlen(char* str);

void print(char* string) {
    sys_write(1, string, strlen(string));
}

void printChar(char c) {
    char str[] = {c};
    sys_write(1, str, 1);
}

char getChar() {
    char c[] = {0};
    sys_read(0, c, 1);
    return c[0];
}

unsigned int strlen(char* str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}