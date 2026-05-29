EXTERN dwSSN:DWORD
EXTERN pvGadget:QWORD

.code
    NtWriteFileStub PROC
        ; r10 must hold rcx before kernel sees syscall
        ; syscall number zero-extends to rax
        ; jump to ntdll "syscall; ret" gadget

        mov     r10, rcx
        mov     eax, DWORD PTR [dwSSN]
        jmp          QWORD PTR [pvGadget]
    NtWriteFileStub ENDP
end
