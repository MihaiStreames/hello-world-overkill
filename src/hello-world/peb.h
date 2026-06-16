#pragma once

#include "types.h"
#include "utils.h"

#ifdef _WIN64
    #define ReadTeb() ((PTEB)__readgsqword(0x30))
#else
    #define ReadTeb() ((PTEB)__readfsdword(0x18))
#endif

static inline PVOID GetNtdllBase(void) {
    PTEB pTeb = ReadTeb();
    PPEB pPeb = pTeb->ProcessEnvironmentBlock;

    PPEB_LDR_DATA pLdr  = pPeb->Ldr;
    LIST_ENTRY*   pHead = &pLdr->InMemoryOrderModuleList;

    for (LIST_ENTRY* pEntry = pHead->Flink; pEntry != pHead; pEntry = pEntry->Flink) {
        PLDR_DATA_TABLE_ENTRY pLdrEntry =
            CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if (djb2w(pLdrEntry->BaseDllName.Buffer) == NTDLL_HASH) {
            return pLdrEntry->DllBase;
        }
    }

    return NULL;
}

static inline HANDLE GetStdoutHandle(void) {
    PTEB pTeb = ReadTeb();
    return pTeb->ProcessEnvironmentBlock->ProcessParameters->StandardOutput;
}
