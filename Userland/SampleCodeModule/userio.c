#include <usyscalls.h>
#include <userio.h>

void print(char* string) {
    sys_write(1, string, strlen(string));
}

void printColor(char* string, uint64_t color) {
    sys_write_color(1, string, strlen(string), color);
}

void printChar(char c) {
    char str[] = {c};
    sys_write(1, str, 1);
}

void printColorChar(char c, uint64_t color) {
    char str[] = {c};
    sys_write_color(1, str, 1, color);
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