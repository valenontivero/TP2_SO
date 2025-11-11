#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_SIZE       512     // buffer size for keyboard
#define MAX_KEY_PRESSED   127

// Scancode definitions
#define LEFT_CTRL           0x1D
#define RIGHT_CTRL          0x1D  // right ctrl not separately handled (same scancode on PS/2 set 1)
#define RELEASE_LEFT_CTRL   0x9D
#define LEFT_SHIFT          0x2A
#define RIGHT_SHIFT         0x36
#define RELEASE_LEFT_SHIFT  0xAA
#define RELEASE_RIGHT_SHIFT 0xB6
#define CAPS_LOCK           0x3A

void initKeyboard();
void keyboard_handler();
char getChar();
char getCharNoBlock();

extern unsigned char getKey();

#endif