// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <usyscalls.h>
#include <types.h>

extern uint64_t sys_call(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size) {
    return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write(unsigned int fd, const char* buffer, unsigned int size) {
    return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write_color(unsigned int fd, const char* buffer, unsigned int size, uint64_t color) {
    /* uint64_t c = color.r;
    c <<= 2;
    c += color.g;
    c <<= 2;
    c += color.b; */
    return sys_call((uint64_t) 2, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, color, (uint64_t) 0);
}

uint64_t sys_get_regs(uint64_t * regsBuff) {
    return sys_call((uint64_t) 3, (uint64_t) regsBuff, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_time(char * buffer) {
    return sys_call((uint64_t)4, (uint64_t)buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_date(char * buffer) {
    return sys_call((uint64_t)5, (uint64_t)buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_clear_screen() {
    return sys_call((uint64_t)6, (uint64_t)0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_draw_rectangle(int x, int y, int width, int height, int color) {
    return sys_call((uint64_t) 7, (uint64_t) x, (uint64_t) y, (uint64_t) width, (uint64_t) height, (uint64_t) color);
}

uint64_t sys_play_sound(int freq, int duration, int waitAfter) {
    return sys_call((uint64_t) 8, (uint64_t) freq, (uint64_t) duration, (uint64_t) waitAfter, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_screen_size(uint16_t * width, uint16_t * height) {
    return sys_call((uint64_t) 9, (uint64_t) width, (uint64_t) height, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_toggle_cursor() {
    return sys_call((uint64_t) 10, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_ticks(uint32_t * ticks) {
    return sys_call((uint64_t) 11, (uint64_t) ticks, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write_place(unsigned int fd, const char* buffer, unsigned int size, int x, int y) {
    return sys_call((uint64_t) 12, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) x, (uint64_t) y);
}

uint64_t sys_draw_image(const unsigned long * image, int width, int height) {
    return sys_call((uint64_t) 13, (uint64_t) image, (uint64_t) width, (uint64_t) height, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_sem_open(const char *name, uint8_t initial_value) {
    return sys_call((uint64_t) 14, (uint64_t) name, (uint64_t) initial_value, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_sem_wait(uint8_t id) {
    return sys_call((uint64_t) 15, (uint64_t) id, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_sem_post(uint8_t id) {
    return sys_call((uint64_t) 16, (uint64_t) id, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_sem_close(uint8_t id) {
    return sys_call((uint64_t) 17, (uint64_t) id, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_launch_process(void* entryPoint, uint8_t prio, uint8_t argc, char** argv) {
    return sys_call((uint64_t) 18, (uint64_t) entryPoint, (uint64_t) prio, (uint64_t) argc, (uint64_t) argv, (uint64_t) 0);
}

uint64_t sys_pipe_open(const char *name) {
    return sys_call((uint64_t) 19, (uint64_t) name, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_pipe_read(unsigned int fd, char *buffer, uint64_t size) {
    return sys_call((uint64_t) 20, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_pipe_write(unsigned int fd, const char *buffer, uint64_t size) {
    return sys_call((uint64_t) 21, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_pipe_close(unsigned int fd) {
    return sys_call((uint64_t) 22, (uint64_t) fd, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_wait(pid_t pid) {
    return sys_call((uint64_t) 23, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_put_in_fg(pid_t pid) {
    return sys_call((uint64_t) 24, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_timer_wait(int seconds) {
    return sys_call((uint64_t) 25, (uint64_t) seconds, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_change_process_fd(uint64_t pid, uint64_t fd, uint64_t end) {
	return sys_call(pid, fd, end, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}