/* Deterministic randombytes for profiling: SHAKE256 incremental XOF over a seed.
 * Written for the fafft-c profiling harness (not part of FAFFT_HQC). */
#include "randombytes.h"
#include "fips202.h"
#include <string.h>

static shake256incctx g_ctx;
static int g_inited = 0;

void randombytes_reseed(const uint8_t *seed, size_t seedlen) {
    const uint8_t dom = 0xFF; /* distinct from HQC's domain bytes 1..4 */
    shake256_inc_init(&g_ctx);
    shake256_inc_absorb(&g_ctx, seed, seedlen);
    shake256_inc_absorb(&g_ctx, &dom, 1);
    shake256_inc_finalize(&g_ctx);
    g_inited = 1;
}

int randombytes(uint8_t *out, size_t n) {
    if (!g_inited) {
        static const uint8_t def[16] = "fafft-c-default";
        randombytes_reseed(def, sizeof def);
    }
    shake256_inc_squeeze(out, n, &g_ctx);
    return 0;
}
