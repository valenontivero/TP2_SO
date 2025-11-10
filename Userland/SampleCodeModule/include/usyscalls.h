#ifndef USYSCALLS_H
#define USYSCALLS_H

#include <stdint.h>
#include <types.h>

uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size);

uint64_t sys_write(unsigned int fd, const char* buffer, unsigned int size);

uint64_t sys_write_color(unsigned int fd, const char* buffer, unsigned int size, uint64_t color);

uint64_t sys_get_regs(uint64_t * regsBuff);

uint64_t sys_get_time(char *buffer);

uint64_t sys_get_date(char *buffer);

uint64_t sys_clear_screen();

uint64_t sys_draw_rectangle(int x, int y, int width, int height, int color);

uint64_t sys_play_sound(int freq, int duration, int waitAfter);

uint64_t sys_get_screen_size(uint16_t * width, uint16_t * height);

uint64_t sys_toggle_cursor();

uint64_t sys_get_ticks(uint32_t * ticks);

uint64_t sys_write_place(unsigned int fd, const char* buffer, unsigned int size, int x, int y);

uint64_t sys_draw_image(const unsigned long * image, int width, int height);

uint64_t sys_sem_open(const char *name, uint8_t initial_value);

uint64_t sys_sem_wait(uint8_t id);

uint64_t sys_sem_post(uint8_t id);

uint64_t sys_sem_close(uint8_t id);

uint64_t sys_launch_process(void* entryPoint, uint8_t prio, uint8_t argc, char** argv);

uint64_t sys_pipe_open(const char *name);

uint64_t sys_pipe_read(unsigned int fd, char *buffer, uint64_t size);

uint64_t sys_pipe_write(unsigned int fd, const char *buffer, uint64_t size);

uint64_t sys_pipe_close(unsigned int fd);
uint64_t sys_wait(pid_t pid);

uint64_t sys_put_in_fg(pid_t pid);

uint64_t sys_timer_wait(int seconds);

uint64_t sys_change_process_fd(uint64_t pid, uint64_t fd, uint64_t end);

uint64_t sys_get_mem_info(memoryData *info);

uint64_t sys_get_process_list(processInfo *buffer, uint16_t maxCount, uint16_t *written);

uint64_t sys_get_process_info(pid_t pid, processInfo *info);

uint64_t sys_get_pid();

uint64_t sys_process_kill(pid_t pid);

uint64_t sys_process_nice(pid_t pid, uint8_t newPriority);

uint64_t sys_process_block(pid_t pid);

uint64_t sys_process_unblock(pid_t pid);

#endif