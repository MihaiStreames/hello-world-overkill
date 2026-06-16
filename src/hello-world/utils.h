#pragma once

#include <windows.h>

#define NTDLL_HASH       0x4fd1cd7bbe06fcfcULL
#define NTWRITEFILE_HASH 0x8accec2d0bb46d81ULL

// http://www.cse.yorku.ca/~oz/hash.html
static DWORD64 djb2(PBYTE pbStr) {
    DWORD64 dwHash = 0x7734773477347734;
    int     bChar;

    while ((bChar = *pbStr++) != 0) {
        dwHash = ((dwHash << 0x5) + dwHash) + bChar;
    }

    return dwHash;
}

// http://www.cse.yorku.ca/~oz/hash.html
// adapted for wide strings
static DWORD64 djb2w(PWSTR pwStr) {
    DWORD64 dwHash = 0x7734773477347734;
    WCHAR   wChar;

    while ((wChar = *pwStr++) != 0) {
        if (wChar >= L'A' && wChar <= L'Z') {
            wChar += 32;
        }
        dwHash = ((dwHash << 0x5) + dwHash) + wChar;
    }

    return dwHash;
}
