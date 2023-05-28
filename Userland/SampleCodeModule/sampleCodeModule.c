/* sampleCodeModule.c */
#include <userio.h>
#include <usyscalls.h>
#include <colors.h>

extern void divideByZero();

extern void invalidOpcode();

extern void fillRegs();

void analizeBuffer(char * buffer);

static char * commandsNames[] = {"help", "time", "date", "registers", "fillregs", "div0", "invalidop", "pong", "clear"};

static char *commands[] = {
	"\thelp: gives you a list of all existants commands.\n",
	"\ttime: prints the time of the OS.\n",
	"\tdate: prints the date of the OS.\n",
	"\tregisters: print the state of the registers at the time you screenshot them with CTRL key.\n",
	"\tfillregs: fill the registers with stepped values for testing.\n",
	"\tdiv0: divide by zero to trigger exception\n",
	"\tinvalidop: \n",
	"\tpong: go to play the \"pong\" game.\n",
	"\tclear: clears the OS screen.\n"
};
#define COMMANDS_QUANTITY 9

char * getTime(){
	static char bufferTime[9];
	sys_get_time(bufferTime);
	return bufferTime;
}

char * getDate(){
	static char bufferDate[9];
	sys_get_date(bufferDate);
	return bufferDate;
}


int main() {
	printColor("Welcome to ben@OS: type \"help\" for command list\n", ORANGE);
	printColor("\nben@OS: $> ", GREEN);

	int count = 0;	
	char buffer[1024] = {0};
	while(1) {
		char c = getChar();
		if (c == '\n') {
			analizeBuffer(buffer);
			buffer[0] = 0;
			printColor("\nben@OS: $> ", GREEN);
			count = 0;
		} else if (c == '\b') {
			if (count > 0) {
				printChar(c);
				count--;
			}
		} else if (c == '\t') {
			// analize the first letter of the buffer, and if it matches with a command, complete it
			int i = 0;
			while (i < COMMANDS_QUANTITY && commandsNames[i][0] != buffer[0]) {
				i++;
			}
			if (i < COMMANDS_QUANTITY) {
				while (commandsNames[i][count] != 0) {
					buffer[count] = commandsNames[i][count];
					printColorChar(commandsNames[i][count++], 0xffffff);
				}
			}
		} else if (c != 0xFF && c > 0) {
			printColorChar(c, 0xffffff);
			buffer[count++] = c;
			buffer[count] = 0;
		}
	}

	return 0;
}

int strcmp(char * str1, char * str2) {
	int i = 0;
	while (str1[i] != 0 && str2[i] != 0) {
		if (str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}
	return str1[i] == str2[i];
}

void analizeBuffer(char * buffer) {
	if (strcmp(buffer, "help") || strcmp(buffer, "HELP")) {
		printColor("\n\nComandos disponibles:\n\n", YELLOW);
		for (int i = 0; i < COMMANDS_QUANTITY; i++) {
			printColor(commands[i], CYAN);
		}
	} else if (strcmp(buffer, "time")) {
		printfColor("\n\nTime of OS: ", YELLOW);
		printfColor("%s\n", CYAN, getTime());
	} else if (strcmp(buffer, "date")) {
		printfColor("\n\nDate of OS: ", YELLOW);
		printfColor("%s\n", CYAN, getDate());
	} else if (strcmp(buffer, "registers")) {
		printRegs();
	} else if (strcmp(buffer, "fillregs")) {
		fillRegs();
	} else if (strcmp(buffer, "clear")) {
		sys_clear_screen();
	} else if (strcmp(buffer, "pong")) {
		printColor("\nir a jugar al pong\n", GREEN);
	} else if (strcmp(buffer, "div0")) {
		divideByZero();
	} else if (strcmp(buffer, "invalidop")) {
		invalidOpcode();
	} else if (!strcmp(buffer, "")) {
		printColor("\nCommand not found. Type \"help\" for command list\n", RED);
	}
}