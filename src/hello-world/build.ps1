param([string]$Arch = "x64")

$ErrorActionPreference = "Stop"

$BuildDir = "build"
$Target   = "$BuildDir\hello.exe"

if ($Arch -eq "x86") {
    $AsmSrc  = "ntwritefile_x86.asm"
    $AsmTool = "llvm-ml"
} else {
    $AsmSrc  = "ntwritefile_x64.asm"
    $AsmTool = "llvm-ml64"
}

$CFlags = @("/Os", "/GS-", "/Gy", "/Gw", "/EHa-", "/clang:-fno-asynchronous-unwind-tables")
$LdFlags = @(
    "/subsystem:console"
    "/nodefaultlib"
    "/entry:main"
    "/merge:.rdata=.text"
    "/merge:.pdata=.text"
    "/out:$Target"
    "$BuildDir\hello.obj"
    "$BuildDir\ntwritefile.obj"
    "$BuildDir\ntwritefile_asm.obj"
)

if (Test-Path $BuildDir) { Remove-Item $BuildDir -Recurse -Force }
New-Item $BuildDir -ItemType Directory | Out-Null

& $AsmTool /c /Fo"$BuildDir\ntwritefile_asm.obj" $AsmSrc
if ($LASTEXITCODE) { exit 1 }

& clang-cl @CFlags /c /Fo:"$BuildDir\hello.obj" hello.c
if ($LASTEXITCODE) { exit 1 }

& clang-cl @CFlags /c /Fo:"$BuildDir\ntwritefile.obj" ntwritefile.c
if ($LASTEXITCODE) { exit 1 }

& lld-link @LdFlags
if ($LASTEXITCODE) { exit 1 }

Write-Host "Built $Target ($((Get-Item $Target).Length) bytes)"
