// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
/* _loader.c */
#include <stdint.h>
#include <uStrings.h>

extern char bss;
extern char endOfBinary;

int main();



int _start() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	return main();

}
