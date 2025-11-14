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
#include <uSync.h>

extern uint64_t test_mm(uint64_t argc, char *argv[]);
extern int64_t test_processes(uint64_t argc, char *argv[]);
extern uint64_t test_prio(uint64_t argc, char *argv[]);
extern uint64_t test_synchro(uint64_t argc, char *argv[]);
extern uint64_t test_no_synchro(uint64_t argc, char *argv[]);

static void test_mm_entry(uint8_t argc, char **argv) {
    test_mm((uint64_t)argc, argv);
}

static void test_processes_entry(uint8_t argc, char **argv) {
    test_processes((uint64_t)argc, argv);
}

static void test_prio_entry(uint8_t argc, char **argv) {
    test_prio((uint64_t)argc, argv);
}

static void test_synchro_entry(uint8_t argc, char **argv) {
    test_synchro((uint64_t)argc, argv);
}

static void test_no_synchro_entry(uint8_t argc, char **argv) {
    test_no_synchro((uint64_t)argc, argv);
}

static void launch_test_process(const char *name, void (*entry)(uint8_t, char **), uint8_t argc, char **argv) {
    pid_t pid = sys_launch_process((void *)entry, DEFAULT_PRIO, argc, argv);
    if ((int64_t)pid < 0) {
        printf("%s: unable to launch test process.\n", name);
    } else {
        printf("%s started with pid %d\n", name, (int)pid);
    }
}


#define NAME_COL_WIDTH 16
#define PID_COL_WIDTH 6
#define PRIO_COL_WIDTH 6
#define STATE_COL_WIDTH 10
#define FG_COL_WIDTH 6
#define POINTER_COL_WIDTH 18

#define PS_BORDER_COLOR ORANGE
#define PS_HEADER_COLOR ORANGE
#define PS_TEXT_COLOR WHITE
#define PS_TRUE_TEXT "yes"
#define PS_FALSE_TEXT "no"

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
    "testmm <max_mem>: stress tests the memory manager with random allocations.",
    "testprocesses <max_procs>: exercises process creation/kill/block/unblock.",
    "testsynchro <pairs> <iterations>: runs synced increment/decrement workload.",
    "testnosynchro <pairs> <iterations>: same as testsynchro but without semaphores.",
    "testpriority <limit>: demonstrates scheduler priority handling.",
	"mem: prints current memory usage.",
	"ps: lists the active processes with their attributes.",
	"loop [seconds]: prints the process ID periodically (default 1 second).",
	"kill <pid>: terminates the process with the given PID.",
	"nice <pid> <priority>: sets the process priority (0 is highest).",
	"block <pid>: toggles the blocked state of the process.",
	"wc: counts the number of input lines.",
	"filter: removes vowels from the input stream.",
	"cat: prints the stdin exactly as received.",
    "echo <text>: prints the provided arguments to the standard output.",
    "mvar <writers> <readers>: launches writers/readers synchronized through an mvar."
};




typedef void (*PsCellPrinter)(const processInfo *info, int width);

typedef struct {
    const char *header;
    int width;
    PsCellPrinter printer;
} PsColumnSpec;




static const char *stateToString(State state) {
    switch (state) {
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case BLOCKED: return "BLOCKED";
        case TERMINATED: return "TERM";
        default: return "UNKNOWN";
    }
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

static void printHorizontal(int width, uint64_t color) {
    for (int i = 0; i < width; i++) {
        printColorChar('-', color);
    }
}

static void printTableSeparator(const PsColumnSpec *columns, int columnCount) {
    printColorChar('+', PS_BORDER_COLOR);
    for (int i = 0; i < columnCount; i++) {
        printHorizontal(columns[i].width + 2, PS_BORDER_COLOR);
        printColorChar('+', PS_BORDER_COLOR);
    }
    printColorChar('\n', PS_BORDER_COLOR);
}

static void printTableCellText(const char *text, int width, uint64_t color) {
    printColorChar('|', PS_BORDER_COLOR);
    printColorChar(' ', PS_BORDER_COLOR);
    printPaddedColor(text, width, color);
    printColorChar(' ', PS_BORDER_COLOR);
}

static void printTableCellDec(int value, int width, uint64_t color) {
    printColorChar('|', PS_BORDER_COLOR);
    printColorChar(' ', PS_BORDER_COLOR);
    printPaddedDec(value, width, color);
    printColorChar(' ', PS_BORDER_COLOR);
}

static void printTableCellHex(uint64_t value, int width, uint64_t color) {
    printColorChar('|', PS_BORDER_COLOR);
    printColorChar(' ', PS_BORDER_COLOR);
    printPaddedHex(value, width, color);
    printColorChar(' ', PS_BORDER_COLOR);
}

static void printTableRowEnd(void) {
    printColorChar('|', PS_BORDER_COLOR);
    printColorChar('\n', PS_BORDER_COLOR);
}

static void printPsNameCell(const processInfo *info, int width) {
    printTableCellText(info->name, width, PS_TEXT_COLOR);
}

static void printPsPidCell(const processInfo *info, int width) {
    printTableCellDec((int)info->pid, width, PS_TEXT_COLOR);
}

static void printPsPriorityCell(const processInfo *info, int width) {
    printTableCellDec((int)info->priority, width, PS_TEXT_COLOR);
}

static void printPsStateCell(const processInfo *info, int width) {
    printTableCellText(stateToString(info->state), width, PS_TEXT_COLOR);
}

static void printPsForegroundCell(const processInfo *info, int width) {
    printTableCellText(info->foreground ? PS_TRUE_TEXT : PS_FALSE_TEXT, width, PS_TEXT_COLOR);
}

static void printPsStackBaseCell(const processInfo *info, int width) {
    printTableCellHex((uint64_t)info->stackBase, width, PS_TEXT_COLOR);
}

static void printPsStackPointerCell(const processInfo *info, int width) {
    printTableCellHex((uint64_t)info->stackPointer, width, PS_TEXT_COLOR);
}

static const PsColumnSpec psColumns[] = {
    {"NAME", NAME_COL_WIDTH, printPsNameCell},
    {"PID", PID_COL_WIDTH, printPsPidCell},
    {"PRIO", PRIO_COL_WIDTH, printPsPriorityCell},
    {"STATE", STATE_COL_WIDTH, printPsStateCell},
    {"FG", FG_COL_WIDTH, printPsForegroundCell},
    {"STACK_BASE", POINTER_COL_WIDTH, printPsStackBaseCell},
    {"STACK_PTR", POINTER_COL_WIDTH, printPsStackPointerCell}
};

static const int psColumnCount = sizeof(psColumns) / sizeof(psColumns[0]);

static void printTableHeader(const PsColumnSpec *columns, int columnCount) {
    for (int i = 0; i < columnCount; i++) {
        printTableCellText(columns[i].header, columns[i].width, PS_HEADER_COLOR);
    }
    printTableRowEnd();
}

static void printProcessRow(const processInfo *info, const PsColumnSpec *columns, int columnCount) {
    for (int i = 0; i < columnCount; i++) {
        columns[i].printer(info, columns[i].width);
    }
    printTableRowEnd();
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

void testmm(uint8_t argc, char **argv) {
    if (argc < 2) {
        printColor("Usage: testmm <max_mem>\n", RED);
        return;
    }
    if (!isNumber(argv[1])) {
        printColor("testmm: max_mem must be numeric.\n", RED);
        return;
    }
    launch_test_process("testmm", test_mm_entry, (uint8_t)(argc - 1), argv + 1);
}

void testprocesses(uint8_t argc, char **argv) {
    if (argc < 2) {
        printColor("Usage: testprocesses <max_procs>\n", RED);
        return;
    }
    if (!isNumber(argv[1])) {
        printColor("testprocesses: max_procs must be numeric.\n", RED);
        return;
    }
    launch_test_process("testprocesses", test_processes_entry, (uint8_t)(argc - 1), argv + 1);
}

void testsynchro(uint8_t argc, char **argv) {
    if (argc < 3) {
        printColor("Usage: testsynchro <pairs> <iterations>\n", RED);
        return;
    }
    if (!isNumber(argv[1]) || !isNumber(argv[2])) {
        printColor("testsynchro: arguments must be numeric.\n", RED);
        return;
    }
    launch_test_process("testsynchro", test_synchro_entry, (uint8_t)(argc - 1), argv + 1);
}

void testnosynchro(uint8_t argc, char **argv) {
    if (argc < 3) {
        printColor("Usage: testnosynchro <pairs> <iterations>\n", RED);
        return;
    }
    if (!isNumber(argv[1]) || !isNumber(argv[2])) {
        printColor("testnosynchro: arguments must be numeric.\n", RED);
        return;
    }
    launch_test_process("testnosynchro", test_no_synchro_entry, (uint8_t)(argc - 1), argv + 1);
}

void testpriority(uint8_t argc, char **argv) {
    if (argc < 2) {
        printColor("Usage: testpriority <limit>\n", RED);
        return;
    }
    if (!isNumber(argv[1])) {
        printColor("testpriority: limit must be numeric.\n", RED);
        return;
    }
    launch_test_process("testpriority", test_prio_entry, (uint8_t)(argc - 1), argv + 1);
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
    static processInfo list[MAX_PROCESSES];
    memset(list, 0, sizeof(list));
    uint16_t count = 0;
    if (sys_get_process_list(list, MAX_PROCESSES, &count) == (uint64_t)-1) {
        printColor("ps: unable to retrieve process list.\n", RED);
        return;
    }

    printColor("\n", PS_TEXT_COLOR);
    printTableSeparator(psColumns, psColumnCount);
    printTableHeader(psColumns, psColumnCount);
    printTableSeparator(psColumns, psColumnCount);

    for (uint16_t i = 0; i < count; i++) {
        printProcessRow(&list[i], psColumns, psColumnCount);
    }
    printTableSeparator(psColumns, psColumnCount);
    printColor("\n", PS_TEXT_COLOR);
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
    blockProcess(pid);
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

void echo(uint8_t argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        printColor(argv[i], WHITE);
        if (i < argc - 1) {
            printColor(" ", WHITE);
        }
    }
    printChar('\0');
    printChar('\n');
} 
