# hello-world-overkill

Three implementations, all bypassing `libc` and the usual output API to reach the kernel directly.

[![License](https://img.shields.io/github/license/MihaiStreames/hello-world-overkill?label=license)](LICENSE)

Linux has two builds: The C version resolves the `write` syscall number at compile time and invokes it through a small assembly stub. The pure-assembly version does the same by hand, using a [few compiler tricks](https://book.rizin.re/src/visual_mode/visual_disassembly.html) to shrink the binary further.

Windows reads the TEB to get the PEB, walks `InMemoryOrderModuleList` to find `ntdll.dll` by hash ([djb2](http://www.cse.yorku.ca/~oz/hash.html)), parses the PE export table to locate `NtWriteFile` by hash, then reads the raw stub bytes to extract the SSN. If the stub is hooked (`0xe9` jmp), it falls back to [Halo's Gate](https://blog.sektor7.net/#!res/2021/halosgate.md) -- scanning neighboring stubs to derive the SSN by offset. Finally it scans `ntdll`'s `.text` section for a `syscall; ret` gadget and jumps to it directly.

<sub>_`GetStdHandle` is still Win32 (the point isn't avoiding `kernel32`, it's bypassing `WriteFile`)._</sub>

> [!NOTE]
> x86 support is a work in progress. See [#2](https://github.com/MihaiStreames/hello-world-overkill/issues/2).

Full write-up: [blog/hello-world-overkill](https://mihaistreames.github.io/blog/hello-world-overkill)

## Build

### Linux

**On Arch:**

```sh
pacman -S clang lld
```

```sh
cd linux/c  # or linux/asm
make
./hello
```

### Windows

Requires MSVC tools (`cl`, `link`, `ml`/`ml64`). Install using [Build Tools with Visual C++](https://visualstudio.microsoft.com/visual-cpp-build-tools) ([tutorial](https://github.com/bycloudai/InstallVSBuildToolsWindows)), or headless:

```pwsh
winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --includeRecommended"
```

```bat
cd windows\c
build.bat

:: extras
:: build for x86 (default is x64)
set ARCH=x86 & build.bat

hello.exe
```

## Credits

Windows syscall techniques:

- **Hell's Gate**: [@smelly__vx](https://twitter.com/smelly__vx) & [@am0nsec](https://github.com/am0nsec) -- [repo](https://github.com/am0nsec/HellsGate)
- **Halo's Gate**: Reenz0h ([@SEKTOR7net](https://twitter.com/SEKTOR7net)) -- [write-up](https://blog.sektor7.net/#!res/2021/halosgate.md)
- **Tartarus' Gate**: [@trickster0](https://github.com/trickster0) -- [repo](https://github.com/trickster0/TartarusGate)

## License

MIT. See [LICENSE](LICENSE).

<div align="center">
  Made with ❤️
</div>
