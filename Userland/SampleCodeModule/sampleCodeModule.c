// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
/* sampleCodeModule.c */
#include <userio.h>
#include <usyscalls.h>
#include <colors.h>
#include <shell.h>
#include <pong.h>
#include <types.h>
#include <uSync.h>

int main() {
	// Choose between terminal or pong
	printColor("Welcome to HomerOS. \n\n", GREEN);

	/* unsigned char c;
	do {
		c = getChar();
	} while (c != 't' && c != 'p');

	getChar();

	if (c == 'p')
		pong(); */

	/* sys_clear_screen(); */

	int shellPid=launchShell();
	wait(shellPid);
	return 0;
}