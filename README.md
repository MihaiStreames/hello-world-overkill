# hello-world-overkill

Windows hello world that bypasses `libc` and the Win32 API entirely to reach the NT kernel directly.

[![License](https://img.shields.io/github/license/MihaiStreames/hello-world-overkill?label=license)](LICENSE)

> [!NOTE]
> x86 support is a work in progress. See [#2](https://github.com/MihaiStreames/hello-world-overkill/issues/2).

Full write-up & details: [blog/hello-world-overkill](https://mihaistreames.github.io/blog/hello-world-overkill)

## Build

Requires [LLVM](https://llvm.org/builds) (`clang-cl`, `lld-link`, `llvm-ml64`). Install headless:

```pwsh
winget install LLVM.LLVM
```

```pwsh
cd src\hello-world
.\build.ps1

# build for x86 (default is x64)
.\build.ps1 -Arch x86

.\build\hello.exe
```

## Credits

Windows syscall techniques:

- **Hell's Gate**: [@smelly\_\_vx](https://twitter.com/smelly__vx) & [@am0nsec](https://github.com/am0nsec) -- [repo](https://github.com/am0nsec/HellsGate)
- **Halo's Gate**: Reenz0h ([@SEKTOR7net](https://twitter.com/SEKTOR7net)) -- [write-up](https://blog.sektor7.net/#!res/2021/halosgate.md)
- **Tartarus' Gate**: [@trickster0](https://github.com/trickster0) -- [repo](https://github.com/trickster0/TartarusGate)

## License

MIT. See [LICENSE](LICENSE).

<div align="center">
  Made with ❤️
</div>
