# GF(2) Karatsuba on AIE1

A packed GF(2) polynomial multiplier for the HQC lengths, split by t levels of two-way Karatsuba over a chain of 3^t + 1 AIE tiles: 3^t working tiles, each multiplying one leaf of L bits, and one tile that folds the partial products and reduces modulo X^n - 1 (t = 0 runs everything on a single tile). The leaf is an int16 `sliding_mul` schedule whose lane products are reduced to their parity.

The 13 directories are the ADF projects of the measured configurations, `<case>_L<L>_t<t>` with case `hqc1` (n = 17,669), `hqc3` (35,851) or `hqc5` (57,637): HQC-1 with t = 0, 2, 3, 4, 5 and HQC-3 and HQC-5 with t = 2, 3, 4, 5, with L = 288 for HQC-1 and HQC-3 and L = 480 for HQC-5 (HQC-3 and HQC-5 do not fit a single tile). Every work tile of a chain is an instance of one kernel source, `aie/src/kernels/tile.cc`: the template argument is the position of the tile in the chain and selects its segment and offset tables at compile time, and the first and the last tile differ from the middle ones only in their stream ports (`tile_first<0>`, `tile_mid<ID>`, `tile_last<ID>`). The single-tile configuration `hqc1_L288_t0` has its one kernel in `tile0.cc`. Two paths differ from the tree the measurements were built from: the tool scripts, which the projects share in `tools/` instead of carrying a copy each, and the leaf, which every project includes from `leaf_aie.h` in this directory.

The numbers in the tables were measured with an earlier form of the same kernels in which every work tile had its own source file. The merged form here has been checked on the host only: every work tile of every configuration instantiated with a C++ compiler against declaration stubs, the graphs parsed, and the instances compiled in two groups and linked without duplicate definitions. It has not been through `aiecompiler`, `x86simulator` or `aiesimulator` yet.

## Layout of one project

```
leaf_aie.h                  the leaf, included by every project
<config>/config.h           sizes, tile segment masks and offsets of this configuration
<config>/kara_ctrl.h        the packed Karatsuba recursion, unrolled per level
<config>/tile.h             the work tile (receive, compute, emit) and the folding tile
<config>/fold.h, leaf.h     word folding with frame tail clearing; the leaf interface
<config>/aie/project.h      the graph: the 3^t work kernels instantiated from the templates, the folding kernel, PLIO ports and stream connections
<config>/aie/project.cpp    graph instance, run(2): two multiplications back to back
<config>/aie/kernels.h      declarations of the three work-tile templates and of fold
<config>/aie/src/kernels/   tile.cc (the work-tile kernel templates; tile0.cc in hqc1_L288_t0), fold.cc, leaf_aie.h (a one-line include of ../../../../leaf_aie.h)
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
