# hello-world-overkill

Windows hello world that bypasses `libc` and the Win32 API (except `GetStdHandle`) to reach the NT kernel directly.

[![License](https://img.shields.io/github/license/MihaiStreames/hello-world-overkill?label=license)](LICENSE)

> [!NOTE]
> x86 support is a work in progress. See [#2](https://github.com/MihaiStreames/hello-world-overkill/issues/2).

Full write-up & details: [blog/hello-world-overkill](https://mihaistreames.github.io/blog/hello-world-overkill)

## Build

Requires MSVC tools (`cl`, `link`, `ml`/`ml64`). Install using [Build Tools with Visual C++](https://visualstudio.microsoft.com/visual-cpp-build-tools) ([tutorial](https://github.com/bycloudai/InstallVSBuildToolsWindows)), or headless:

```pwsh
winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --includeRecommended"
```

```bat
cd src\hello-world
build.bat

:: build for x86 (default is x64)
set ARCH=x86 & build.bat

build\hello.exe
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
