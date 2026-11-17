# HQC v5.1.0 KeyGen golden values

One directory per parameter set (`hqc-1/`, `hqc-3/`, `hqc-5/`), one file per step of KeyGen. Every value was recomputed by `tools/dump_keygen.c` with the official reference implementation (gitlab.com/pqc-hqc/hqc, tag v5.1.0, commit 06567cc) and checked against the official `intermediates_values` file and the KAT response file of each set (`10_summary.txt` lists the checks, all PASS).

The seed `seed_kem` is the one of the KEYGEN section of the official intermediate values (the same 32 bytes for the three sets, which the official tests take from one DRBG stream; the sets differ by the domain byte k), so every step can be compared byte for byte with the official files.

## Files (hqc-1 shown, the other sets use the same names)

| File | Content | Step |
| --- | --- | --- |
| `00_seed_kem.txt` | 32-byte seed_kem, which in v5.1.0 is dk_kem | `randombytes` in `hqc_kem_keygen` |
| `01_seed_pke.txt` | the 33-byte message seed_kem‖k, the 25 lanes before and after the permutation, seed_pke | `xof_init_dom_sep` + `xof_get_bytes` |
| `02_seed_dk_seed_ek.txt` | SHA3-512(seed_pke‖0x02): message, padded state, permuted state, digest, seed_dk and seed_ek | `hash_i` |
| `03_xof_dk_blocks.txt` | SHAKE256(seed_dk): initial state and every 136-byte block the sampling consumed | squeeze of `xof_init(seed_dk)` |
| `04_y_candidates.txt` | rejection sampling trace of y: raw bytes, value, below threshold, value mod n, duplicate, accepted | `vect_generate_random_support1` |
| `04_y_support.txt` | the ω positions of y, in sampling order and sorted, with the word/bit of each | same |
| `04_y_vector.txt`, `04_y_u32.txt` | y as an n-bit vector (hex bytes) and as little-endian 32-bit words | `vect_write_support_to_vector` |
| `05_x_*` | the same files for x, sampled next from the same XOF stream | same |
| `06_xof_ek_blocks.txt` | SHAKE256(seed_ek): initial state and its 17 / 33 / 53 blocks | `xof_init(seed_ek)` |
| `06_h_raw.txt`, `06_h.txt`, `06_h_u32.txt` | the first VEC_N_SIZE_BYTES squeezed bytes, then h with the last word masked to n mod 64 bits | `vect_set_random` |
| `07_yh_unreduced.txt` | the linear product y(X)·h(X), degree below 2n − 1 | for designs that reduce after a linear convolution |
| `07_yh.txt`, `07_yh_u32.txt` | y·h mod (X^n − 1) | `vect_mul` |
| `08_s.txt`, `08_s_u32.txt` | s = x + y·h | `vect_add` |
| `09_ek.txt`, `09_dk.txt` | ek = seed_ek‖s (the KAT pk); dk_pke = seed_dk and dk_kem = seed_kem (the KAT sk) | assembly |
| `10_summary.txt` | parameters, counts (candidates, rejections, blocks, permutations) and all PASS/FAIL lines | |

Lines starting with `#` are comments, the rest are data.

## Formats

- Byte strings are lower-case hex in memory order, the same as the pk in the KAT files.
- An n-bit vector stores bit i in bit i mod 8 of byte i/8 (little endian), i.e. the `uint64_t v[VEC_N_SIZE_64]` array written byte by byte.
- `*_u32.txt`: the `uint64_t` array split into little-endian 32-bit words, low half first, one 8-digit hex word per line, 2·VEC_N_SIZE_64 words (the extra high words are zero).
- Keccak states: 25 lanes in x + 5y order, each lane a 16-digit hex little-endian `uint64_t`; a "state before the permutation" already contains the message and the padding, so every before/after pair is a test vector of Keccak-f[1600].

## Checks performed by the dumper

1. One Keccak-f[1600] on the recorded state gives seed_pke; SHA3-512 recomputed from the padded state equals `hash_i`.
2. The sampling trace, rewritten with plain modular reduction, gives the same support and consumes the same bytes as the library's Barrett version.
3. Setting the positions directly equals `vect_write_support_to_vector`; the weight is exactly ω.
4. The squeezed blocks concatenate to the consumed bytes; truncation to n bits equals the library's h.
5. `vect_mul(y, h)` equals the word-shifted sparse product folded mod X^n − 1 and the bitwise cyclic-shift sum.
6. `vect_add(x, y·h)` equals flipping the ω positions of x in y·h.
7. The assembled ek and dk equal `hqc_pke_keygen(seed_pke)`, and `hqc_kem_keygen()` seeded with seed_kem gives the same pair.
8. seed_kem, seed_pke, seed_dk, seed_ek, y, x, h, s, ek_pke and dk_pke match the official intermediate values byte for byte.
9. For all 100 entries of `PQCkemKAT_32.rsp`, KeyGen from the sk (= seed_kem) reproduces the pk and sk of the file.

## Regenerate

```sh
git clone --branch v5.1.0 https://gitlab.com/pqc-hqc/hqc.git golden/tools/upstream/hqc
sh golden/tools/regen.sh            # HQC_DIR overrides the clone location
```

Only a C compiler is needed. `tools/shim/immintrin.h` is an empty stand-in because the reference headers include `<immintrin.h>` although the ref code uses no intrinsics; it lets the dumper build on Apple Silicon. Nothing inside the clone is modified; the binaries go to `tools/build/`. A different seed: `tools/build/dump_keygen_hqc1 --out DIR --seed HEX64` (`--intermediates` and `--kat` are optional).
