/* vect_mul vs an independent reference: for every set bit i of a, xor (b << i) into a 2n-bit
 * accumulator (linear product), then fold bit i >= n onto bit i - n.  Bitwise compare. */
#include "common.h"
#include "parameters.h"
#include "gf2x.h"

#define W ((PARAM_N + 63) / 64)   /* parenthesized: VEC_N_SIZE_64 is (N+63)/64 without parens */
#define W2 (2 * W + 1)

static void ref_mul(uint64_t *o, const uint64_t *a, const uint64_t *b) {
    static uint64_t acc[W2];
    memset(acc, 0, sizeof acc);
    for (uint32_t i = 0; i < PARAM_N; i++) {
        if (!((a[i >> 6] >> (i & 63)) & 1)) continue;
        uint32_t ws = i >> 6, bs = i & 63;
        for (uint32_t j = 0; j < W; j++) {
            acc[j + ws] ^= b[j] << bs;
            if (bs) acc[j + ws + 1] ^= b[j] >> (64 - bs);
        }
    }
    memset(o, 0, 8 * W);
    for (uint32_t i = 0; i < 2 * PARAM_N - 1; i++) {
        uint64_t bit = (acc[i >> 6] >> (i & 63)) & 1;
        uint32_t t = i >= PARAM_N ? i - PARAM_N : i;
        o[t >> 6] ^= bit << (t & 63);
    }
}
static void rand_dense(uint64_t *v) {
    for (uint32_t i = 0; i < W; i++) v[i] = lcg_next();
    v[W - 1] &= (PARAM_N % 64) ? ((1ULL << (PARAM_N % 64)) - 1) : ~0ULL;
}
static void rand_sparse(uint64_t *v, int weight) {
    memset(v, 0, 8 * W);
    int w = 0;
    while (w < weight) {
        uint32_t p = (uint32_t)(lcg_next() % PARAM_N);
        if ((v[p >> 6] >> (p & 63)) & 1) continue;
        v[p >> 6] |= 1ULL << (p & 63); w++;
    }
}
static int check(const char *label, const uint64_t *a, const uint64_t *b) {
    static uint64_t o1[W], o2[W];
    memset(o1, 0, sizeof o1); memset(o2, 0, sizeof o2);
    H(vect_mul)(o1, a, b);
    ref_mul(o2, a, b);
    int ok = memcmp(o1, o2, 8 * W) == 0;
    if (!ok) printf("  MISMATCH %s\n", label);
    return ok;
}
int main(void) {
    static uint64_t a[W], b[W];
    int ok = 1, nd = 0, ns = 0, nb = 0;
    for (int t = 0; t < 20; t++) { rand_dense(a); rand_dense(b); nd += check("dense", a, b); }
    for (int t = 0; t < 20; t++) { rand_sparse(a, PARAM_OMEGA); rand_dense(b); ns += check("sparse*dense", a, b); }
    /* boundary: highest bit (x^(n-1)) set, all ones */
    uint64_t top[W]; memset(top, 0, sizeof top); top[(PARAM_N - 1) >> 6] = 1ULL << ((PARAM_N - 1) & 63);
    uint64_t ones[W]; for (uint32_t i = 0; i < W; i++) ones[i] = ~0ULL;
    ones[W - 1] &= (PARAM_N % 64) ? ((1ULL << (PARAM_N % 64)) - 1) : ~0ULL;
    nb += check("top*top", top, top);
    nb += check("ones*ones", ones, ones);
    nb += check("ones*top", ones, top);
    rand_dense(a); nb += check("dense*top", a, top);
    rand_sparse(a, PARAM_OMEGA); a[(PARAM_N - 1) >> 6] |= 1ULL << ((PARAM_N - 1) & 63); rand_dense(b); b[(PARAM_N - 1) >> 6] |= 1ULL << ((PARAM_N - 1) & 63);
    nb += check("sparse+top * dense+top", a, b);
    ok = (nd == 20 && ns == 20 && nb == 5);
    printf("VECTMUL %s %s n=%d omega=%d dense=%d/20 sparse_x_dense=%d/20 boundary=%d/5 => %s\n",
           SET_NAME, OPT_NAME, PARAM_N, PARAM_OMEGA, nd, ns, nb, ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
