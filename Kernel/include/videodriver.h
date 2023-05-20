#ifndef _VIDEODRIVER_H
#define _VIDEODRIVER_H

#define red     0xFF0000
#define green   0x00FF00
#define blue    0x0000FF
#define white   0xFFFFFF

void putPixel(char r, char g, char b, int x, int y);

void drawWhiteLine();

void drawRect(int x, int y, int width, int height);

void printChar(char c, int x, int y);

#endif