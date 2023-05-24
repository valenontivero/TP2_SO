#include <usyscalls.h>
#include <userio.h>

extern void fillRegs();

void print(char* string) {
    sys_write(1, string, strlen(string));
}

void printColor(char* string, uint64_t color) {
    sys_write_color(1, string, strlen(string), color);
}

void printChar(char c) {
    char str[] = {c};
    sys_write(1, str, 1);
}

void printColorChar(char c, uint64_t color) {
    char str[] = {c};
    sys_write_color(1, str, 1, color);
}

char getChar() {
    char c[] = {0};
    sys_read(0, c, 1);
    return c[0];
}

unsigned int strlen(char* str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

// Convert 64 bit integer to hex string
void intToHex(uint64_t num, char* hex) {
    int i = 0;
    for (i = 15; i >= 0; i--) {
        int aux = num & 0xF;
        if (aux < 10) {
            hex[i] = aux + '0';
        } else {
            hex[i] = aux - 10 + 'A';
        }
        num >>= 4;
    }
    hex[16] = 0;
}

void printRegs() {
    fillRegs();
    uint64_t regs[17];
    char * regsnames[] = {"RAX ", "RBX ", "RCX ", "RDX ", "RSI ", "RDI ", "RBP ", "RSP ", "R8 ", "R9 ", "R10 ", "R11 ", "R12 ",
        "R13 ", "R14 ", "R15 ", "RIP "};
    sys_get_regs(regs);
    for (int i = 0; i < 17; i++) {
        char hex[17];
        intToHex(regs[i], hex);
        print(regsnames[i]);
        print(hex);
        print("\n");
    }
}