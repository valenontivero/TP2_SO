#include <stdint.h>
#include <videodriver.h>
#include <defs.h>
#include <syscalls.h>
#include <keyboard.h>


void syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    switch(id) {
        case 0:
            sys_read(arg0, arg1, arg2);
            break;
        case 1:
            sys_write(arg0, arg1, arg2);
            break;
        case 2:
            sys_write_color(arg0, arg1, arg2, arg3);
            break;
            /*
        case 3:

        case 4:
        */
    }
    //ver de agregar excepción si no existe el id
}


static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd != STDIN) 
        return -1;
    int i = 0;
    char c;
    char * buff = (char *) buffer;
    while(i < length && (c = getChar()) != 0xFF) {
        buff[i] = c;
        i++;
    }
    return i;
}

static void sys_write(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd == STDOUT) {
        printStringN((char *) buffer, length);
    }
}

static void sys_write_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color) {
    if (fd == STDOUT) {
        Color c;
        c.r = (char) color;
        c.g = (char) (color >> 8);
        c.b = (char) (color >> 16);
        printStringNColor((char *) buffer, length, c);
    }
}

