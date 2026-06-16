#pragma once

#include "types.h"

extern void NtWriteFileStub(void); // MASM cannot express the real prototype

typedef NTSTATUS (*NtWriteFile_t)(
    HANDLE           FileHandle,
    HANDLE           Event,
    PIO_APC_ROUTINE  ApcRoutine,
    PVOID            ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID            Buffer,
    ULONG            Length,
    PLARGE_INTEGER   ByteOffset,
    PULONG           Key
);

BOOL ResolveNtWriteFile(DWORD* pdwSSN, PVOID* ppvGadget);
