GLOBAL cpuVendor 
GLOBAL getKey
GLOBAL getRax, getRbx, getRcx, getRdx, getRsi, getRdi, getRbp, getRsp, getR8, getR9, getR10, getR11, getR12, getR13, getR14, getR15

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


getKey:
	push rbp
	mov rbp, rsp

	in al, 0x60

	mov rsp, rbp
	pop rbp
	ret


; return registers functions PREGUNTAR LA PARTE DE ARMADO DE STACK
getRax:
	ret

getRbx:
	mov rax, rbx
	ret

getRcx:
	mov rax, rcx
	ret

getRdx:
	mov rax, rdx
	ret

getRsi:
	mov rax, rsi
	ret

getRdi:
	mov rax, rdi
	ret

getRbp:
	mov rax, rbp
	ret

getRsp:
	mov rax, rsp
	ret

getR8:
	mov rax, r8
	ret

getR9:
	mov rax, r9
	ret

getR10:
	mov rax, r10
	ret

getR11:
	mov rax, r11
	ret
	
getR12:
	mov rax, r12
	ret

getR13:
	mov rax, r13
	ret

getR14:
	mov rax, r14
	ret
	
getR15:
	mov rax, r15
	ret