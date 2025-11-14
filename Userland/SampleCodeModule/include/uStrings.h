#ifndef U_STRINGS_H
#define U_STRINGS_H

#include <stdint.h>

void strcpy(char * dest, char * src);

int strcmp(const char * str1, const char * str2);

int strncmp(char * str1, char * str2, int length);

uint64_t atoi(char * str);

int strtoi(char * buffer, int * i);

void intToStr(int num, char* str);

void intToHex(uint64_t num, char* hex);

unsigned int strlen(char* str);

char* strchr(const char* str, int c);

char* strtok(char* str, const char* delim);

void appendString(char* dest, const char* src);

void copyString(char* dest, const char* src);

int isDigit(char c);

int isVowel(char c);

int isNumber(const char* str);

void* memset(void* destiation, int32_t c, uint64_t length);

#endif // U_STRINGS_H