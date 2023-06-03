#ifndef USERIO_H
#define USERIO_H

#include <stdint.h>

static char * commandsNames[] = {"help", "time", "date", "registers", "fillregs", "div0", "invalidop", "pong", "clear"};

static char *commands[] = {
	"\thelp: gives you a list of all existants commands.\n",
	"\ttime: prints the time of the OS.\n",
	"\tdate: prints the date of the OS.\n",
	"\tregisters: print the state of the registers at the time you screenshot them with CTRL key.\n",
	"\tfillregs: fill the registers with stepped values for testing.\n",
	"\tdiv0: divide by zero to trigger exception\n",
	"\tinvalidop: trigger invalid operation code exception\n",
	"\tpong: go to play the \"pong\" game.\n",
	"\tclear: clears the OS screen.\n"
};
void print(char* string);

void printColor(char* string, uint64_t color);

void printChar(char c);

void printColorChar(char c, uint64_t color);

char getChar();

void putChar(char c);

unsigned int strlen(char *str);

void printRegs();

void printf(char *format, ...);

void printfColor(char *format, uint64_t color, ...);

// int scanf(char *format, ...);

void strcpy(char * dest, char * src);

int strncmp(char * str1, char * str2, int length);

void fillRegisters();

char * getTime();

char * getDate();

#define COMMANDS_QUANTITY 9

#endif