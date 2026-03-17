# PODT

`PODT` is a CMake-based C++ research codebase containing example and benchmarking implementations for ODTE schemes:

- `ODTE_SH`
- `PVODTE_MS`
- `PVODTE_MH`

This repository is intended for experimentation and paper-aligned evaluation work. It is not packaged as a production library and should be treated as research code.

## Repository Layout

```text
.
├── CMakeLists.txt
├── cmake/           # Custom Find modules for dependencies
├── common/          # Shared helpers and cryptographic utilities
├── ODTE_SH/         # $\sf ODTE_{SH}$: semi-honest secure ODTE protocol
├── PVODTE_MS/       # $\sf PVODTE_{MS}$: maliciously secure ODTE protocol
└── PVODTE_MH/       # $\sf PVODTE_{MH}$: maliciously secure ODTE protocol
```

Each scheme directory contains:

- `include/` for public headers
- `src/` for implementation files
- `test/main.cpp` for the benchmark / example entrypoint

## Requirements

The project builds with:

- CMake `>= 3.25`
- A C++11-compatible compiler
- [NTL](https://libntl.org/)
- GMP

The root `CMakeLists.txt` uses `find_package(NTL REQUIRED)` and `find_package(GMP REQUIRED)`, so both libraries must be installed and discoverable by CMake before configuration.

## Build

```sh
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run

The build produces one executable per scheme:

```sh
./build/ODTE_SH/ODTE_SH
./build/PVODTE_MS/PVODTE_MS
./build/PVODTE_MH/PVODTE_MH
```

Each executable runs a small benchmark loop across several preset tree depths and attribute counts from its corresponding `test/main.cpp`.

## Notes

- The current code targets benchmarking and scheme validation, not hardened deployment.
- Parameters in the test drivers are currently hard-coded.
- If CMake cannot find `NTL` or `GMP`, provide their install prefixes through the usual CMake variables such as `CMAKE_PREFIX_PATH`.

## License

This project is available under the MIT License. See [LICENSE](LICENSE) for details.
