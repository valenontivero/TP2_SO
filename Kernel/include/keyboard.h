#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_SIZE 512

char isLetter(unsigned char key);

void keyboard_handler();

char getChar();

extern unsigned char getKey();

#endif