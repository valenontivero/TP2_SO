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
    SYSCALL_DRAW_IMAGE
};

typedef uint64_t (*sys_call_t)(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

static sys_call_t syscallsArray[] = {
        [SYSCALL_READ] (sys_call_t) sys_read,
        [SYSCALL_WRITE] (sys_call_t) sys_write,
        [SYSCALL_WRITE_COLOR] (sys_call_t) sys_write_color,
        [SYSCALL_WRITE_PLACE] (sys_call_t) sys_write_place,
        [SYSCALL_GET_REGISTERS] (sys_call_t) sys_get_registers,
        [SYSCALL_GET_TIME] (sys_call_t) sys_get_time,
        [SYSCALL_GET_DATE] (sys_call_t) sys_get_date,
        [SYSCALL_CLEAR_SCREEN] (sys_call_t) sys_clear_screen,
        [SYSCALL_DRAW_RECT] (sys_call_t) sys_draw_rect,
        [SYSCALL_PLAY_SOUND] (sys_call_t) sys_play_sound,
        [SYSCALL_GET_SCREENSIZE] (sys_call_t) sys_get_screensize,
        [SYSCALL_TOGGLE_CURSOR] (sys_call_t) sys_toggle_cursor,
        [SYSCALL_GET_TICKS] (sys_call_t) sys_get_ticks,
        [SYSCALL_DRAW_IMAGE] (sys_call_t) sys_draw_image
    };

uint64_t syscallDispatcher(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    if (id < sizeof(syscallsArray) / sizeof(sys_call_t)) {
        return syscallsArray[id](arg0, arg1, arg2, arg3, arg4, arg5);
    }
    return -1;  // ID inválido
}
