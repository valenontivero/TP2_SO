#ifndef COLORS_H
#define COLORS_H

#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define WHITE   0xFFFFFF

typedef struct Color {
    char r, g, b;
} Color;

extern Color white;

#endif