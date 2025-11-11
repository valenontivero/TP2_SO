// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <string.h>
#include <keyboard.h>
#include <videodriver.h>
#include <colors.h>
#include <mySem.h>
#include <processManager.h>
#include <lib.h>

static char buffer[BUFFER_SIZE] = {0};
static int elemCount = 0;
static int readIndex = 0;
static int writeIndex = 0;

static char shiftPressed = 0;
static char capsLocked = 0;
static char ctrlPressed = 0;

static int semaphoreId = -1;

static const char charHexMap[256] = {
        0,  1/*esc*/,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '\'',  '<', '\b',
    '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '\\',   '+', '\n',
        0,     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '{',
    '|',  5/*shift*/,  '}',  'z',  'x', 'c', 'v',  'b',  'n',  'm',  ',',  '.',  '-',    0,
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0,
    0,     0,     0,    0,      0,      0,      0,      0,         17/*up*/,
    0,      0,      18/*left*/,    0,     19/*right*/,     0,      0,      20/*down*/,      0,
};

static const char charCapsHexMap[256] = {
        0,    1,  '!',  '"',  '#',  '$',  '%',  '&',   '/',  '(',  ')',   '=',   '?',  '>', '\b',
    '\t', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',   'O',  'P',  '^',   '*', '\n',
        0,     'A',  'S', 'D',  'F',  'G',  'H',  'J',  'K',  'L',  '~',  '[',
    '/',  5/*shift*/,  ']',  'Z',  'X', 'C', 'V',  'B',  'N',  'M',  ';',  ':',  '_',    0,
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0,
    0,     0,     0,    0,      0,      0,      0,      0,         17/*up*/,
    0,      0,      18/*left*/,    0,     19/*right*/,     0,      0,      20/*down*/,      0,
};

static int isLetter(unsigned char key) {
    char c = charHexMap[key];
    return (c >= 'a' && c <= 'z');
}

static void pushChar(char c) {
    if (elemCount >= BUFFER_SIZE) {
        return;
    }
    buffer[writeIndex] = c;
    writeIndex = (writeIndex + 1) % BUFFER_SIZE;
    elemCount++;
    if (semaphoreId >= 0) {
        sem_post((uint8_t)semaphoreId);
    }
}

static void handleCtrlC(void) {
    PCB *fg = getForegroundProcess();
    if (fg != NULL && fg->pid != 0 && strcmp(fg->name, "shell") != 0) {
        killProcessInFG();
        return;
    }
    if (currentProcess != NULL && currentProcess->pid != 0 && strcmp(currentProcess->name, "shell") != 0) {
        killProcess(currentProcess->pid);
    }
}

void keyboard_handler() {
    unsigned char key = getKey();

    if (key == CAPS_LOCK) {
        capsLocked = !capsLocked;
        return;
    }

    if (key == LEFT_SHIFT || key == RIGHT_SHIFT) {
        shiftPressed = 1;
        return;
    }

    if (key == LEFT_CTRL) {
        if (!ctrlPressed) {
            ctrlPressed = 1;
        }
        return;
    }

    if (key == RELEASE_LEFT_SHIFT || key == RELEASE_RIGHT_SHIFT) {
        shiftPressed = 0;
        return;
    }

    if (key == RELEASE_LEFT_CTRL) {
        if (ctrlPressed) {
            ctrlPressed = 0;
        }
        return;
    }

    if (key & 0x80) {
        return;
    }

    if (key >= 256) {
        return;
    }

    char outputChar;
    if (isLetter(key)) {
        if ((shiftPressed && !capsLocked) || (!shiftPressed && capsLocked)) {
            outputChar = charCapsHexMap[key];
        } else {
            outputChar = charHexMap[key];
        }
    } else {
        outputChar = shiftPressed ? charCapsHexMap[key] : charHexMap[key];
    }

    if (ctrlPressed) {
        if (outputChar >= 'a' && outputChar <= 'z') {
            outputChar = outputChar - 'a' + 1;
        } else if (outputChar >= 'A' && outputChar <= 'Z') {
            outputChar = outputChar - 'A' + 1;
        }

        if (outputChar == 3) { // Ctrl+C
            handleCtrlC();
            pushChar(outputChar);
            return;
        }

        if (outputChar == 4) { // Ctrl+D
            pushChar(outputChar);
            return;
        }

        if (outputChar == 12) { // Ctrl+L
            clearScreen();
            printStringColor("\nHomerOS: $> ", GREEN);
            return;
        }
    }

    pushChar(outputChar);
}

char getChar() {
    if (semaphoreId < 0) {
        if (elemCount == 0) {
            return 0;
        }
        char toReturn = buffer[readIndex];
        readIndex = (readIndex + 1) % BUFFER_SIZE;
        elemCount--;
        return toReturn;
    }

    sem_wait((uint8_t)semaphoreId);

    if (elemCount == 0) {
        return 0;
    }

    char toReturn = buffer[readIndex];
    readIndex = (readIndex + 1) % BUFFER_SIZE;
    elemCount--;
    return toReturn;
}

char getCharNoBlock() {
    if (elemCount == 0) {
        return 0;
    }
    char toReturn = buffer[readIndex];
    readIndex = (readIndex + 1) % BUFFER_SIZE;
    elemCount--;
    return toReturn;
}

void initKeyboard() {
    if (semaphoreId < 0) {
        uint8_t id = sem_open("keyboard", 0);
        if (id == (uint8_t)-1) {
            semaphoreId = -1;
        } else {
            semaphoreId = id;
        }
    }
}