# FAFFT multiplier on AIE1

GF(2) polynomial multiplication for the HQC lengths with the Frobenius additive FFT, spread over the AI Engine array. The arithmetic is that of the pure C `vect_mul` of [FAFFT_HQC](https://github.com/myhoon/FAFFT_HQC): each operand is zero-padded, converted to the FFT basis (the basis conversion, then `encode_trunc_fft` for every group of 32 words) and transformed by the forward butterfly layers; the two transforms are multiplied group by group (`gf232v_mul`); the product goes through the inverse butterflies, `decode_trunc_fft`, the inverse basis conversion and the reduction modulo X^n - 1. The data stays in the bitsliced 32-bit words of the C code and every kernel is scalar code. The transform has 2,048 words and 11 butterfly layers for HQC-1 (n = 17,669), and 4,096 words and 12 layers for HQC-3 (35,851) and HQC-5 (57,637), which differ only in the frame length and the reduction.

## Structure

The array of the transform is cut into 16 rails. A butterfly layer that pairs two rails is done by an exchange kernel with two inputs and two outputs, which streams one group of 32 words at a time; the layers inside a rail are done by three layer kernels per rail. Every pass of the two word-level stages of the basis conversion is one streaming kernel, fed in descending index order, and the 256-bit-block stage together with the encode or decode is split over 8 or 16 leaf kernels. Both operands have their own forward kernels and meet at the pointwise multipliers, one per rail.

## Configurations

One configuration per parameter set. Every kernel runs on its own tile, so the kernel count is the tile count.

| Configuration | Case | Kernels | Kernel sources | Tile locations |
| --- | --- | ---: | --- | --- |
| `n16` | HQC-1 | 342 | 28: the 9 in `kernels/common` and the 19 in `kernels/hqc1` | chosen by `aiecompiler` |
| `h3n16` | HQC-3 | 383 | 29: the 9 in `kernels/common` and 20 of the 21 in `kernels/hqc35` | fixed in the graph |
| `h5n16` | HQC-5 | 383 | 29: the 9 in `kernels/common` and 20 of the 21 in `kernels/hqc35` | fixed in the graph |

The constants of a tile (block position, pass geometry, frame length) are compile-time constants of its kernel. Kernels that differ only in such constants are function templates, and the graph passes the constants as template arguments, as in `kernel::create(k_fp<510, 2, 1>)`. A kernel whose code is the same on all of its tiles is an ordinary function that the graph creates several times. The comment after each `kernel::create` names the tile. The static buffers carry `X86SIM_THREAD_LOCAL`, which keeps the buffers of kernels that share a function apart in the x86 simulator, where all kernels run in one process, and expands to nothing in a hardware build.

The measurements were taken with the same kernels written out tile by tile: one function and one source file per tile, with the constants as literals. Instantiating the sources here with the template arguments in the graphs gives that code back token for token, which was checked for all 1,108 tiles, and the three graphs compiled with a host compiler reproduce `data/expect.txt`. `aiecompiler` compiles a function-template kernel together with the main program of its tile, not as a separate file. In a trial with four of the kernels this left the program of a layer kernel unchanged, changed that of an exchange kernel by two instructions over three calls, and added 16 instructions per call to a pass kernel and 5 to a join kernel. The three configurations have not been built with `aiecompiler` or simulated in the template form, so its latency can differ slightly from the measured one.

With 383 of the 400 tiles in use the placer of `aiecompiler` 2023.2 did not find a routable placement for `h3n16` and `h5n16` (45 to 69 streams across column boundaries that carry 36), so their graphs fix every tile: operand a flows east in columns 0 to 14, operand b is its mirror image and flows west in columns 17 to 31, the two meet at the pointwise multipliers in columns 15 and 16, and the inverse path occupies columns 32 to 49. No column boundary then carries more than 16 streams in either direction.

## Layout

```
t1_common.h              stream read and write helpers
layers/                  multiplier circuits (scalar_v*.h) and the combine step, the same for both transform sizes
layers/n2048/, n4096/    the butterfly layers and their constant tables for the 2,048- and the 4,096-word transform
kernels/common/          kernels that all three configurations use, and their declarations (kernels.h)
kernels/hqc1/            kernels of the 2,048-word transform (n16), and their declarations
kernels/hqc35/           kernels of the 4,096-word transform (h3n16, h5n16), and their declarations
<config>/graph.h         the graph: kernels, PLIO ports, stream connections, tile locations where fixed
<config>/graph.cpp       graph instance, run(3): three multiplications back to back
data/                    in_a.txt, in_b.txt, expect.txt for HQC-1; hqc3/ and hqc5/ hold the same three files for HQC-3 and HQC-5
tools/                   check.py, latency.py, pm_report.py
Makefile                 the targets below, with CFG=<config>
```

Kernels in data-flow order (`A`/`B` is the operand): `FIN` stores an input frame, pads it and sends it in descending index order; `FP_<k>` is pass k of the forward basis conversion; `FLEAF_<t>` keeps its share of the 256-bit blocks, finishes the conversion on them and encodes 8 groups; `XF_<layer>_<rail>` is a forward exchange between two rails; `FL_<c>_<rail>` are the three layer kernels of a rail; `PW_<rail>` multiplies the two transforms; `IL`, `XI` are the inverse layers and exchanges; `BLEAF_<t>` decodes 8 groups and starts the inverse conversion; `JA` to `JD` interleave the leaf outputs into one descending stream; `BP_<k>` is pass k of the inverse conversion; `BS16` (HQC-3 and HQC-5 only) recombines the two halves of the 4,096-word array; `BOUT` reduces and sends the result.

Kernel sources: `k_fin.cc`; `k_fp.cc`, `k_fp_half.cc` and `k_fp_bits.cc` for the three forms of a forward pass (the buffers `source_only` and `overlap` of a block have different sizes, the same size, or the overlap is shorter than one word) and `k_bp*.cc` likewise for the inverse passes; `k_fleaf_first.cc`, `k_fleaf.cc` and `k_fleaf_last.cc` (the first and the last leaf skip words on one side only); `k_xf_<layer>.cc` and `k_xi_<layer>.cc`, one per exchange layer with its multiplier circuit; `k_fl_<c>.cc` and `k_il_<c>.cc`, the three layer kernels of a rail; `k_pw.cc`; `k_bleaf.cc`; `k_join.cc` for `JA` to `JD`; `k_bs16.cc`; and one `k_bout*.cc` per parameter set, which checks n against the upstream `parameters.h`.

The multiplier circuits in `layers/scalar_v*.h` and the constant tables in `layers/n2048/const_*.h` and `layers/n4096/const_*.h` are derived from `gft_mul_vi.c` and `btfy_consts_gft.c` of FAFFT_HQC, which is in the public domain.

Every PLIO file holds one decimal 32-bit word per line. A file has three frames: the y and h (and their product y·h) of the HQC KeyGen golden values in `../../golden`, then two pairs of random dense operands. The expected frames were computed with the upstream C code and agree with schoolbook products computed with Python integers.

## Build and run

The kernels include a few C files of the upstream HQC code (`gfv_tower.h`, `encode.c`, `bitconversion.c`, `parameters.h`). Fetch them once with `../../baseline/hqc-a72/setup.sh`; the Makefile reads them from `../../baseline/hqc-a72/src/hqc-128`, `hqc-192` or `hqc-256` according to the configuration (variable `UPSTREAM`).

```bash
make x86 x86run check CFG=n16                 # x86simulator, output against data/expect.txt
make hw pm aiesim check_hw latency CFG=n16    # aiecompiler, memory report, aiesimulator, output check, latency
```

Targets: `x86`, `x86run`, `check` run `aiecompiler -target=x86sim`, the x86 simulator and the output check; `hw`, `pm`, `aiesim`, `check_hw`, `latency` run `aiecompiler -target=hw --stacksize=4096`, the program and data memory report from the linker maps, `aiesimulator --profile`, the output check and the latency report (`results/<config>/latency.json`). `hwfull` runs `hw` into a fresh work directory, keeps the compiler log and repeats the CDO step. Variables: `CFG`, `VITIS`, `PLATFORM`, `UPSTREAM`, `WORK_X86`, `WORK_HW`, `X86_OUTPUT`, `HW_OUTPUT`, `RESULTS`, `AIE_FLAGS`.

Notes:

- `aiecompiler` 2023.2 keeps three file descriptors open per kernel and fails after the last kernel is built when it runs out of them; the `hw` target raises the limit to 65,536 for that reason.
- The placer runs for about 30 minutes on `n16`; with fixed locations (`h3n16`, `h5n16`) placement and routing take seconds.
- `aiesimulator` needs up to 38 GB of memory for the 383 tiles of `h3n16` and `h5n16` (the 342 tiles of `n16` were at 28 GB one minute into the run), and about an hour for the three multiplications of `h3n16`.
- The block under `#ifdef TREE_TAP` in each `graph.h` adds an output port to every stream. It was used during development to compare intermediate streams in the x86 simulator, and no target here defines it.

## Measurements

`latency.py` reports the time stamp of the last output word of the first multiplication, counted from the start of the simulation, and the intervals between successive results; Mbps = n × 1000 / latency_ns. `pm_report.py` lists the program and data memory of every tile against the limits of a tile (16,384 and 32,768 bytes). `../../benchmark/fafft/make_table.py` collects the `results/<config>/latency.json` files into the comparison table against the A72 baseline.
