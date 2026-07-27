# Exercise 1 - Toolchain & Setup

Hey TAs! 👋 This directory contains the initial C++23 environment setup test.

## What's Where?

* `ToolChain.cpp` — Source code demonstrating C++23 feature support (`std::println` from `<print>` and recursive lambdas using `this auto`).
* `ToolChain` — Compiled binary.

---

## How to Compile & Run

### Prerequisites
* `clang++` (version 22.1.5 or newer with `-std=c++23` support)

### 1. Compile
```bash
cd Exercise_1
clang++ -std=c++23 ToolChain.cpp -o ToolChain
```

### 2. Run
```bash
./ToolChain
```

### Expected Output
```text
Hello World!
0  1  1  2  3  5  8  13  21  
```
