#include <usyscalls.h>
#include <userio.h>
#include <stdarg.h>
#include <colors.h>

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

void putChar(char c) {
    sys_write(1, &c, 1);
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
    uint64_t regs[17];
    char * regsnames[] = {"RAX ", "RBX ", "RCX ", "RDX ", "RSI ", "RDI ", "RBP ", "RSP ", "R8  ", "R9  ", "R10 ", "R11 ", "R12 ",
        "R13 ", "R14 ", "R15 ", "RIP "};
    print("\n");
    sys_get_regs(regs);
    for (int i = 0; i < 17; i++) {
        char hex[17];
        intToHex(regs[i], hex);
        print(regsnames[i]);
        print(hex);
        print("\n");
    }
}

void intToStr(int num, char* str) {
    int i = 0;
    if (num < 0) {
        str[i] = '-';
        i++;
        num = -num;
    }
    int aux = num;
    while (aux > 0) {
        aux /= 10;
        i++;
    }
    str[i] = 0;
    i--;
    while (num > 0) {
        str[i] = num % 10 + '0';
        num /= 10;
        i--;
    }
}

void printfColor(char* format, uint64_t color, ...) {
    va_list args;
    va_start(args, color);
    int i = 0;
    while (format[i] != 0) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'd': {
                    int num = va_arg(args, int);
                    char str[11];
                    intToStr(num, str);
                    printColor(str, color);
                    break;
                }
                case 'x': {
                    int num = va_arg(args, int);
                    char str[9];
                    intToHex(num, str);
                    printColor(str, color);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    printColor(str, color);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    printColorChar(c, color);
                    break;
                }
                default:
                    printColorChar(format[i], color);
                    break;
            }
        } else {
            printColorChar(format[i], color);
        }
        i++;
    }
    va_end(args);
}

void printf(char* format, ...) {
    va_list args;
    va_start(args, format);
    int i = 0;
    while (format[i] != 0) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'd': {
                    int num = va_arg(args, int);
                    char str[11];
                    intToStr(num, str);
                    printColor(str, WHITE);
                    break;
                }
                case 'x': {
                    int num = va_arg(args, int);
                    char str[9];
                    intToHex(num, str);
                    printColor(str, WHITE);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    printColor(str, WHITE);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    printColorChar(c, WHITE);
                    break;
                }
                default:
                    printColorChar(format[i], WHITE);
                    break;
            }
        } else {
            printColorChar(format[i], WHITE);
        }
        i++;
    }
    va_end(args);
}
