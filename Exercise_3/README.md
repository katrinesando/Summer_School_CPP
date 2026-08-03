# Summer School C++

Katrine Sando — AU803248

Exercises for "Learn software design using C++ from the inventor of C++" (F26).

## Repo layout

| Path                         | What it is                                          |
| ---------------------------- | --------------------------------------------------- |
| `Exercise_3/ListsVsVectors.cpp` | microbenchmarks for links, vector and sets  |
| `Tools/tools.cpp`            | `timer` module: timing + benchmark summary, reused by later exercises |
| `build.sh`                   | Build script for Exercise 2                         |

## Machine

- OS: Omarchy (Arch Linux), kernel: 7.0.9-arch2-1
- CPU: AMD Ryzen 7 5700U with Radeon Graphics
- RAM: 16Gb
- Compiler: g++ (GCC) 16.1.1 20260430
- Flags: -std=c++23 -fmodules -O2 -Wall

## Exercise 3

```sh
./build.sh
./list_vs_vector
```
If `./build.sh` gives "permission denied", run `chmod +x build.sh` first (or use `sh build.sh`).

`./build.sh clean` removes all build artifacts and the module cache.

