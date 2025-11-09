#include <stdint.h>
#include <shellFunc.h>
#include <userio.h>
#include <colors.h>
#include <sounds.h>
#include <pong.h>
#include <uStrings.h>
#include <processLib.h>
#include <usyscalls.h>
#include <stddef.h>
#include <utime.h>

extern void divideByZero();
extern void invalidOpcode();

static char *commandsHelp[] = {
	"\thelp: gives you a list of all existent commands.\n",
	"\ttime: prints the time of the OS.\n",
	"\tdate: prints the date of the OS.\n",
	"\tregisters: print the state of the registers at the time you screenshot them with CTRL key.\n",
	"\tfillregs: fill the registers with stepped values for testing.\n",
	"\tdiv0: divide by zero to trigger exception\n",
	"\tinvalidop: trigger invalid operation code exception\n",
	"\tpong: go to play the \"pong\" game.\n",
	"\tclear: clears the OS screen.\n",
    "\thello:prints hello every 5 seconds.\n"
};

void help (uint8_t argc, char **argv) {
    printColor("Available commands:\n", ORANGE);
    for (int i = 0; i < COMMANDS_QUANTITY; i++) {
        printColor(commandsHelp[i], GREEN);
    }
}


void time (uint8_t argc, char **argv) {
    printfColor("\nTime of OS: ", ORANGE);
    printfColor("%s\n", GREEN, getTime());
}

void date (uint8_t argc, char **argv) {
    printfColor("\nDate of OS: ", ORANGE);
    printfColor("%s\n", GREEN, getDate());
}

void registers (uint8_t argc, char **argv) {
    printRegs();
}

void fillregs (uint8_t argc, char **argv) {
    fillRegisters();
}

void div0 (uint8_t argc, char **argv) {
    divideByZero();
}

void invalidop (uint8_t argc, char **argv) {
    invalidOpcode();
}

void pong (uint8_t argc, char **argv) {
    playPong();
}

void clear (uint8_t argc, char **argv) {
    sys_clear_screen();
}

void hello (uint8_t argc, char **argv) {
    while (1) {
        printColor("Hello\n", CYAN);
        sleep(5);
    }
}