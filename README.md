# CuLibLinearAlg

[![build](https://github.com/Jort12/CuLibLinearAlg/actions/workflows/build.yml/badge.svg)](https://github.com/Jort12/CuLibLinearAlg/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A custom linear algebra library in C++ with planned CUDA acceleration.

## Build Instructions

Requires CMake 3.15+ and a C++17 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

Binaries produced:

- `linalg` — static library
- `linalg_tests` — test runner
- `linalg_bench` — benchmark runner
- `linalg_example` — example program

## Roadmap

- **Phase 1 — CPU implementation:** Matrix storage, element access, and free-function ops (`matmul`, `gemv`, `add`, `scale`, `hadamard`, `transpose`) on CPU.
- **Phase 2 — CUDA kernels:** GPU backend and kernels for the same op set, dispatched via `Backend::CUDA`.
- **Phase 3 — Optimization and benchmarking vs cuBLAS:** Tune kernels and compare throughput against cuBLAS on representative workloads.

## Project Structure

```
CuLibLinearAlg/
├── include/linalg/        # Public headers
│   ├── matrix.hpp         # Matrix<T> template
│   ├── ops.hpp            # Free-function API declarations
│   └── backend.hpp        # Backend enum (CPU, CUDA)
├── src/                   # Library implementation
│   ├── matrix.cpp
│   └── ops_cpu.cpp        # CPU backend for ops
├── tests/                 # Unit tests
│   └── test_main.cpp
├── benchmarks/            # Performance benchmarks
│   └── bench_main.cpp
├── examples/              # Usage examples
│   └── example_basic.cpp
├── .github/workflows/     # CI configuration
│   └── build.yml
├── CMakeLists.txt
├── LICENSE
└── README.md
```
