#ifndef _VIDEODRIVER_H
#define _VIDEODRIVER_H

#include <colors.h>
#include <stdint.h>

void putPixel(char r, char g, char b, int x, int y);

void drawWhiteLine();

void drawRect(int x, int y, int width, int height);

void printString(char * string);

void printStringN(char * string, uint64_t length);

void printLn(char * string);

void moveOneLineUp();

void printStringColor(char * string, Color color);

void printStringNColor(char * buffer, uint64_t length, Color color);

void moveCursor();

void eraseCursor();

void clearScreen();

// void drawImage(int * image);

#endif