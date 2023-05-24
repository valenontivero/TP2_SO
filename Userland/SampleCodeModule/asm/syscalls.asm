section .text
GLOBAL sys_call
GLOBAL fillRegs

sys_call:
    push rbp
    mov rbp, rsp
    int 80h

    mov rsp, rbp
    pop rbp
    ret

fillRegs:
    mov rax, 0x683F
    mov rcx, 0xad19
    mov rdx, 0x9123
    mov r8, 0x5121
    mov r11, 0x123456789ABCDEF
    mov r10, 0xabcd9832
    mov r9, 0xf0f0f0f0f
    mov rdi, 0xfa2721
    mov rsi, 0x321123
    mov r14, 0x542343
    ret