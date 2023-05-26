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

void printStringNColor(char * buffer, uint64_t length, Color color);

void moveCursor();

void eraseCursor();

void clearScreen();

#endif