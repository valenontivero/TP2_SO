//#include <stdint.h>
#include <string.h>

extern unsigned char getKey(); // pasarlo a keyboard.h

#define BUFFER_SIZE 512

static char buffer[BUFFER_SIZE] = {0};
static int elemCount = 0;
static int readIndex = 0;
static int writeIndex = 0;

static const char charHexMap[256] = {       
        0,    0,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '-',  '=', '\b',
    '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '[',   ']', '\n',   
        0,     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\"',
    0,  0,  '\\',  'z',  'x', 'c', 'v',  'b',  'n',  'm',  ',',  '.',  '/',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, 
    0,     0,     0,    0,      0,      0,      0,      0,         'U'/*up*/,
    0,      0,      'L'/*left*/,    0,     'R'/*right*/,     0,      0,      'D'/*down*/,      0,
};


void keyboard_handler() {
    unsigned char key = getKey();
    if (key < 83) { // 83 elems in the charHexMap
        if (elemCount >= BUFFER_SIZE) return;  // buffer is full
        
        // make the array circular
        if (writeIndex >= BUFFER_SIZE)
            writeIndex = 0;

        buffer[writeIndex] = charHexMap[key];

        // update iterators
        elemCount++;
        writeIndex++;
    }
}

char getChar(){
    if (elemCount == 0) { 
        return 0xFF; // buffer is empty
    }

    char toReturn = buffer[readIndex];
    
    // update iterators
    elemCount--;
    readIndex++;

    // make the array circular
    if (readIndex == BUFFER_SIZE) readIndex = 0;
    
    return toReturn;
}