// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <syscalls.h>

// Si quieron agregar un syscall, agregarlo en el enum y en el array, codear la función en syscalls.c y el prototipo en syscalls.h.
enum syscallsEnum {
    SYSCALL_READ = 0,
    SYSCALL_WRITE,
    SYSCALL_WRITE_COLOR,
    SYSCALL_GET_REGISTERS,
    SYSCALL_GET_TIME,
    SYSCALL_GET_DATE,
    SYSCALL_CLEAR_SCREEN,
    SYSCALL_DRAW_RECT,
    SYSCALL_PLAY_SOUND,
    SYSCALL_GET_SCREENSIZE,
    SYSCALL_TOGGLE_CURSOR,
    SYSCALL_GET_TICKS,
    SYSCALL_WRITE_PLACE,
    SYSCALL_DRAW_IMAGE,
    SYSCALL_SEM_OPEN,
    SYSCALL_SEM_WAIT,
    SYSCALL_SEM_POST,
    SYSCALL_SEM_CLOSE,
    SYSCALL_CREATE_PROCESS,
    SYSCALL_PIPE_OPEN,
    SYSCALL_PIPE_READ,
    SYSCALL_PIPE_WRITE,
    SYSCALL_PIPE_CLOSE
};

typedef uint64_t (*sys_call_t)(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

static sys_call_t syscallsArray[] = {   // id
        [SYSCALL_READ] (sys_call_t) sys_read, // 0
        [SYSCALL_WRITE] (sys_call_t) sys_write, // 1
        [SYSCALL_WRITE_COLOR] (sys_call_t) sys_write_color, // 2
        [SYSCALL_WRITE_PLACE] (sys_call_t) sys_write_place, // 3
        [SYSCALL_GET_REGISTERS] (sys_call_t) sys_get_registers, // 4
        [SYSCALL_GET_TIME] (sys_call_t) sys_get_time, // 5
        [SYSCALL_GET_DATE] (sys_call_t) sys_get_date, // 6
        [SYSCALL_CLEAR_SCREEN] (sys_call_t) sys_clear_screen, // 7
        [SYSCALL_DRAW_RECT] (sys_call_t) sys_draw_rect, // 8
        [SYSCALL_PLAY_SOUND] (sys_call_t) sys_play_sound, // 9
        [SYSCALL_GET_SCREENSIZE] (sys_call_t) sys_get_screensize, // 10
        [SYSCALL_TOGGLE_CURSOR] (sys_call_t) sys_toggle_cursor, // 11
        [SYSCALL_GET_TICKS] (sys_call_t) sys_get_ticks, // 12
        [SYSCALL_DRAW_IMAGE] (sys_call_t) sys_draw_image, // 13
        [SYSCALL_SEM_OPEN] (sys_call_t) sys_sem_open, // 14
        [SYSCALL_SEM_WAIT] (sys_call_t) sys_sem_wait, // 15
        [SYSCALL_SEM_POST] (sys_call_t) sys_sem_post, // 16
        [SYSCALL_SEM_CLOSE] (sys_call_t) sys_sem_close, // 17
        [SYSCALL_CREATE_PROCESS] (sys_call_t) sys_create_process, // 18
        [SYSCALL_PIPE_OPEN] (sys_call_t) sys_pipe_open, // 19
        [SYSCALL_PIPE_READ] (sys_call_t) sys_pipe_read, // 20
        [SYSCALL_PIPE_WRITE] (sys_call_t) sys_pipe_write, // 21
        [SYSCALL_PIPE_CLOSE] (sys_call_t) sys_pipe_close // 22
    };

uint64_t syscallDispatcher(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    if (id < sizeof(syscallsArray) / sizeof(sys_call_t)) {
        return syscallsArray[id](arg0, arg1, arg2, arg3, arg4, arg5);
    }
    return -1;  // ID inválido
}
