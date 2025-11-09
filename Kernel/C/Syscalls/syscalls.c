// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <videodriver.h>
#include <defs.h>
#include <syscalls.h>
#include <keyboard.h>
#include <clock.h>
#include <sound.h>
#include <ourTime.h>
#include <mySem.h>
#include <pipe.h>
#include <processManager.h>

extern const uint64_t registers[17];


uint64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused, uint64_t unused2, uint64_t unused3){
    if (fd != STDIN) 
        return -1;
    int i = 0;
    char c;
    char * buff = (char *) buffer;
    while(i < length && (c = getChar()) != 0) {
        buff[i] = c;
        i++;
    }
    return i;
}

uint64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused, uint64_t unused2, uint64_t unused3){
    if (fd == STDOUT) {
        printStringN((char *) buffer, length);
    } else if (fd == STDERR) {
        printStringNColor((char *) buffer, length, RED);
    }
    return 0;
}

uint64_t sys_write_place(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y, uint64_t unused3) {
    if (fd == STDOUT) {
        printStringPlace((char *) buffer, (int) x, (int) y, WHITE);
    } else if (fd == STDERR) {
        printStringPlace((char *) buffer, (int) x, (int) y, RED);
    }
    return 0;
}

uint64_t sys_write_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color, uint64_t unused2, uint64_t unused3) {
    if (fd == STDOUT || fd == STDERR) {
        Color c;
        c.r = (char) color;
        c.g = (char) (color >> 8);
        c.b = (char) (color >> 16);
        printStringNColor((char *) buffer, length, c);
    }
    return 0;
}

uint64_t sys_get_registers(uint64_t regsBuff, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    for(int i = 0; i < 17; i++) {
        ((uint64_t *)regsBuff)[i] = registers[i];
    }
    return 0; 
}

uint64_t sys_get_time(uint64_t buffer, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    timeToStr((char *) buffer);
    return 0;
}

uint64_t sys_get_date(uint64_t buffer, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    dateToStr((char *) buffer);
    return 0;
}

uint64_t sys_clear_screen(uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5, uint64_t unused6) {
    clearScreen();
    return 0;
}

uint64_t sys_draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color, uint64_t unused1) {
    drawRect( (int) x, (int) y, (int) width, (int) height, (int) color );
    return 0;
}

uint64_t sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter,uint64_t unused1, uint64_t unused2, uint64_t unused3) {
    playNote((int) freq, (int) duration, (int) waitAfter);
    return 0;
}

uint64_t sys_get_screensize(uint64_t width, uint64_t height, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    uint16_t * w = (uint16_t *) width;
    uint16_t * h = (uint16_t *) height;
    *w = getWidth();
    *h = getHeight();
    return 0;
}

uint64_t sys_toggle_cursor(uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5, uint64_t unused6){
    toggleCursor();
    return 0;
}

uint64_t sys_get_ticks(uint64_t ticks,uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    uint32_t * t = (uint32_t *) ticks;    
    *t = ticks_elapsed();
    return 0;
}

uint64_t sys_draw_image(uint64_t image, uint64_t width, uint64_t height, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    drawImage((const unsigned long int *) image, (int) width, (int) height);
    return 0;
}

// Semaforos
uint64_t sys_sem_open(uint64_t name, uint64_t initial_value, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    const char *nameStr = (const char *) name;
    uint8_t id = sem_open(nameStr, (uint8_t) initial_value);
    if (id >= MAX_SEMAPHORES) {
        return (uint64_t)-1;
    }
    return (uint64_t) id;
}

uint64_t sys_sem_wait(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    int result = sem_wait((uint8_t) id);
    if (result < 0) {
        return (uint64_t)-1;
    }
    return (uint64_t) 0;
}

uint64_t sys_sem_post(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    int result = sem_post((uint8_t) id);
    if (result < 0) {
        return (uint64_t)-1;
    }
    return (uint64_t) 0;
}

uint64_t sys_sem_close(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    if (id >= MAX_SEMAPHORES) {
        return (uint64_t)-1;
    }
    sem_destroy((uint8_t) id);
    return (uint64_t) 0;
}

uint64_t sys_create_process(uint64_t entryPoint, uint64_t prio, uint64_t argc, uint64_t argv, uint64_t unused4, uint64_t unused5) {
    void (*fn)(uint8_t, char **) = (void (*)(uint8_t, char **)) entryPoint;
    char ** args = (char **) argv;
    pid_t pid = createProcess(fn, (int) prio, (int) argc, args, args[0]);
    if (pid < 0) {
        return (uint64_t)-1;
    }
    return pid;
}

uint64_t sys_pipe_open(uint64_t name, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    const char *pipeName = (const char *)name;
    return (uint64_t)pipe_open(pipeName);
}

uint64_t sys_pipe_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    return pipe_read((int)fd, (char *)buffer, length);
}

uint64_t sys_pipe_write(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    return pipe_write((int)fd, (const char *)buffer, length);
}

uint64_t sys_pipe_close(uint64_t fd, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    pipe_close((int)fd);
    return 0;
}
uint64_t sys_wait(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    wait((pid_t) pid);
    return 0;
}

uint64_t sys_put_in_fg(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    putInFG((pid_t) pid);
    return 0;
}

uint64_t sys_timer_wait(uint64_t seconds, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5) {
    timer_wait((int)(seconds * TICKS_FREQUENCY));
    return 0;
}