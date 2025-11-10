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
	"help: gives you a list of all existent commands.",
	"time: prints the time of the OS.",
	"date: prints the date of the OS.",
	"registers: print the state of the registers at the time you screenshot them with CTRL key.",
	"fillregs: fill the registers with stepped values for testing.",
	"div0: divide by zero to trigger exception",
	"invalidop: trigger invalid operation code exception",
	"pong: go to play the \"pong\" game.",
	"clear: clears the OS screen.",
    "hello: prints hello every 5 seconds.",
	"testprint: tests that you can create a process and it can print on screen.",
	"testsem: tests that a process can block itself with a semaphore and another process can unblock it",
	"testpipe: tests that 2 processes can comunicate between pipes",
	"testpriority: tests the priorities of 3 processes",
	"mem: prints current memory usage.",
	"ps: lists the active processes with their attributes.",
	"loop [seconds]: prints the process ID periodically (default 1 second).",
	"kill <pid>: terminates the process with the given PID.",
	"nice <pid> <priority>: sets the process priority (0 is highest).",
	"block <pid>: toggles the blocked state of the process.",
	"wc: counts the number of input lines.",
	"filter: removes vowels from the input stream.",
	"cat: prints the stdin exactly as received."
};

static const char *stateToString(State state) {
    switch (state) {
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case BLOCKED: return "BLOCKED";
        case TERMINATED: return "TERM";
        default: return "UNKNOWN";
    }
}

static int isDigit(char c) {
    return c >= '0' && c <= '9';
}

static int isNumber(const char *str) {
    if (str == NULL || *str == 0) {
        return 0;
    }
    int i = 0;
    while (str[i]) {
        if (!isDigit(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

static int isVowel(char c) {
    char lower = (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

static void printPaddedColor(const char *str, int width, uint64_t color) {
    int len = strlen((char *)str);
    printColor((char *)str, color);
    while (len < width) {
        printColorChar(' ', color);
        len++;
    }
}

static void printPaddedDec(int value, int width, uint64_t color) {
    char buffer[12] = {0};
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
    } else {
        intToStr(value, buffer);
    }
    printPaddedColor(buffer, width, color);
}

static void printPaddedHex(uint64_t value, int width, uint64_t color) {
    char buffer[19] = {0};
    buffer[0] = '0';
    buffer[1] = 'x';
    intToHex(value, buffer + 2);
    printPaddedColor(buffer, width, color);
}

#define NAME_COL_WIDTH 16
#define PID_COL_WIDTH 6
#define PRIO_COL_WIDTH 6
#define STATE_COL_WIDTH 10
#define FG_COL_WIDTH 6
#define POINTER_COL_WIDTH 18

static void printHorizontal(int width, uint64_t color) {
    for (int i = 0; i < width; i++) {
        printColorChar('-', color);
    }
}

static void printTableSeparator(uint64_t color) {
    printColorChar('+', color);
    printHorizontal(NAME_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(PID_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(PRIO_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(STATE_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(FG_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(POINTER_COL_WIDTH + 2, color);
    printColorChar('+', color);
    printHorizontal(POINTER_COL_WIDTH + 2, color);
    printColor("+\n", color);
}

static void printTableCell(const char *text, int width, uint64_t color) {
    printColorChar('|', ORANGE);
    printColorChar(' ', ORANGE);
    printPaddedColor(text, width, color);
    printColorChar(' ', ORANGE);
}

static void printTableCellDec(int value, int width, uint64_t color) {
    printColorChar('|', ORANGE);
    printColorChar(' ', ORANGE);
    printPaddedDec(value, width, color);
    printColorChar(' ', ORANGE);
}

static void printTableCellHex(uint64_t value, int width, uint64_t color) {
    printColorChar('|', ORANGE);
    printColorChar(' ', ORANGE);
    printPaddedHex(value, width, color);
    printColorChar(' ', ORANGE);
}

static void printTableRowEnd() {
    printColor("|\n", ORANGE);
}

void help (uint8_t argc, char **argv) {
	printColor("\nAvailable commands:\n", ORANGE);
	for (int i = 0; i < COMMANDS_QUANTITY; i++) {
		printColor("\t", GREEN);
		printColor(commandsHelp[i], GREEN);
		printColor("\n", GREEN);
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

void mem(uint8_t argc, char **argv) {
    memoryData info = {0};
    if (sys_get_mem_info(&info) != 0) {
        printColor("mem: unable to retrieve memory information.\n", RED);
        return;
    }
    printf("Total: %d bytes\n", (int)info.total);
    printf("Used: %d bytes\n", (int)info.used);
    printf("Free: %d bytes\n", (int)info.free);
}

void ps(uint8_t argc, char **argv) {
    processInfo list[MAX_PROCESSES] = {0};
    uint16_t count = 0;
    if (sys_get_process_list(list, MAX_PROCESSES, &count) == (uint64_t)-1) {
        printColor("ps: unable to retrieve process list.\n", RED);
        return;
    }

    printColor("\n", WHITE);
    printTableSeparator(ORANGE);
    printTableCell("NAME", NAME_COL_WIDTH, ORANGE);
    printTableCell("PID", PID_COL_WIDTH, ORANGE);
    printTableCell("PRIO", PRIO_COL_WIDTH, ORANGE);
    printTableCell("STATE", STATE_COL_WIDTH, ORANGE);
    printTableCell("FG", FG_COL_WIDTH, ORANGE);
    printTableCell("STACK_BASE", POINTER_COL_WIDTH, ORANGE);
    printTableCell("STACK_PTR", POINTER_COL_WIDTH, ORANGE);
    printTableRowEnd();
    printTableSeparator(ORANGE);

    for (uint16_t i = 0; i < count; i++) {
        printTableCell(list[i].name, NAME_COL_WIDTH, WHITE);
        printTableCellDec((int)list[i].pid, PID_COL_WIDTH, WHITE);
        printTableCellDec((int)list[i].priority, PRIO_COL_WIDTH, WHITE);
        printTableCell(stateToString(list[i].state), STATE_COL_WIDTH, WHITE);
        printTableCell(list[i].foreground ? "yes" : "no", FG_COL_WIDTH, WHITE);
        printTableCellHex((uint64_t)list[i].stackBase, POINTER_COL_WIDTH, WHITE);
        printTableCellHex((uint64_t)list[i].stackPointer, POINTER_COL_WIDTH, WHITE);
        printTableRowEnd();
    }
    printTableSeparator(ORANGE);
    printColor("\n", WHITE);
}

void loop(uint8_t argc, char **argv) {
    int seconds = 1;
    if (argc > 1) {
        if (!isNumber(argv[1])) {
            printColor("loop: seconds must be numeric.\n", RED);
            return;
        }
        seconds = (int)atoi(argv[1]);
        if (seconds <= 0) {
            seconds = 1;
        }
    }

    pid_t pid = (pid_t)sys_get_pid();
    while (1) {
        printf("Process %d says hello!\n", (int)pid);
        sleep(seconds);
    }
}

void kill(uint8_t argc, char **argv) {
    if (argc < 2) {
        printColor("Usage: kill <pid>\n", RED);
        return;
    }
    if (!isNumber(argv[1])) {
        printColor("kill: pid must be numeric.\n", RED);
        return;
    }
    pid_t pid = (pid_t)atoi(argv[1]);
    processInfo info;
    if (sys_get_process_info(pid, &info) != 0) {
        printColor("kill: process not found or terminated.\n", RED);
        return;
    }
    if (pid == 0 || strcmp(info.name, "shell") == 0) {
        printColor("kill: cannot terminate shell or init process.\n", RED);
        return;
    }
    if (sys_process_kill(pid) != 0) {
        printColor("kill: could not terminate process.\n", RED);
    }
}

void nice(uint8_t argc, char **argv) {
    if (argc < 3) {
        printColor("Usage: nice <pid> <priority>\n", RED);
        return;
    }
    if (!isNumber(argv[1]) || !isNumber(argv[2])) {
        printColor("nice: pid and priority must be numeric.\n", RED);
        return;
    }
    pid_t pid = (pid_t)atoi(argv[1]);
    uint8_t priority = (uint8_t)atoi(argv[2]);
    if (priority >= PRIORITY_LEVELS) {
        printColor("nice: priority out of range.\n", RED);
        return;
    }
    if (sys_process_nice(pid, priority) != 0) {
        printColor("nice: failed to update priority.\n", RED);
    }
}

void block(uint8_t argc, char **argv) {
    if (argc < 2) {
        printColor("Usage: block <pid>\n", RED);
        return;
    }
    if (!isNumber(argv[1])) {
        printColor("block: pid must be numeric.\n", RED);
        return;
    }
    pid_t pid = (pid_t)atoi(argv[1]);
    processInfo info;
    if (sys_get_process_info(pid, &info) != 0) {
        printColor("block: process not found or terminated.\n", RED);
        return;
    }
    uint64_t res;
    if (info.state == BLOCKED) {
        res = sys_process_unblock(pid);
    } else {
        res = sys_process_block(pid);
    }
    if (res != 0) {
        printColor("block: operation failed.\n", RED);
    }
}

void wc(uint8_t argc, char **argv) {
	printChar('\n');
	char buffer[2] = {0};
	int lines = 1;
	while (1) {
		uint64_t read = sys_read(STDIN, buffer, 1);
		if (read == (uint64_t)-1) {
			printColor("wc: read error.\n", RED);
			return;
		}
		if (read == 0) {
			continue;
		}
		char c = buffer[0];
		if (c == 4) { // CTRL + D
			break;
		}
		if (c == '\n') {
			printChar('\n');
			lines++;
		} else if (c > 20 && c < 127) {
			sys_write(STDOUT, &c, 1);
		}
	}
	if (lines < 0) {
		lines = 0;
	}
	printf("\nTotal lines: %d\n", lines);
}

void filter(uint8_t argc, char **argv) {
	char buffer[2] = {0};
	while (1) {
		uint64_t read = sys_read(STDIN, buffer, 1);
		if (read == (uint64_t)-1) {
			printColor("filter: read error.\n", RED);
			return;
		}
		if (read == 0) {
			continue;
		}
		if (buffer[0] == 4) {
			break;
		}
		if (!isVowel(buffer[0])) {
			sys_write(STDOUT, buffer, 1);
		}
	}
}

void cat(uint8_t argc, char **argv) {
	char buffer[2] = {0};
	while (1) {
		uint64_t read = sys_read(STDIN, buffer, 1);
		if (read == (uint64_t)-1) {
			printColor("cat: read error.\n", RED);
			return;
		}
		if (read == 0) {
			continue;
		}
		if (buffer[0] == 4) {
			break;
		}
		sys_write(STDOUT, buffer, 1);
	}
}