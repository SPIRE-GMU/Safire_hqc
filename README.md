# Jetcas2027

GF(2) polynomial multiplication for HQC on the AI Engine array of an AMD Versal VCK190, measured against the board's own Cortex-A72. Two designs: a GF(2) Karatsuba chain that spreads the operands over a tree of AIE tiles (`src/karatsuba`), and a FAFFT multiplier (`src/fafft`, to be added). The lengths are the three HQC parameter sets, n = 17,669 / 35,851 / 57,637.

## Requirements

- Vitis 2023.2 with the `xilinx_vck190_base_202320_1` platform on Linux: `aiecompiler`, `x86simulator` and `aiesimulator`. Every project Makefile reads `VITIS` (default `/tools/Xilinx/Vitis/2023.2`) and `PLATFORM`.
- Python 3 (standard library only) for the simulator tool scripts.
- For the CPU baseline: the aarch64 cross compiler shipped with Vitis, `curl`, and a VCK190 booted into Linux.

## Build and run

Design 1, GF(2) Karatsuba (`src/karatsuba`):

```bash
cd src/karatsuba/hqc1_L288_t5/aie
make x86 x86run check                 # x86simulator, output against data/expected.txt
make hw pm aiesim check_hw latency    # aiecompiler, program-memory report, aiesimulator, output check, latency of the first multiplication
```

A configuration is `<case>_L<L>_t<t>`: case `hqc1`, `hqc3` or `hqc5`, leaf length L, and t levels of Karatsuba over 3^t + 1 tiles (t = 0 is a single tile). The 13 measured configurations are in `src/karatsuba`; its README lists the targets, the variables and the compile-time switches.

Design 2, FAFFT: to be added under `src/fafft`.

CPU baseline (`baseline/hqc-a72`): `./setup.sh` fetches the HQC C code at the pinned upstream commit, `./build_arm.sh` cross-compiles the checks and the profilers into one self-extracting file, which runs on the board with `sh vck190-hqc-profile.run`. See `baseline/hqc-a72/README.md`.

## Reproduce the tables

| Table | Command |
| --- | --- |
| Karatsuba design points: tiles, latency, throughput, speedup over the A72 | `make latency` in each configuration, then `python3 benchmark/karatsuba/collect.py table` and `python3 benchmark/karatsuba/make_table.py` |
| Program memory and stack per kernel | `make pm` in each project (`tools/pm_report.py`) |
| A72 baseline: one `vect_mul` and the three KEM operations per parameter set | `vck190-hqc-profile.run` on the board |

Throughput is n / latency of one multiplication. Latency is the time from the start of the simulation to the last output word of the first multiplication in `aiesimulator`.

## Check correctness

- `make check` and `make check_hw` compare the simulator output words with the two expected frames of the configuration (`data/expected.txt`): the y·h product of the HQC KeyGen golden values in `golden/`, then the product of two random dense operands, both computed as schoolbook products with Python integers.
- `golden/` holds step-by-step values of HQC v5.1.0 KeyGen for the three parameter sets, recomputed with the official reference implementation and checked against its intermediate values and KAT files (`golden/README.md`).
- On the board, `kem_check` and `vectmul_check` run before the profiles.

## Layout

```
src/karatsuba/   GF(2) Karatsuba: the AIE kernel sources of the 13 measured configurations
src/fafft/       FAFFT multiplier (to be added)
baseline/        HQC C code profiled on the VCK190's Cortex-A72, against a pinned upstream commit
benchmark/       result collection and comparison tables
golden/          HQC KeyGen golden values and the tool that regenerates them
```

## License

MIT, see `LICENSE`. `setup.sh` in `baseline/hqc-a72` fetches the HQC code of [FAFFT_HQC](https://github.com/myhoon/FAFFT_HQC) and the SHA-3 code of [PQClean](https://github.com/PQClean/PQClean); both are public domain and are not part of this repository.
