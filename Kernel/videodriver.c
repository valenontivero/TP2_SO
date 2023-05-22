
#include <stdint.h>
#include <font.h>
#include <videodriver.h>
#include <lib.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));


typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;


void putPixel(char r, char g, char b, int x, int y) {
    char * videoPtr = (char *) ((uint64_t)VBE_mode_info->framebuffer);
    int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
    videoPtr[offset] = b;
    videoPtr[offset+1] = g;
    videoPtr[offset+2] = r;
}

void drawWhiteLine() {
    for(int i = 0; i < VBE_mode_info->width * (VBE_mode_info->bpp / 8); i++) {
        putPixel(0xff, 0xff, 0xff, i, 3);
        putPixel(0xff, 0xff, 0xff, i, 4);
        putPixel(0xff, 0xff, 0xff, i, 5);
    }
}

void drawRect(int x, int y, int width, int height) {
	for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			putPixel(0xff, 0xff, 0xff, j, i);
		}
	}
}

/* void * getPixel(int x, int y) {
	return VBE_mode_info->framebuffer + (VBE_mode_info->bpp / 8) * (VBE_mode_info->width * y + x);
} */


int line = 0;
int column = 0;

#define MAX_LINES VBE_mode_info->height / CHAR_HEIGHT
#define MAX_COLUMNS VBE_mode_info->width / CHAR_WIDTH - 1

void moveCursor(int x);
void eraseCursor();
void drawCursor();

void printChar(char c, int x, int y) {
	eraseCursor();
	if (c == '\b') {
		if (x > 0) {
			column -= 2;
			for (int i = y; i < y + CHAR_HEIGHT; i++) {
				for (int j = x - CHAR_WIDTH; j < x; j++) {
					putPixel(0, 0, 0, j, i);
				}
			}
		} else if (line > 0) {
			line--;
			column = MAX_COLUMNS - 2;
			for (int i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
				for (int j = (column + 1) * CHAR_WIDTH; j < (MAX_COLUMNS + 1) * CHAR_WIDTH; j++) {
					putPixel(0, 0, 0, j, i);
				}
			}
		}
		moveCursor(-1);
		return;
	} else if (c == '\t') {
		if (column + 4 < MAX_COLUMNS) {
			column += 4;
		} else {
			line++;
			column = 0;
		}
		moveCursor(5);
		return;
	}

	if (c < FIRST_CHAR || c > LAST_CHAR )
		return;

	const unsigned char * charMap = font[c-32];
	for (int i = 0; i < CHAR_HEIGHT; i++) {
		char mask = 0b1000000;
		for (int j = 0; j < CHAR_WIDTH; j++) {
			if (*charMap & mask)
				putPixel(0xff, 0xff, 0xff, x+j, y+i);
			mask >>= 1;
		}
		charMap++;
	}
	moveCursor(1);
}

/* void printStr(char * string, int x, int y) {
	int line = 0;
	int i = 0, j = 0;
	while (string[i] != 0) {
		printChar(string[i], x + j * CHAR_WIDTH, y + line * CHAR_HEIGHT);
		i++; j++;
		if (x + j * CHAR_WIDTH > VBE_mode_info->width - 9) {
			line++;
			j = 0;
		}
	}
} */

void printString(char * string) {
	int i = 0;
	while (string[i] != 0) {
		if (string[i] == '\n') {
			line++;
			column = 0;
			eraseCursor();
			moveCursor(MAX_COLUMNS - column);
		} else {
			printChar(string[i], column * CHAR_WIDTH, line * CHAR_HEIGHT);
			column++;
			if (column > MAX_COLUMNS) {
				line++;
				column = 0;
			}
		}
		if (line >= MAX_LINES) {
			moveOneLineUp();
			eraseCursor();
			moveCursor(-(MAX_COLUMNS - column));
		}
		i++;
	}
}

void printStringN(char * string, uint64_t length) {
	int i = 0;
	while (string[i] != 0 && length > 0) {
		if (string[i] == '\n') {
			line++;
			column = 0;
			eraseCursor();
			moveCursor(MAX_COLUMNS - column);
		} else {
			printChar(string[i], column * CHAR_WIDTH, line * CHAR_HEIGHT);
			column++;
			if (column >= MAX_COLUMNS) {
				line++;
				column = 0;
			}
		}
		if (line >= MAX_LINES) {
			moveOneLineUp();
			eraseCursor();
			moveCursor(-(MAX_COLUMNS - column));
		}
		i++;
		length--;
	}
}

void printLn(char * string) {
	printString(string);
	line++;
	column = 0;
	if (line >= MAX_LINES) {
		moveOneLineUp();
	}
}

void moveOneLineUp() {
	char * dst = (char *) (uint64_t)(VBE_mode_info->framebuffer);
	char * src = dst + VBE_mode_info->pitch * CHAR_HEIGHT;
	for (int i = 0; i < VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT); i++) {
		dst[i] = src[i];
	}
	memset((void *) (uint64_t)(VBE_mode_info->framebuffer + VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT)), 0, VBE_mode_info->pitch * CHAR_HEIGHT);
	line--;
}

int cursorline = 0;
int cursorcolumn = 0;

void moveCursor(int x) {
	cursorcolumn += x;
	if (cursorcolumn > MAX_COLUMNS) {
		cursorline++;
		if (x == 1)
			cursorcolumn = 1;
		else
			cursorcolumn = 0;
	} else if (cursorcolumn < 0) {
		cursorline--;
		cursorcolumn = MAX_COLUMNS - 1;
	}
	drawCursor();
}

void drawCursor() {
	for (int i = cursorline * CHAR_HEIGHT; i < (cursorline + 1) * CHAR_HEIGHT; i++) {
		for (int j = cursorcolumn * CHAR_WIDTH; j < (cursorcolumn + 1) * CHAR_WIDTH; j++) {
			putPixel(0xff, 0xff, 0xff, j, i);
		}
	}
}

void eraseCursor() {
	for (int i = cursorline * CHAR_HEIGHT; i < (cursorline + 1) * CHAR_HEIGHT; i++) {
		for (int j = cursorcolumn * CHAR_WIDTH; j < (cursorcolumn + 1) * CHAR_WIDTH; j++) {
			putPixel(0, 0, 0, j, i);
		}
	}
}