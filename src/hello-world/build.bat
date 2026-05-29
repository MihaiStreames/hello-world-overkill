@echo off
setlocal

if "%ARCH%"=="x86" (
  set ASM_SRC=ntwritefile_x86.asm
  set ASMTOOL=ml
) else (
  set ASM_SRC=ntwritefile_x64.asm
  set ASMTOOL=ml64
)

set BUILD_DIR=build
set TARGET=%BUILD_DIR%\hello.exe
set CFLAGS=/Os /GS-
set LDFLAGS=/subsystem:console /nodefaultlib /entry:main kernel32.lib

if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
mkdir %BUILD_DIR%

%ASMTOOL% /c /Fo%BUILD_DIR%\ntwritefile_asm.obj %ASM_SRC%
if %ERRORLEVEL% neq 0 (
  echo error: asm build failed
  exit /b 1
)

cl %CFLAGS% /c /Fo:%BUILD_DIR%\hello.obj hello.c
if %ERRORLEVEL% neq 0 (
  echo error: C build failed
  exit /b 1
)

cl %CFLAGS% /c /Fo:%BUILD_DIR%\ntwritefile.obj ntwritefile.c
if %ERRORLEVEL% neq 0 (
  echo error: C build failed
  exit /b 1
)

link %LDFLAGS% /out:%TARGET% %BUILD_DIR%\hello.obj %BUILD_DIR%\ntwritefile.obj %BUILD_DIR%\ntwritefile_asm.obj
if %ERRORLEVEL% neq 0 (
  echo error: link failed
  exit /b 1
)

echo Built %TARGET%
