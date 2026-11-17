/* Layer 2: one vect_mul (gf2x.c -> gfmul_fft.c bmul2_* + reduce.c) rewritten stage-by-stage with
 * rdtsc between stages; output verified byte-for-byte against the library's vect_mul. */
#include "common.h"
#include "parameters.h"
#include "gf2x.h"
#include "gfmul_fft.h"
#include "encode.h"
#include "btfy_ffft.h"
#include "bitconversion.h"
#include "gfv_tower.h"   /* gf232v_mul (static inline, pure-C branch) */
#include "reduce.h"

#if SECURITYLEVEL == 1
#define PRE_BITS 32768
#define FFT_BITS 65536
#define COPY_BYTES 2216
#define BC bc_1_32768
#define BTFY btfy_65536
#define IBTFY ibtfy_65536
#define IBC ibc_1_65536
#define NAME_BC "bc_1_32768"
#define NAME_BTFY "btfy_65536"
#define NAME_IBTFY "ibtfy_65536"
#define NAME_IBC "ibc_1_65536"
#else
#define PRE_BITS 65536
#define FFT_BITS 131072
#define COPY_BYTES (VEC_N_SIZE_64 * 8)
#define BC bc_1_65536
#define BTFY btfy_131072
#define IBTFY ibtfy_131072
#define IBC ibc_1_131072
#define NAME_BC "bc_1_65536"
#define NAME_BTFY "btfy_131072"
#define NAME_IBTFY "ibtfy_131072"
#define NAME_IBC "ibc_1_131072"
#endif
#define PRE_W (PRE_BITS / 32)
#define FFT_W (FFT_BITS / 32)
#define STRIDE (FFT_W / 32)

/* bmul2_*_prepare, staged.  which = "a" or "b" selects the stage names. */
#define STAGED_PREPARE(PFX, a_out, a_in)                                                           \
    do {                                                                                           \
        uint32_t a0[PRE_W] = {0};                                                                  \
        memcpy(a0, a_in, COPY_BYTES);                                                              \
        PROF_NEXT(PFX " zero a0 + memcpy input", "COPY");                                          \
        BC(a0);                                                                                    \
        PROF_NEXT(PFX " basis conversion " NAME_BC, "BC");                                         \
        uint32_t temp[16];                                                                         \
        for (int i = 0; i < STRIDE; i++) {                                                         \
            for (int j = 0; j < 16; j++) temp[j] = a0[STRIDE * j + i];                             \
            encode_trunc_fft(a_out + i * 32, temp);                                                \
        }                                                                                          \
        PROF_NEXT(PFX " encode loop (gather + encode_to_gft)", "ENCODE");                          \
        BTFY(a_out);                                                                               \
        PROF_NEXT(PFX " butterfly " NAME_BTFY, "BTFY");                                            \
    } while (0)

static void staged_vect_mul(uint64_t *o, const uint64_t *v1, const uint64_t *v2) {
    PROF_DECL; uint64_t _t0 = tsc_begin(); PROF_START();
    uint32_t o_u32[FFT_N / 32] = {0};
    PROF_NEXT("vect_mul/00 zero o_u32 (FFT_N/32 words)", "COPY");
    /* bmul2_*(c=o_u32, a=v1, b=v2) */
    uint32_t a_u32[FFT_W];
    uint32_t b_u32[FFT_W];
    STAGED_PREPARE("vect_mul/01 prepare(a):", a_u32, (const uint8_t *)v1);
    STAGED_PREPARE("vect_mul/02 prepare(b):", b_u32, (const uint8_t *)v2);
    /* bmul2_*_mul(c, a, b) */
    uint8_t *c = (uint8_t *)o_u32;
    uint32_t a0[FFT_W];
    uint32_t *c32 = (uint32_t *)c;
    for (int i = 0; i < FFT_W; i += 32) { gf232v_mul(a0 + i, a_u32 + i, b_u32 + i); }
    PROF_NEXT("vect_mul/03 pointwise gf232v_mul loop", "PMUL");
    IBTFY(a0);
    PROF_NEXT("vect_mul/04 inverse butterfly " NAME_IBTFY, "IBTFY");
    uint32_t temp[32];
    for (int i = 0; i < FFT_W; i += 32) {
        decode_trunc_fft(temp, a0 + i);
        for (int j = 0; j < 32; j++) c32[STRIDE * j + i / 32] = temp[j];
    }
    PROF_NEXT("vect_mul/05 decode loop (decode_from_gft + scatter)", "DECODE");
    IBC(c32);
    PROF_NEXT("vect_mul/06 inverse basis conversion " NAME_IBC, "IBC");
    reduce((uint32_t *)o, (uint32_t *)o_u32);
    PROF_STOP("vect_mul/07 reduce (mod X^n-1)", "REDUCE");
    { uint64_t _e = tsc_end(); static int _id = -1; if (_id < 0) _id = stage_id("vect_mul/TOTAL(staged)", "TOTAL"); stage_add(_id, _e - _t0); }
}

#define W ((PARAM_N + 63) / 64)   /* parenthesized: VEC_N_SIZE_64 is (N+63)/64 without parens */
static void rand_dense(uint64_t *v) {
    for (uint32_t i = 0; i < W; i++) v[i] = lcg_next();
    v[W - 1] &= (PARAM_N % 64) ? ((1ULL << (PARAM_N % 64)) - 1) : ~0ULL;
}
static void rand_sparse(uint64_t *v, int weight) {
    memset(v, 0, 8 * W);
    int w = 0;
    while (w < weight) { uint32_t p = (uint32_t)(lcg_next() % PARAM_N); if ((v[p >> 6] >> (p & 63)) & 1) continue; v[p >> 6] |= 1ULL << (p & 63); w++; }
}

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2000;
    int warm = argc > 2 ? atoi(argv[2]) : 100;
    const char *csv = argc > 3 ? argv[3] : NULL;
    static uint64_t a[W], b[W], o1[W], o2[W];
    /* verification on 20 inputs (sparse x dense, as in HQC) + 5 dense x dense */
    int bad = 0;
    for (int t = 0; t < 25; t++) {
        if (t < 20) rand_sparse(a, PARAM_OMEGA_R); else rand_dense(a);
        rand_dense(b);
        memset(o1, 0, sizeof o1); memset(o2, 0, sizeof o2);
        H(vect_mul)(o1, a, b); staged_vect_mul(o2, a, b);
        if (memcmp(o1, o2, sizeof o1)) bad++;
    }
    printf("VERIFY staged vect_mul == library vect_mul (25 inputs): %s\n", bad ? "MISMATCH" : "OK");
    if (bad) return 1;
    for (int i = 0; i < g_nstages; i++) g_stages[i].n = 0;

    int id_lib = stage_id("vect_mul/TOTAL(library)", "TOTAL");
    /* inputs as in HQC: sparse (weight omega_r) x dense; a fresh pair every iteration */
    for (int i = 0; i < warm + n; i++) {
        rand_sparse(a, PARAM_OMEGA_R); rand_dense(b); memset(o1, 0, sizeof o1);
        uint64_t t0 = tsc_begin(); H(vect_mul)(o1, a, b); uint64_t t1 = tsc_end();
        if (i >= warm) stage_add(id_lib, t1 - t0);
    }
    for (int i = 0; i < warm; i++) { rand_sparse(a, PARAM_OMEGA_R); rand_dense(b); memset(o2, 0, sizeof o2); staged_vect_mul(o2, a, b); }
    for (int i = 0; i < g_nstages; i++) if (i != id_lib) g_stages[i].n = 0;
    for (int i = 0; i < n; i++) { rand_sparse(a, PARAM_OMEGA_R); rand_dense(b); memset(o2, 0, sizeof o2); staged_vect_mul(o2, a, b); }

    double ghz = tsc_ghz_measure();
    printf("samples: n=%d warmup=%d\n", n, warm);
    print_report(ghz, csv, "vect_mul");
    return 0;
}
