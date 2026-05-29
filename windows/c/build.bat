@echo off
setlocal

set TARGET=hello.exe
set ASM_OBJ=ntwritefile_asm.obj

if "%ARCH%"=="x86" (
  set ASM_SRC=ntwritefile_x86.asm
  set ASMTOOL=ml
) else (
  set ASM_SRC=ntwritefile_x64.asm
  set ASMTOOL=ml64
)

set CFLAGS=/Os /GS-

if "%DBG%"=="1" (
  set CFLAGS=%CFLAGS% /DDBG
  set LDFLAGS=/subsystem:console kernel32.lib
) else (
  set LDFLAGS=/subsystem:console /nodefaultlib /entry:main kernel32.lib
)

%ASMTOOL% /c /Fo%ASM_OBJ% %ASM_SRC%
if %ERRORLEVEL% neq 0 (
  echo error: asm build failed
  exit /b 1
)

cl %CFLAGS% /c /Fo:hello.obj hello.c
if %ERRORLEVEL% neq 0 (
  echo error: C build failed
  exit /b 1
)

cl %CFLAGS% /c /Fo:ntwritefile.obj ntwritefile.c
if %ERRORLEVEL% neq 0 (
  echo error: C build failed
  exit /b 1
)

link %LDFLAGS% /out:%TARGET% hello.obj ntwritefile.obj %ASM_OBJ%
if %ERRORLEVEL% neq 0 (
  echo error: link failed
  exit /b 1
)

echo Built %TARGET%
