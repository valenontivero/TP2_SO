GLOBAL divideByZero
GLOBAL invalidOpcode
GLOBAL fillRegs

divideByZero:
    mov rax, 3
    mov rbx, 0
    ; mov rcx, 10
    ; mov rdi, 60
    ; mov rsi, 70
    ; mov rdx, 1
    ; mov r9, 9
    ; mov r10, 10
    ; mov r11, 11
    ; mov r12, 12
    ; mov r13, 13
    ; mov r14, 14
    ; mov r15, 15
    div rbx
    ret

invalidOpcode:
    mov cr6, rax
    ret


%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

fillRegs:
    pushState
    mov rax, 0xFFFFFFFF
    mov rbx, 0xFF000000
    mov rcx, 0xEEEEEEEE
    mov rdx, 0xDDDDDDDD
    mov rsi, 0xCCCCCCCC
    mov rdi, 0xBBBBBBBB
    mov rbp, 0xBB000000
    mov r8,  0xAAAAAAAA
    mov r9,  0x99999999
    mov r10, 0x88888888
    mov r11, 0x77777777
    mov r12, 0x77770000
    mov r13, 0x77000000
    mov r14, 0x55566666
    mov r15, 0x26666666

.decr:
    dec rax
    dec rbx
    dec rcx
    dec rdx
    dec r8
    dec r9
    dec r10
    dec r11
    dec r12
    dec r13
    dec r14
    dec r15
    dec rdi
    dec rsi
    dec rbp

    cmp r15, 0
    je .end
    jmp .decr

.end:
    popState
    ret