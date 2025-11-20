// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <colors.h>
#include <userio.h>
#include <usyscalls.h>
#include <colors.h>
#include <sounds.h>
#include <pong.h>
#include <shell.h>
#include <types.h>
#include <stddef.h>
#include <processLib.h>
#include <uStrings.h>
#include <shellFunc.h>
#include <uSync.h>

//
// Necesarry functions declarations
//

static void parseInput(char *input, ParsedCommand *out);
static void (*getFn(const char *cmd))(uint8_t, char **);
static int countArgs(char **argv);
void shell();


//
// Structures to map commands to functions
//

static void  (*instructionFunctions[])(uint8_t, char **) = {
	help,
	time,
	date,
	registers, 
	fillregs, 
	div0, 
	invalidop, 
	pong, 
	clear,
    hello,
    testmm,
    testprocesses,
    testsynchro,
    testnosynchro,
    testpriority,
    mem,
    ps,
    loop,
    kill,
    nice,
    block,
    wc,
    filter,
    cat,
	echo,
    mvar,
    yield_cmd,
	NULL};

static char *commandsNames[] = {"help", "time", "date", "registers", "fillregs", "div0", "invalidop", "pong", "clear", "hello", "testmm", "testprocesses", "testsynchro", "testnosynchro", "testpriority", "mem", "ps", "loop", "kill", "nice", "block", "wc", "filter", "cat","echo", "mvar", "yield", 0};


//
// Shell implentation variables
//

static pid_t fgProccess=0;
static int hasToWait = 1;
static int pipeCounter=0;

//
// Shell implementation
//


pid_t launchShell(){
	char* argv[] = {"shell"};
	return  createProcess((void*) shell, 4, 0, argv);
}

void shell() {
    printColor("Welcome to HomerOS. Type \"help\" for command list\n", ORANGE);
	pid_t shellPid = sys_get_pid();
	sys_put_in_fg(shellPid,0);
	sys_process_set_foreground(shellPid, 1);
	printColor("\nHomerOS: $> ", GREEN);

	int count = 0;	
	char buffer[1024] = {0};
	char oldBuffer[1024] = {0};
	char flag = 0; // Used for up arrow
	while(1) {
		unsigned char c = getChar();
		if (c == 3) { // Ctrl + C
			printColor("^C\n", ORANGE);
			if (fgProccess != 0) {
				killProcess(fgProccess);
				sys_process_set_foreground(shellPid, 1);
				sys_put_in_fg(shellPid,0);
				fgProccess = 0;
			}
			count = 0;
			buffer[0] = 0;
			printColor("HomerOS: $> ", GREEN);
		} else if (c == 4) { // Ctrl + D at prompt
			count = 0;
			buffer[0] = 0;
			printColor("\nHomerOS: $> ", GREEN);
		} else if (c == '\n') {
			printChar('\n');
			buffer[count] = 0;
			analizeBuffer(buffer, count);
			printColor("\nHomerOS: $> ", GREEN);
			strcpy(oldBuffer, buffer);
			flag = 1;
			count = 0;
		} else if (c == '\b') {
			if (count > 0) {
				printChar(c);
				count--;
			}
		} else if (c == '\t') {
			// analize count letters of the buffer and see if match with any command, if so, complete the command
			int i = 0;
			while (i < COMMANDS_QUANTITY && !strncmp(buffer, commandsNames[i], count)) {
				i++;
			}
			if (i < COMMANDS_QUANTITY) {
				while (commandsNames[i][count] != 0) {
					printChar(commandsNames[i][count]);
					buffer[count] = commandsNames[i][count];
					count++;
				}
			}
		} else if (c == 17 && flag) {
			// up arrow
			while (count > 0) {
				printChar('\b');
				count--;
			}
			strcpy(buffer, oldBuffer);
			count = strlen(buffer);
			print(buffer);
			flag = 0;
		} else if (c == 20 && !flag) {
			// down arrow
			while (count > 0) {
				printChar('\b');
				count--;
			}
			flag = 1;
		} else if (c > 20 && c < 127) {
			printChar(c);
			buffer[count++] = c;
			buffer[count] = 0;
		}
	}
}


/* Deprecated
int commandMatch(char * str1, char * command, int count) {
	int i = 0;
	if (count != strlen(command))
		return 0;
	while (str1[i] != 0 && command[i] != 0 && str1[i] == command[i] && i < count) {
		i++;
	}
	return str1[i] == command[i];
}
*/

void analizeBuffer(char * buffer, int count) {
	if (count <= 0)
		return;
	ParsedCommand parsed= {0};
	parseInput(buffer, &parsed);

	     
	hasToWait= !parsed.isBackground;

	
	
	if (parsed.hasPipe) { 
		char name[16];
		unsigned_num_to_str(pipeCounter,0,name);
		uint8_t anonPipe=sys_pipe_open(name);
		pipeCounter++;
		
		
		void (*fn1)(uint8_t, char **) = getFn(parsed.cmd1);
		void (*fn2)(uint8_t, char **) = getFn(parsed.cmd2);

		if (!fn1 || !fn2) {
			printf("Unknown command in pipe: %s | %s\n", parsed.cmd1, parsed.cmd2);
			return;
		}

		pid_t shellPid = sys_get_pid();

		pid_t p1 = sys_launch_process(fn1, DEFAULT_PRIO ,countArgs(parsed.args1), parsed.args1);
		sys_process_set_foreground(p1, 0);
		sys_change_process_fd(p1,anonPipe,1);

		pid_t p2 = sys_launch_process(fn2, DEFAULT_PRIO ,countArgs(parsed.args2), parsed.args2);
		sys_process_set_foreground(p2, parsed.isBackground ? 0 : 1);
		sys_change_process_fd(p1,anonPipe,1);
		sys_change_process_fd(p2,anonPipe,0);

		if (!parsed.isBackground) {
			sys_process_set_foreground(shellPid, 0);
			fgProccess = p2;
			putInFg(p2,p1);
			wait(p2);
			sys_process_set_foreground(shellPid, 1);
		} else {
			fgProccess = 0;
		}

		if (hasToWait && fgProccess != 0){
			wait(fgProccess);
		}
		
		return;
	}
	void (*fn)(uint8_t, char **) = getFn(parsed.cmd1);

	if ( fn == NULL) {
		printf("Command not found: '%s'\n", parsed.cmd1);
		fgProccess = 0;
		return;
	}

	pid_t shellPid = sys_get_pid();

	pid_t pid = createProcess(fn, DEFAULT_PRIO, countArgs(parsed.args1), parsed.args1);
	sys_process_set_foreground(pid, parsed.isBackground ? 0 : 1);
	if (!parsed.isBackground){
		sys_process_set_foreground(shellPid, 0);
		fgProccess = pid;
		putInFg(pid,0);
	}
	if (hasToWait && fgProccess != 0)
	{
		wait(fgProccess);
		sys_process_set_foreground(shellPid, 1);
		sys_put_in_fg(shellPid,0);
		fgProccess = 0;
	}
}


static int countArgs(char **argv) {
	int count = 0;
	while (argv[count]) {
		count++;
	}
	return count;
}

static void parseInput(char *input, ParsedCommand *out) {
    out->hasPipe = 0;
    out->isBackground = 0;
    memset(out->args1, 0, sizeof(out->args1));
    memset(out->args2, 0, sizeof(out->args2)); 

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '&') {
        out->isBackground = 1;
        input[len - 1] = 0;
    }

    char *pipePos = strchr(input, '|');
    if (pipePos) {
        out->hasPipe = 1;
        *pipePos = 0;
        pipePos++;
    }

    int i = 0;
    char *tok = strtok(input, " \t");
    while (tok && i < MAX_ARGS - 1) {
        out->args1[i++] = tok;
        tok = strtok(NULL, " \t");
    }
    out->cmd1 = out->args1[0];

    if (out->hasPipe) {
        i = 0;
        tok = strtok(pipePos, " \t");
        while (tok && i < MAX_ARGS - 1) {
            out->args2[i++] = tok;
            tok = strtok(NULL, " \t");
        }
        out->cmd2 = out->args2[0];
    }
}

static void (*getFn(const char *cmd))(uint8_t, char **) {
    for (int i = 0; commandsNames[i] != NULL; i++) {
        if (strcmp(commandsNames[i], cmd) == 0) {
            return instructionFunctions[i];
        }
    }
    return NULL;
}