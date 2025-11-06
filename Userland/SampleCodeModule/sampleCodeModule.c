/* sampleCodeModule.c */
#include <userio.h>
#include <usyscalls.h>
#include <colors.h>
#include <shell.h>
#include <pong.h>

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

	launchShell();

	return 0;
}