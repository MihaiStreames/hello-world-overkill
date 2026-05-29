section .text

; collapse .rodata into .text
; effectively mimicking the behavior of the linker

global _start

; use narrower registers, syscalls only need 8 bits
; aka:
; 1. eax instead of rax
; 2. edi instead of rdi
; 3. edx instead of rdx

_start:
	mov	    eax, 1
	mov	    edi, 1		; stdout
	lea	    rsi, [rel msg]
	mov	    edx, msg_len
	syscall

	; rax = bytes written (positive) or -errno (negative)
	; shifting by 63 bits gives 0 (if positive) or 1 (if negative),
	; which removes jz/jg for the syscall return value check

	shr	    rax, 63
	mov	    rdi, rax
	mov	    eax, 60
	syscall

msg:        db "Hello world!", 10
msg_len     equ $ - msg
