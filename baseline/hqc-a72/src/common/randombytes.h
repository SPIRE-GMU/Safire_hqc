/* Deterministic randombytes for profiling: SHAKE256 incremental XOF over a seed.
 * Written for the fafft-c profiling harness (not part of FAFFT_HQC). */
#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H
#include <stddef.h>
#include <stdint.h>

int randombytes(uint8_t *out, size_t n);
/* Reset the random stream to SHAKE256(seed[0..seedlen) || 0xFF). */
void randombytes_reseed(const uint8_t *seed, size_t seedlen);

#endif
