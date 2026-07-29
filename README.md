# Summer School C++

Katrine Sando — AU803248

Exercises for "Learn software design using C++ from the inventor of C++" (F26).

## Repo layout

| Path                         | What it is                                          |
| ---------------------------- | --------------------------------------------------- |
| `Exercise_1/ToolChain.cpp`   | A1: toolchain setup exercise                        |
| `Exercise_2/Measurement.cpp` | A2: microbenchmarks for std::find / std::find_if    |
| `Tools/tools.cpp`            | `timer` module: timing + benchmark summary, reused by later exercises |
| `build.sh`                   | Build script for Exercise 2                         |

## Machine

- OS: Omarchy (Arch Linux), kernel: 7.0.9-arch2-1
- CPU: AMD Ryzen 7 5700U with Radeon Graphics
- RAM: 16Gb
- Compiler: g++ (GCC) 16.1.1 20260430
- Flags: -std=c++23 -fmodules -O2 -Wall

## Exercise 1

```sh
cd Exercise_1
g++ -std=c++23 -O2 ToolChain.cpp -o ToolChain
./ToolChain
```

## Exercise 2

```sh
./build.sh
./measurement
```
If `./build.sh` gives "permission denied", run `chmod +x build.sh` first (or use `sh build.sh`).

`./build.sh clean` removes all build artifacts and the module cache.

The code uses C++23 modules, including `import std`, so build order matters.
The script runs these steps in order:

```sh
g++ -std=c++23 -fmodules -O2 -Wall -fsearch-include-path bits/std.cc -c -o /dev/null   # std module -> gcm.cache/std.gcm (once)
g++ -std=c++23 -fmodules -O2 -Wall -c Tools/tools.cpp -o tools.o                        # timer module
g++ -std=c++23 -fmodules -O2 -Wall Exercise_2/Measurement.cpp tools.o -o measurement    # main program
```
