# PODT

PODT is a CMake-based C++ research artifact for secure decision tree evaluation. This repository accompanies the following paper:

> J. Li and L. F. Zhang. ``Silent Guardians: Independent and Secure Decision Tree Evaluation Without Chatter.'' *IEEE Transactions on Dependable and Secure Computing*, 2026. [https://doi.org/10.1109/TDSC.2026.3675596](https://doi.org/10.1109/TDSC.2026.3675596)

The codebase contains three protocol implementations used for paper-aligned benchmarking and validation:

| Component | Security setting | Role in the artifact |
| --- | --- | --- |
| `ODTE_SH` | Semi-honest | Semi-honest reference implementation |
| `PVODTE_MS` | Maliciously secure | Maliciously secure implementation built around `HSS`-based components |
| `PVODTE_MH` | Maliciously secure | Maliciously secure implementation built around `VHSS`-based components |

This repository is intended for experimental evaluation and reproducibility. It is not packaged or audited as a production cryptographic library.

## Repository Structure

```text
.
├── CMakeLists.txt
├── cmake/              # Custom CMake find-modules for NTL and GMP
├── common/             # Shared helpers and cryptographic primitives
├── ODTE_SH/            # Semi-honest protocol implementation
├── PVODTE_MS/          # Maliciously secure protocol implementation
└── PVODTE_MH/          # Maliciously secure protocol implementation
```

Each protocol directory follows the same internal organization:

- `include/`: public headers
- `src/`: implementation files
- `test/main.cpp`: benchmark driver

## Build Requirements

The project requires:

- CMake `>= 3.25`
- A C++11-compatible compiler
- [NTL](https://libntl.org/)
- GMP

The top-level build configuration resolves NTL and GMP via `find_package`, so both dependencies must be installed and visible to CMake before configuration.

## Build

```sh
cmake -S . -B build
cmake --build build
```

If CMake cannot locate `NTL` or `GMP`, provide the corresponding installation prefix through standard CMake discovery variables such as `CMAKE_PREFIX_PATH`.

## Running the Benchmarks

The build produces one executable for each protocol:

```sh
./build/ODTE_SH/ODTE_SH
./build/PVODTE_MS/PVODTE_MS
./build/PVODTE_MH/PVODTE_MH
```

Each benchmark driver evaluates the protocol on the same preset workload family:

- tree depths: `3`, `8`, `13`, `17`, `20`
- attribute counts: `13`, `9`, `13`, `57`, `784`
- message bit-length: `10`
- repetitions per measurement: `5`

For each parameter point, the programs report phase-wise runtime statistics, including mean execution time and relative standard deviation (RSD). The measured phases cover setup, provider encryption, feature selection, secure comparison, classification generation, decision-tree evaluation, and decryption.

## Reproducibility Notes

- The benchmark drivers currently use hard-coded parameter sets defined in each `test/main.cpp`.
- Input instances are generated inside the testing harness and are intended for protocol timing studies rather than application deployment.
- The implementation targets research evaluation and scheme validation; it should not be interpreted as a hardened deployment artifact.

## Citation

If you use this repository in academic work, please cite the associated paper:

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

This project is released under the MIT License. See [LICENSE](LICENSE) for details.
