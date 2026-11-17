# HQC on the VCK190's Cortex-A72

The CPU baseline. The HQC C code of [FAFFT_HQC](https://github.com/myhoon/FAFFT_HQC) at commit `16d6773f` (`crypto_kem/hqc-{128,192,256}/m4f`, built in its pure C mode) is profiled on one Cortex-A72 core of the VCK190 by a harness that times one `vect_mul` stage by stage and the three KEM operations statement by statement.

- `setup.sh` fetches the upstream sources at that commit into `src/hqc-{128,192,256}/` and removes the `_M4_ASM_` define from `parameters.h`, the only change to the upstream code. It also fetches `fips202.{c,h}` (SHAKE and SHA-3) from PQClean into `src/common/`, next to the deterministic `randombytes` of this harness.
- `harness/`: `kem_check` (KEM round trips, tampered ciphertexts rejected, `vect_mul` call counts), `vectmul_check` (random dense products, sparse-by-dense products and boundary cases against an independent reference), `prof_vectmul` (one multiplication rewritten stage by stage, verified byte for byte against the library's `vect_mul`), `prof_kem` (KeyGen, Encaps and Decaps statement by statement). `common.h` holds the timer, the CPU pinning and the statistics.
- `build_arm.sh` cross-compiles the three libraries and the four programs with the aarch64 gcc of Vitis 2023.2 (`-O3 -mcpu=cortex-a72`, static), checks that every binary is static aarch64, and wraps them with `run_profile.sh` and `header.sh` into one self-extracting file, `out/vck190-hqc-profile.run`, whose payload is protected by a SHA-256.

On the board (Linux, a shell as the board's user):

```sh
cd /tmp && sh vck190-hqc-profile.run
```

The file refuses to run on anything but aarch64, checks its own payload, runs `kem_check` (1000 rounds) and `vectmul_check` for the three parameter sets, then `prof_vectmul` and `prof_kem` (N = 2000 timed calls after 100 warm-up calls), and leaves one result file, `vck190-hqc-profile-<host>-<time>.txt`, in the current directory. The CSV with min, quartiles, median and mean ticks per stage is appended to it. `N`, `WARM` and `ROUNDS` can be set in the environment. No root, no network, no change to the system; the temporary directory is removed at the end.

Timer: the Arm generic timer `CNTVCT_EL0` read between `isb` barriers, with `clock_gettime(CLOCK_MONOTONIC_RAW)` as the fallback if user space cannot read the counter. The programs pin themselves to CPU 1 (`PIN_CPU` overrides). Report lines name the timer and its measured frequency.
