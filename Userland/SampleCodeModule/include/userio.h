#ifndef USERIO_H
#define USERIO_H

#include <stdint.h>

void print(char* string);

void printColor(char* string, uint64_t color);

void printChar(char c);

void printColorChar(char c, uint64_t color);

char getChar();

unsigned int strlen(char *str);

#endif