#include "ntwritefile.h"
#include "types.h"

DWORD dwSSN    = 0;
PVOID pvGadget = NULL;

int main(void) {
    const char      szMsg[]       = "Hello world!\n";
    IO_STATUS_BLOCK ioStatusBlock = {0};
    NtWriteFile_t   pfnWriteFile  = (NtWriteFile_t)(void*)NtWriteFileStub;
    NTSTATUS        lStatus;

    if (!ResolveNtWriteFile(&dwSSN, &pvGadget)) {
        return 1;
    }

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    lStatus = pfnWriteFile(
        hStdout,
        NULL,
        NULL,
        NULL,
        &ioStatusBlock,
        (PVOID)(ULONG_PTR)szMsg,
        sizeof(szMsg) - 1,
        NULL,
        NULL
    );

    return lStatus < 0 ? 1 : 0;
}
