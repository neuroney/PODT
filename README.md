# PODT

PODT is a CMake-based C++ research artifact for secure decision tree evaluation. This repository accompanies the following paper:

> J. Li and L. F. Zhang. `Silent Guardians: Independent and Secure Decision Tree Evaluation Without Chatter.` *IEEE Transactions on Dependable and Secure Computing*, 2026. [https://doi.org/10.1109/TDSC.2026.3675596](https://doi.org/10.1109/TDSC.2026.3675596)

The repository contains three protocol implementations:

| Component | Security setting | Notes |
| --- | --- | --- |
| `ODTE_SH` | Semi-honest | Baseline implementation |
| `PVODTE_MS` | Maliciously secure | Variant with verification parameter `A` |
| `PVODTE_MH` | Maliciously secure | Variant based on `VHSS` verification |

This code is intended for experimental evaluation and reproducibility. It is not a production cryptographic library.

## Repository Layout

```text
.
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/
├── common/
├── ODTE_SH/
├── PVODTE_MS/
├── PVODTE_MH/
└── .github/workflows/ci.yml
```

## Requirements

- CMake `>= 3.25`
- A C++11-compatible compiler
- [NTL](https://libntl.org/)
- GMP

Example package names:

```sh
# Ubuntu / Debian
sudo apt-get update
sudo apt-get install -y cmake g++ libntl-dev libgmp-dev
```

```sh
# macOS with Homebrew
brew install cmake ntl gmp
```

## Quick Start

Configure and build:

```sh
cmake --preset default
cmake --build --preset default
```

Run the repository smoke tests:

```sh
ctest --preset default
```

Run one protocol directly:

```sh
./build/default/ODTE_SH/ODTE_SH --quick
./build/default/PVODTE_MS/PVODTE_MS --quick
./build/default/PVODTE_MH/PVODTE_MH --quick
```

If the dependencies are not in a standard search path, configure with an explicit prefix:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/homebrew
```

You can also pass `NTL_DIR` and `GMP_DIR` directly:

```sh
cmake -S . -B build -DNTL_DIR=/path/to/ntl -DGMP_DIR=/path/to/gmp
```

## Running Benchmarks

The default commands are:

```sh
./build/default/ODTE_SH/ODTE_SH
./build/default/PVODTE_MS/PVODTE_MS
./build/default/PVODTE_MH/PVODTE_MH
```

These run the repository's default workload family:

- tree depths: `3`, `8`, `13`, `17`, `20`
- attribute counts: `13`, `9`, `13`, `57`, `784`
- message bit-length: `10`
- repetitions: `5`

Use `--quick` only for a fast end-to-end check. It runs a reduced case and is not intended for paper timings.

## Command-Line Options

All three executables share the same CLI:

```sh
./build/default/ODTE_SH/ODTE_SH --help
./build/default/ODTE_SH/ODTE_SH --depth 3 --attributes 13 --msgbit 10 --cycles 3
```

Supported options:

- `--quick`: run a reduced smoke-test case
- `--depth <n>`: set tree depth for a single custom run
- `--attributes <n>`: set attribute count for a single custom run
- `--msgbit <n>`: set message bit-length
- `--cycles <n>`: set timing repetitions
- `--debug`: enable debug mode
- `--help`: print usage

To compare all three protocols on the same parameter point:

```sh
./build/default/ODTE_SH/ODTE_SH --depth 3 --attributes 13 --msgbit 10 --cycles 3
./build/default/PVODTE_MS/PVODTE_MS --depth 3 --attributes 13 --msgbit 10 --cycles 3
./build/default/PVODTE_MH/PVODTE_MH --depth 3 --attributes 13 --msgbit 10 --cycles 3
```

## Output

Each run prints phase-wise timing statistics such as:

- `Setup algo time`
- `Provider encryption time`
- `Feature Selection time`
- `HSSCMP` or `VHSSCMP time`
- `ClassificationGen time`
- `DTevaluation 0/1 time`
- `Decryption time`

The reported values are mean time in milliseconds plus `RSD` (relative standard deviation). In `--quick` mode the default repetition count is `1`, so `RSD` is usually `0%`.

## Notes

- `ctest --preset default` currently runs the same `--quick` mode for all three executables.
- The benchmark drivers generate inputs internally; this repository is aimed at protocol evaluation, not deployment.
- The repo includes `CMakePresets.json`, `.editorconfig`, `.clang-format`, and GitHub Actions CI for basic project hygiene.

## Citation

If you use this repository in academic work, please cite:

```bibtex
@article{li2026silent,
  author = {J. Li and L. F. Zhang},
  title = {Silent Guardians: Independent and Secure Decision Tree Evaluation Without Chatter},
  journal = {IEEE Transactions on Dependable and Secure Computing},
  year = {2026},
  pages = {1--18},
  doi = {10.1109/TDSC.2026.3675596}
}
```

## License

Released under the MIT License. See [LICENSE](LICENSE) for details.
