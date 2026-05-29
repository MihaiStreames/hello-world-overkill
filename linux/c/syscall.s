section .text

global sys_write
global sys_exit

sys_write:
    mov     eax, 1      ; SYS_write
    syscall
    ret

sys_exit:
    mov     eax, 60     ; SYS_exit
    syscall
