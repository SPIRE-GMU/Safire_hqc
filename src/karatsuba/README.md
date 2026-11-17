# GF(2) Karatsuba on AIE1

A packed GF(2) polynomial multiplier for the HQC lengths, split by t levels of two-way Karatsuba over a chain of 3^t + 1 AIE tiles: 3^t working tiles, each multiplying one leaf of L bits, and one tile that folds the partial products and reduces modulo X^n - 1 (t = 0 runs everything on a single tile). The leaf is an int16 `sliding_mul` schedule whose lane products are reduced to their parity.

The 13 directories are the ADF projects of the measured configurations, `<case>_L<L>_t<t>` with case `hqc1` (n = 17,669), `hqc3` (35,851) or `hqc5` (57,637): HQC-1 with t = 0, 2, 3, 4, 5 and HQC-3 and HQC-5 with t = 2, 3, 4, 5, with L = 288 for HQC-1 and HQC-3 and L = 480 for HQC-5 (HQC-3 and HQC-5 do not fit a single tile). They are the kernel sources exactly as compiled for the measurements, with two paths changed: the tool scripts, which the projects share in `tools/` instead of carrying a copy each, and the leaf, which every project includes from `leaf_aie.h` in this directory.

## Layout of one project

```
leaf_aie.h                  the leaf, included by every project
<config>/config.h           sizes, tile segment masks and offsets of this configuration
<config>/kara_ctrl.h        the packed Karatsuba recursion, unrolled per level
<config>/tile.h             the work tile (receive, compute, emit) and the folding tile
<config>/fold.h, leaf.h     word folding with frame tail clearing; the leaf interface
<config>/aie/project.h      the graph: 3^t + 1 kernels, PLIO ports and stream connections
<config>/aie/project.cpp    graph instance, run(2): two multiplications back to back
<config>/aie/kernels.h      kernel declarations
<config>/aie/src/kernels/   tile<i>.cc (one translation unit and namespace per working tile), fold.cc, leaf_aie.h (a one-line include of ../../../../leaf_aie.h)
<config>/aie/data/          in.txt (two input frames), expected.txt (the two expected output frames), manifest.json (which vectors they are)
<config>/aie/tools/project.json   frame length, kernel names and the checker path, read by the tools
<config>/aie/Makefile       the targets below
tools/                      run_sim.py, check.py, compare.py, latency.py, pm_report.py, plio.py, shared by all projects
```

Every PLIO file holds one decimal 32-bit word per line. The first input frame of the HQC configurations is the y·h product of the KeyGen golden values in `../../golden`, the second a product of random dense operands; the expected frames are schoolbook products computed with Python integers.

## Targets of a project Makefile (run in `<config>/aie`)

`x86`, `x86run`, `check` run `aiecompiler -target=x86sim`, the x86 simulator and the output check; `hw`, `pm`, `aiesim`, `check_hw`, `latency` run `aiecompiler -target=hw --stacksize=4096`, the program-memory report from the `.map` files, `aiesimulator --profile`, the output check and the latency report. Variables: `VITIS`, `PLATFORM`, `WORK_X86`, `WORK_HW`, `X86_OUTPUT`, `HW_OUTPUT`, `RESULTS`, `AIE_FLAGS`. The scripts refuse to overwrite an existing simulator output; rerun with new `WORK_HW` and `HW_OUTPUT` names. `hwfull` retries the CDO step of a failed `hw` build and keeps the compiler log.

Compile-time switches, passed as `AIE_FLAGS=--Xpreproc=-D...`: `KARA_PARITY_P1=1` (0/1 coefficients with an explicit parity instead of the P3 sign trick), `KARA_MASK_BITREV=1` (bit-reversed output mask), `KARA_STATIC_TRIPS=0` (dynamic loop trip counts), `KARA_LEAF_PROFILE=1` (cycle probes inside the leaf).

## Measurements

For every configuration: `make x86 x86run check hw pm aiesim check_hw latency`. `latency.py` reports the time stamp of the last output word of the first frame, from the start of the simulation, and the interval between the two frames; Mbps = n × 1000 / latency_ns. `pm_report.py` reads the `.map` files for the program memory, data memory and stack of every kernel. `../../benchmark/karatsuba/collect.py` gathers the `results/latency.json` files and the mapping reports into the comparison table.
