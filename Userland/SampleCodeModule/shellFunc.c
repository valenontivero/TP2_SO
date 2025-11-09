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
#include <test_print.h>
#include <test_sem.h>
#include <test_pipe.h>
#include <test_priority.h>

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
    "\thello:prints hello every 5 seconds.\n",
	"\ttestprint: tests that you can create a process and it can print on screen.\n",
	"\ttestsem: tests that a process can block itself with a semaphore and another process can unblock it\n",
	"\ttestpipe: tests that 2 processes can comunicate between pipes\n",
	"\ttestpriority: tests the priorities of 3 processes\n"
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

void testprint (uint8_t argc, char **argv){
	char* argvv[] = {"testPrint"};
	sys_launch_process((void*) testPrint, 1, 1, argvv);
}

void testsem (uint8_t argc, char **argv){
	char* argv1[] = {"testSemPoster"};
	char* argv2[] = {"testSemWaiter"};
	sys_launch_process((void*) testSemPoster, 1, 1, argv1);
	sys_launch_process((void*) testSemWaiter, 1, 1, argv2);
}

void testpipe (uint8_t argc, char **argv){
	char* argv1[] = {"testPipeReader"};
	char* argv2[] = {"testPipeWriter"};
	sys_launch_process((void*) testPipeReader, 1, 1, argv1);
	sys_launch_process((void*) testPipeWriter, 1, 1, argv2);
}

void testpriority (uint8_t argc, char **argv){
	char* argv1[] = {"testPriorityHigh"};
	char* argv2[] = {"testPriorityMedium"};
	char* argv3[] = {"testPriorityLow"};
	sys_launch_process((void*) testPriorityHigh, 3, 1, argv1);
	sys_launch_process((void*) testPriorityMedium, 2, 1, argv2);
	sys_launch_process((void*) testPriorityLow, 1, 1, argv3);
}