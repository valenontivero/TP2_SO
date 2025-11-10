#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
uint64_t syscallDispatcher(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

uint64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused, uint64_t unused2, uint64_t unused3);

uint64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused, uint64_t unused2, uint64_t unused3);

uint64_t sys_write_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color, uint64_t unused2, uint64_t unused3);

uint64_t sys_write_place(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y, uint64_t unused3); 

uint64_t sys_get_registers(uint64_t regsBuff, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_time(uint64_t buffer, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_date(uint64_t buffer, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_clear_screen(uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5, uint64_t unused6);

uint64_t sys_draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color, uint64_t unused1);

uint64_t sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter,uint64_t unused1, uint64_t unused2, uint64_t unused3);

uint64_t sys_get_screensize(uint64_t width, uint64_t height, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_toggle_cursor(uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5, uint64_t unused6);

uint64_t sys_get_ticks(uint64_t ticks,uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_timer_wait(uint64_t seconds, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);


uint64_t sys_draw_image(uint64_t image, uint64_t width, uint64_t height, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_sem_open(uint64_t name, uint64_t initial_value, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_sem_wait(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_sem_post(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_sem_close(uint64_t id, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);


uint64_t sys_create_process(uint64_t entryPoint, uint64_t prio, uint64_t argc, uint64_t argv, uint64_t unused4, uint64_t unused5);

uint64_t sys_pipe_open(uint64_t name, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_pipe_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_pipe_write(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_pipe_close(uint64_t fd, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);
uint64_t sys_put_in_fg(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_wait(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);


uint64_t sys_change_process_fd(uint64_t pid, uint64_t fd, uint64_t end, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_mem_info(uint64_t buffer, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_process_list(uint64_t buffer, uint64_t maxCount, uint64_t countPtr, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_process_info(uint64_t pid, uint64_t outPtr, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_get_pid(uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5, uint64_t unused6);

uint64_t sys_process_kill(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_process_nice(uint64_t pid, uint64_t newPriority, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_process_block(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

uint64_t sys_process_unblock(uint64_t pid, uint64_t unused1, uint64_t unused2, uint64_t unused3, uint64_t unused4, uint64_t unused5);

#endif