/*
 * dump_keygen.c -- step-by-step golden values for HQC KeyGen (v5.1.0, ref code).
 *
 * One binary per parameter set (compile with that set's include directories,
 * see regen.sh). The program re-executes HQC-KEM.KeyGen exactly in the order of
 * src/common/hqc-kem.c (hqc_kem_keygen_internal) and src/ref/hqc.c
 * (hqc_pke_keygen), using the library's own functions for every step, and in
 * parallel recomputes each step with small independent code (plain modulo
 * instead of Barrett, bit-level cyclic rotation instead of Karatsuba, ...).
 * Every intermediate value is written to its own file in --out, then the
 * result is checked against
 *   - the library's hqc_pke_keygen / hqc_kem_keygen outputs,
 *   - the official kats/ref/hqc-N/intermediates_values file (--intermediates),
 *   - all 100 entries of the official KAT response file (--kat).
 *
 * Usage:
 *   dump_keygen --out DIR [--seed HEX64] [--intermediates FILE] [--kat FILE]
 * If --seed is absent the seed_kem printed in the intermediates file is used.
 *
 * Exit status 0 only if every check passed.
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pull in the fips202 translation unit so the static Keccak permutation and
 * the incremental helpers are visible here (fips202.c is NOT compiled
 * separately when this file is used). */
#include "fips202.c"

#include "hqc-kem.h"
#include "parameters.h"
#include "hqc.h"
#include "symmetric.h"
#include "vector.h"
#include "gf2x.h"
#include "randombytes.h"

/* ------------------------------------------------------------------------ */
/* Deterministic randombytes: hqc_kem_keygen() asks for SEED_BYTES bytes.     */
/* ------------------------------------------------------------------------ */
static uint8_t g_rng_buf[256];
static size_t g_rng_len = 0, g_rng_pos = 0;

static void rng_load(const uint8_t *b, size_t n) {
    memcpy(g_rng_buf, b, n);
    g_rng_len = n;
    g_rng_pos = 0;
}

int randombytes(uint8_t *out, size_t outlen) {
    if (g_rng_pos + outlen > g_rng_len) return -1;
    memcpy(out, g_rng_buf + g_rng_pos, outlen);
    g_rng_pos += outlen;
    return 0;
}

/* ------------------------------------------------------------------------ */
/* Output helpers                                                             */
/* ------------------------------------------------------------------------ */
static const char *g_outdir = NULL;
static FILE *g_summary = NULL;
static int g_fail = 0;

static FILE *out_open(const char *name) {
    char path[4096];
    snprintf(path, sizeof path, "%s/%s", g_outdir, name);
    FILE *f = fopen(path, "w");
    if (!f) {
        perror(path);
        exit(1);
    }
    return f;
}

static void put_hex(FILE *f, const uint8_t *b, size_t n) {
    for (size_t i = 0; i < n; i++) fprintf(f, "%02x", b[i]);
    fputc('\n', f);
}

/* 25 Keccak lanes, lane index x + 5*y, each as a 64-bit little-endian word in hex */
static void put_lanes(FILE *f, const uint64_t *s) {
    for (int i = 0; i < 25; i++) fprintf(f, "%s%016llx", i ? " " : "", (unsigned long long)s[i]);
    fputc('\n', f);
}

/* 64-bit words as pairs of 32-bit words, low half first (little-endian word order) */
static void put_u32_words(FILE *f, const uint64_t *v, size_t nwords64) {
    for (size_t i = 0; i < nwords64; i++) fprintf(f, "%08x\n%08x\n", (uint32_t)v[i], (uint32_t)(v[i] >> 32));
}

static void check(int ok, const char *what) {
    printf("%s  %s\n", ok ? "PASS" : "FAIL", what);
    if (g_summary) fprintf(g_summary, "%s  %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) g_fail = 1;
}

static void note(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
    if (g_summary) {
        va_start(ap, fmt);
        vfprintf(g_summary, fmt, ap);
        va_end(ap);
        fputc('\n', g_summary);
    }
}

static int hexval(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* Parse exactly n bytes of hex; returns 0 on success. */
static int parse_hex(const char *s, uint8_t *out, size_t n) {
    for (size_t i = 0; i < n; i++) {
        int h = hexval(s[2 * i]), l = hexval(s[2 * i + 1]);
        if (h < 0 || l < 0) return -1;
        out[i] = (uint8_t)((h << 4) | l);
    }
    int c = s[2 * n];
    if (hexval(c) >= 0) return -1; /* longer than expected */
    return 0;
}

/* ------------------------------------------------------------------------ */
/* Keccak helpers (independent of the hqc wrappers, but using fips202's core) */
/* ------------------------------------------------------------------------ */
/* Bytes 0..n-1 of the sponge state (rate part), little-endian lanes. */
static void lanes_to_bytes(const uint64_t *s, uint8_t *out, size_t n) {
    for (size_t i = 0; i < n; i++) out[i] = (uint8_t)(s[i >> 3] >> (8 * (i & 7)));
}

/* Squeeze nblocks full SHAKE256 blocks from a finalized (not yet permuted)
 * context copy, recording the state after each permutation and the block. */
static void dump_blocks(FILE *f, const shake256_xof_ctx *finalized, size_t nblocks, uint8_t *stream) {
    shake256_xof_ctx c = *finalized;
    for (size_t b = 0; b < nblocks; b++) {
        KeccakF1600_StatePermute(c.ctx);
        fprintf(f, "# block %zu: state after permutation %zu (25 lanes)\n", b, b + 1);
        put_lanes(f, c.ctx);
        fprintf(f, "# block %zu: 136 output bytes (first 136 bytes of that state)\n", b);
        lanes_to_bytes(c.ctx, stream + b * SHAKE256_RATE, SHAKE256_RATE);
        put_hex(f, stream + b * SHAKE256_RATE, SHAKE256_RATE);
    }
}

/* ------------------------------------------------------------------------ */
/* Independent re-implementations of the KeyGen steps                        */
/* ------------------------------------------------------------------------ */
typedef struct {
    uint8_t raw[3];
    uint32_t cand;    /* 24-bit little-endian candidate */
    uint32_t reduced; /* cand mod n (plain modulo) */
    int below;        /* cand < UTILS_REJECTION_THRESHOLD */
    int dup;          /* reduced already in the support */
    int accepted;     /* below && !dup */
} cand_t;

#define MAX_TRACE 4096

/* Same algorithm as vect_generate_random_support1 but with plain modulo and a trace. */
static size_t trace_support1(shake256_xof_ctx *ctx, uint32_t *support, uint16_t weight, cand_t *tr, uint8_t *rawstream,
                             size_t *rawlen) {
    size_t i = 0, t = 0;
    while (i < weight) {
        uint8_t rb[3];
        xof_get_bytes(ctx, rb, 3);
        memcpy(rawstream + *rawlen, rb, 3);
        *rawlen += 3;
        uint32_t cand = (uint32_t)rb[0] | ((uint32_t)rb[1] << 8) | ((uint32_t)rb[2] << 16);
        int below = cand < UTILS_REJECTION_THRESHOLD;
        uint32_t red = cand % PARAM_N;
        int dup = 0;
        for (size_t j = 0; j < i; j++) dup |= (support[j] == red);
        int acc = below && !dup;
        if (t < MAX_TRACE) {
            memcpy(tr[t].raw, rb, 3);
            tr[t].cand = cand;
            tr[t].reduced = red;
            tr[t].below = below;
            tr[t].dup = dup;
            tr[t].accepted = acc;
        }
        t++;
        if (acc) support[i++] = red;
    }
    return t;
}

static void support_to_vector_simple(uint64_t *v, const uint32_t *support, uint16_t weight) {
    for (size_t i = 0; i < weight; i++) v[support[i] >> 6] |= (uint64_t)1 << (support[i] & 63);
}

static int getbit(const uint64_t *v, size_t i) { return (int)((v[i >> 6] >> (i & 63)) & 1); }
static void xorbit(uint64_t *v, size_t i) { v[i >> 6] ^= (uint64_t)1 << (i & 63); }

/* Method 2: sparse x dense linear product by word shifts (2n-1 bits), then
 * bit-level fold modulo X^n - 1. */
static void sparse_dense_unreduced(uint64_t *unred /* 2*VEC_N_SIZE_64 */, const uint32_t *support, uint16_t weight,
                                   const uint64_t *h) {
    memset(unred, 0, 2 * VEC_N_SIZE_64 * sizeof(uint64_t));
    for (size_t k = 0; k < weight; k++) {
        uint32_t p = support[k], q = p >> 6, r = p & 63;
        for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
            unred[i + q] ^= h[i] << r;
            if (r) unred[i + q + 1] ^= h[i] >> (64 - r);
        }
    }
}

static void fold_mod_xn_minus_1(uint64_t *red /* VEC_N_SIZE_64 */, const uint64_t *unred) {
    memcpy(red, unred, VEC_N_SIZE_64 * sizeof(uint64_t));
    red[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64); /* keep bits < n */
    for (size_t i = PARAM_N; i < 2 * (size_t)PARAM_N - 1; i++)
        if (getbit(unred, i)) xorbit(red, i - PARAM_N);
}

/* Method 3: sum of cyclic rotations of h, one per set bit of y, bit by bit. */
static void cyclic_rotation_sum(uint64_t *c, const uint32_t *support, uint16_t weight, const uint64_t *h) {
    memset(c, 0, VEC_N_SIZE_64 * sizeof(uint64_t));
    for (size_t k = 0; k < weight; k++) {
        size_t p = support[k];
        for (size_t i = 0; i < PARAM_N; i++)
            if (getbit(h, i)) xorbit(c, (i + p) % PARAM_N);
    }
}

/* ------------------------------------------------------------------------ */
/* Official-file parsers                                                      */
/* ------------------------------------------------------------------------ */
#define LINE_MAX_CHARS 70000
static char g_line[LINE_MAX_CHARS];

/* Find "label: hex" inside the ### KEYGEN ### section of intermediates_values.
 * Returns 0 and fills out (n bytes) on success. */
static int intermediates_get(const char *path, const char *label, uint8_t *out, size_t n) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int in_keygen = 0, found = -1;
    size_t ll = strlen(label);
    while (fgets(g_line, sizeof g_line, f)) {
        if (strstr(g_line, "### KEYGEN ###")) in_keygen = 1;
        else if (strstr(g_line, "### ENCAPS ###")) break;
        if (!in_keygen) continue;
        if (strncmp(g_line, label, ll) == 0 && g_line[ll] == ':' && g_line[ll + 1] == ' ') {
            char *p = g_line + ll + 2;
            p[strcspn(p, "\r\n")] = 0;
            found = parse_hex(p, out, n);
            break;
        }
    }
    fclose(f);
    return found;
}

static void compare_with_intermediates(const char *path, const char *label, const uint8_t *mine, size_t n) {
    static uint8_t ref[CRYPTO_PUBLICKEYBYTES + 64];
    char what[256];
    int rc = intermediates_get(path, label, ref, n);
    snprintf(what, sizeof what, "intermediates_values[KEYGEN].%s == computed (%zu bytes)", label, n);
    check(rc == 0 && memcmp(ref, mine, n) == 0, what);
}

/* Run every KAT entry: sk is seed_kem in v5.1.0, so keygen from sk must give pk. */
static void run_kat(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        check(0, "open KAT response file");
        return;
    }
    static uint8_t pk_ref[CRYPTO_PUBLICKEYBYTES], sk_ref[CRYPTO_SECRETKEYBYTES];
    static uint8_t pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    int count = -1, have_pk = 0, have_sk = 0, n_ok = 0, n_total = 0, n_bad = 0;
    while (fgets(g_line, sizeof g_line, f)) {
        g_line[strcspn(g_line, "\r\n")] = 0;
        if (strncmp(g_line, "count = ", 8) == 0) {
            count = atoi(g_line + 8);
            have_pk = have_sk = 0;
        } else if (strncmp(g_line, "pk = ", 5) == 0) {
            have_pk = parse_hex(g_line + 5, pk_ref, sizeof pk_ref) == 0;
        } else if (strncmp(g_line, "sk = ", 5) == 0) {
            have_sk = parse_hex(g_line + 5, sk_ref, sizeof sk_ref) == 0;
        }
        if (have_pk && have_sk) {
            n_total++;
            rng_load(sk_ref, sizeof sk_ref);
            memset(pk, 0, sizeof pk);
            memset(sk, 0, sizeof sk);
            int rc = hqc_kem_keygen(pk, sk);
            if (rc == 0 && memcmp(pk, pk_ref, sizeof pk) == 0 && memcmp(sk, sk_ref, sizeof sk) == 0) n_ok++;
            else {
                n_bad++;
                note("KAT count %d: MISMATCH", count);
            }
            have_pk = have_sk = 0;
        }
    }
    fclose(f);
    char what[256];
    snprintf(what, sizeof what, "KAT: %d/%d entries, keygen(sk=seed_kem) reproduces pk and sk", n_ok, n_total);
    check(n_total > 0 && n_bad == 0, what);
}

/* ------------------------------------------------------------------------ */
int main(int argc, char **argv) {
    const char *seed_hex = NULL, *imd_path = NULL, *kat_path = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--out") && i + 1 < argc) g_outdir = argv[++i];
        else if (!strcmp(argv[i], "--seed") && i + 1 < argc) seed_hex = argv[++i];
        else if (!strcmp(argv[i], "--intermediates") && i + 1 < argc) imd_path = argv[++i];
        else if (!strcmp(argv[i], "--kat") && i + 1 < argc) kat_path = argv[++i];
        else {
            fprintf(stderr, "usage: %s --out DIR [--seed HEX64] [--intermediates FILE] [--kat FILE]\n", argv[0]);
            return 2;
        }
    }
    if (!g_outdir) {
        fprintf(stderr, "--out is required\n");
        return 2;
    }

    static uint8_t seed_kem[SEED_BYTES];
    if (seed_hex) {
        if (parse_hex(seed_hex, seed_kem, SEED_BYTES)) {
            fprintf(stderr, "--seed must be %d hex characters\n", 2 * SEED_BYTES);
            return 2;
        }
    } else if (imd_path) {
        if (intermediates_get(imd_path, "seed_kem", seed_kem, SEED_BYTES)) {
            fprintf(stderr, "could not read seed_kem from %s\n", imd_path);
            return 2;
        }
    } else {
        fprintf(stderr, "need --seed or --intermediates\n");
        return 2;
    }

    g_summary = out_open("10_summary.txt");
    fprintf(g_summary, "# %s KeyGen golden run: parameters, counts and verification results\n", CRYPTO_ALGNAME);
    note("variant %s: n=%d omega=%d omega_r=%d VEC_N_SIZE_BYTES=%d VEC_N_SIZE_64=%d SEED_BYTES=%d PARAM_K=%d",
         CRYPTO_ALGNAME, PARAM_N, PARAM_OMEGA, PARAM_OMEGA_R, VEC_N_SIZE_BYTES, VEC_N_SIZE_64, SEED_BYTES, PARAM_K);
    note("PUBLIC_KEY_BYTES_PKE=%d (= SEED_BYTES + VEC_N_SIZE_BYTES) SECRET_KEY_BYTES_PKE=%d CRYPTO_SECRETKEYBYTES=%u",
         PUBLIC_KEY_BYTES_PKE, SECRET_KEY_BYTES_PKE, (unsigned)CRYPTO_SECRETKEYBYTES);
    note("UTILS_REJECTION_THRESHOLD=%d (= floor(2^24/n)*n = %d*%d) PARAM_N_MU=%llu (= floor(2^32/n))",
         UTILS_REJECTION_THRESHOLD, (1 << 24) / PARAM_N, PARAM_N, (unsigned long long)PARAM_N_MU);
    note("last 64-bit word of an n-bit vector keeps %d bits (n mod 64), mask 0x%016llx", PARAM_N % 64,
         (unsigned long long)BITMASK(PARAM_N, 64));

    /* ---------------- step 0: seed_kem ---------------- */
    {
        FILE *f = out_open("00_seed_kem.txt");
        fprintf(f, "# %s step 0: seed_kem, %d random bytes drawn by randombytes() in hqc_kem_keygen.\n", CRYPTO_ALGNAME,
                SEED_BYTES);
        fprintf(f, "# In v5.1.0 this IS the KEM decapsulation key dk_kem (compressed dk).\n");
        put_hex(f, seed_kem, SEED_BYTES);
        fclose(f);
    }

    /* ---------------- step 1: seed_pke = XOF(seed_kem || k)[0..31] ---------------- */
    static uint8_t seed_pke[SEED_BYTES];
    size_t perm_count = 0;
    {
        shake256_xof_ctx ctx_kem = {0};
        xof_init_dom_sep(&ctx_kem, seed_kem, SEED_BYTES, PARAM_K);
        shake256_xof_ctx pre = ctx_kem; /* absorbed + padded, not yet permuted */
        xof_get_bytes(&ctx_kem, seed_pke, SEED_BYTES);
        perm_count += 1;

        shake256_xof_ctx mine = pre;
        KeccakF1600_StatePermute(mine.ctx);
        uint8_t mine_out[SEED_BYTES];
        lanes_to_bytes(mine.ctx, mine_out, SEED_BYTES);
        check(memcmp(mine_out, seed_pke, SEED_BYTES) == 0, "step 1: one Keccak permutation of the padded state yields seed_pke");
        check(memcmp(mine.ctx, ctx_kem.ctx, 25 * 8) == 0 && ctx_kem.ctx[25] == SHAKE256_RATE - SEED_BYTES,
              "step 1: library XOF state after squeeze matches (104 bytes left in the block)");

        FILE *f = out_open("01_seed_pke.txt");
        fprintf(f, "# %s step 1: seed_pke = SHAKE256(seed_kem || byte(k))[0..%d), k = PARAM_K = %d (xof_init_dom_sep).\n",
                CRYPTO_ALGNAME, SEED_BYTES, PARAM_K);
        fprintf(f, "# absorbed message (%d bytes): seed_kem || 0x%02x\n", SEED_BYTES + 1, PARAM_K);
        uint8_t msg[SEED_BYTES + 1];
        memcpy(msg, seed_kem, SEED_BYTES);
        msg[SEED_BYTES] = (uint8_t)PARAM_K;
        put_hex(f, msg, sizeof msg);
        fprintf(f, "# SHAKE256 padding: 0x1f XORed at byte %d, 0x80 at byte %d (rate 136)\n", SEED_BYTES + 1, SHAKE256_RATE - 1);
        fprintf(f, "# state before the permutation (25 lanes, lane x+5y as little-endian uint64)\n");
        put_lanes(f, pre.ctx);
        fprintf(f, "# state after permutation 1\n");
        put_lanes(f, mine.ctx);
        fprintf(f, "# seed_pke (first %d bytes of that state). Decaps also squeezes sigma (%d bytes) right after it.\n",
                SEED_BYTES, PARAM_K);
        put_hex(f, seed_pke, SEED_BYTES);
        fclose(f);
    }

    /* ---------------- step 2: (seed_dk, seed_ek) = SHA3-512(seed_pke || 0x02) ---------------- */
    static uint8_t keypair_seed[2 * SEED_BYTES];
    const uint8_t *seed_dk = keypair_seed, *seed_ek = keypair_seed + SEED_BYTES;
    {
        hash_i(keypair_seed, seed_pke);
        perm_count += 1;

        sha3_512incctx c;
        sha3_512_inc_init(&c);
        sha3_512_inc_absorb(&c, seed_pke, SEED_BYTES);
        uint8_t dom = HQC_I_FCT_DOMAIN;
        sha3_512_inc_absorb(&c, &dom, 1);
        keccak_inc_finalize(c.ctx, SHA3_512_RATE, 0x06);
        sha3_512incctx pre = c;
        KeccakF1600_StatePermute(c.ctx);
        uint8_t mine[64];
        lanes_to_bytes(c.ctx, mine, 64);
        check(memcmp(mine, keypair_seed, 64) == 0, "step 2: SHA3-512 recomputed from the padded state matches hash_i");

        FILE *f = out_open("02_seed_dk_seed_ek.txt");
        fprintf(f, "# %s step 2: hash_i = SHA3-512(seed_pke || 0x%02x) -> 64 bytes = seed_dk (32) || seed_ek (32).\n",
                CRYPTO_ALGNAME, HQC_I_FCT_DOMAIN);
        fprintf(f, "# absorbed message (%d bytes)\n", SEED_BYTES + 1);
        uint8_t msg[SEED_BYTES + 1];
        memcpy(msg, seed_pke, SEED_BYTES);
        msg[SEED_BYTES] = dom;
        put_hex(f, msg, sizeof msg);
        fprintf(f, "# SHA3 padding: 0x06 XORed at byte %d, 0x80 at byte %d (rate 72)\n", SEED_BYTES + 1, SHA3_512_RATE - 1);
        fprintf(f, "# state before the permutation\n");
        put_lanes(f, pre.ctx);
        fprintf(f, "# state after the permutation (digest = first 64 bytes)\n");
        put_lanes(f, c.ctx);
        fprintf(f, "# digest (64 bytes)\n");
        put_hex(f, keypair_seed, 64);
        fprintf(f, "# seed_dk\n");
        put_hex(f, seed_dk, SEED_BYTES);
        fprintf(f, "# seed_ek\n");
        put_hex(f, seed_ek, SEED_BYTES);
        fclose(f);
    }

    /* ---------------- steps 3-5: y and x from XOF(seed_dk) ---------------- */
    static uint64_t y[VEC_N_SIZE_64], x[VEC_N_SIZE_64];
    static uint32_t sup_y[PARAM_OMEGA_R], sup_x[PARAM_OMEGA_R];
    static cand_t tr_y[MAX_TRACE], tr_x[MAX_TRACE];
    static uint8_t rawstream[3 * 2 * MAX_TRACE];
    size_t raw_len = 0, t_y, t_x, dk_blocks;
    {
        shake256_xof_ctx dk_ctx = {0};
        xof_init(&dk_ctx, seed_dk, SEED_BYTES);
        shake256_xof_ctx dk_finalized = dk_ctx;

        /* independent trace */
        t_y = trace_support1(&dk_ctx, sup_y, PARAM_OMEGA, tr_y, rawstream, &raw_len);
        size_t raw_after_y = raw_len;
        t_x = trace_support1(&dk_ctx, sup_x, PARAM_OMEGA, tr_x, rawstream, &raw_len);
        dk_blocks = (raw_len + SHAKE256_RATE - 1) / SHAKE256_RATE;
        perm_count += dk_blocks;

        /* library: supports */
        shake256_xof_ctx c2 = dk_finalized;
        static uint32_t lib_sup_y[PARAM_OMEGA_R], lib_sup_x[PARAM_OMEGA_R];
        vect_generate_random_support1(&c2, lib_sup_y, PARAM_OMEGA);
        vect_generate_random_support1(&c2, lib_sup_x, PARAM_OMEGA);
        check(memcmp(lib_sup_y, sup_y, PARAM_OMEGA * 4) == 0,
              "step 4: traced support of y (plain modulo) == vect_generate_random_support1 (Barrett)");
        check(memcmp(lib_sup_x, sup_x, PARAM_OMEGA * 4) == 0,
              "step 5: traced support of x == vect_generate_random_support1");
        check(memcmp(c2.ctx, dk_ctx.ctx, 26 * 8) == 0, "steps 4-5: XOF state after both samplings identical (same bytes consumed)");

        /* library: vectors */
        shake256_xof_ctx c3 = dk_finalized;
        static uint64_t lib_y[VEC_N_SIZE_64], lib_x[VEC_N_SIZE_64];
        memset(lib_y, 0, sizeof lib_y);
        memset(lib_x, 0, sizeof lib_x);
        vect_sample_fixed_weight1(&c3, lib_y, PARAM_OMEGA);
        vect_sample_fixed_weight1(&c3, lib_x, PARAM_OMEGA);
        memset(y, 0, sizeof y);
        memset(x, 0, sizeof x);
        support_to_vector_simple(y, sup_y, PARAM_OMEGA);
        support_to_vector_simple(x, sup_x, PARAM_OMEGA);
        check(memcmp(lib_y, y, sizeof y) == 0, "step 4: direct bit set from support == vect_write_support_to_vector (y)");
        check(memcmp(lib_x, x, sizeof x) == 0, "step 5: direct bit set from support == vect_write_support_to_vector (x)");
        static uint64_t w_y[VEC_N_SIZE_64];
        memset(w_y, 0, sizeof w_y);
        vect_write_support_to_vector(w_y, sup_y, PARAM_OMEGA);
        check(memcmp(w_y, y, sizeof y) == 0, "step 4: vect_write_support_to_vector(support_y) == y");
        int wt_y = 0, wt_x = 0;
        for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
            wt_y += __builtin_popcountll(y[i]);
            wt_x += __builtin_popcountll(x[i]);
        }
        check(wt_y == PARAM_OMEGA && wt_x == PARAM_OMEGA, "steps 4-5: Hamming weight of y and x equals omega");

        /* the squeezed block stream must reproduce the consumed bytes */
        FILE *fb = out_open("03_xof_dk_blocks.txt");
        fprintf(fb, "# %s step 3: SHAKE256(seed_dk) stream used by the sampler: states after each permutation and output blocks.\n",
                CRYPTO_ALGNAME);
        fprintf(fb, "# absorbed message (%d bytes) = seed_dk; padding 0x1f at byte %d, 0x80 at byte %d\n", SEED_BYTES, SEED_BYTES,
                SHAKE256_RATE - 1);
        put_hex(fb, seed_dk, SEED_BYTES);
        fprintf(fb, "# state before the first permutation\n");
        put_lanes(fb, dk_finalized.ctx);
        fprintf(fb, "# blocks consumed: %zu (y and x together used %zu bytes = 3 * %zu candidates)\n", dk_blocks, raw_len, t_y + t_x);
        static uint8_t stream[SHAKE256_RATE * (6 * MAX_TRACE / SHAKE256_RATE + 2)];
        dump_blocks(fb, &dk_finalized, dk_blocks, stream);
        fclose(fb);
        check(memcmp(stream, rawstream, raw_len) == 0, "step 3: concatenated squeezed blocks == bytes consumed by the sampler");

        /* candidate traces */
        const char *names[2] = {"04_y_candidates.txt", "05_x_candidates.txt"};
        const char *vn[2] = {"y", "x"};
        cand_t *trs[2] = {tr_y, tr_x};
        size_t ts[2] = {t_y, t_x};
        size_t offs[2] = {0, raw_after_y};
        for (int k = 0; k < 2; k++) {
            FILE *f = out_open(names[k]);
            fprintf(f, "# %s step %d: rejection sampling of the support of %s (vect_generate_random_support1).\n", CRYPTO_ALGNAME,
                    4 + k, vn[k]);
            fprintf(f, "# Each candidate takes 3 XOF bytes b0 b1 b2 -> cand = b0 | b1<<8 | b2<<16 (24 bits).\n");
            fprintf(f, "# accept iff cand < %d (threshold) and reduced = cand mod %d is not already in the support.\n",
                    UTILS_REJECTION_THRESHOLD, PARAM_N);
            fprintf(f, "# columns: idx stream_byte_offset b0b1b2 cand below_threshold reduced duplicate accepted support_size_after\n");
            size_t acc = 0, nrej = 0, ndup = 0;
            for (size_t i = 0; i < ts[k]; i++) {
                cand_t *c = &trs[k][i];
                if (c->accepted) acc++;
                if (!c->below) nrej++;
                if (c->below && c->dup) ndup++;
                fprintf(f, "%zu %zu %02x%02x%02x %u %d %u %d %d %zu\n", i, offs[k] + 3 * i, c->raw[0], c->raw[1], c->raw[2], c->cand,
                        c->below, c->reduced, c->dup, c->accepted, acc);
            }
            fprintf(f, "# candidates %zu: accepted %zu, rejected by threshold %zu, rejected as duplicate %zu\n", ts[k], acc, nrej, ndup);
            fclose(f);
            note("%s: %zu candidates, %zu rejected by threshold, %zu duplicates, %zu XOF bytes", vn[k], ts[k], nrej, ndup, 3 * ts[k]);
        }

        /* supports and vectors */
        const char *sn[2] = {"04_y_support.txt", "05_x_support.txt"};
        const char *vecn[2] = {"04_y_vector.txt", "05_x_vector.txt"};
        const char *u32n[2] = {"04_y_u32.txt", "05_x_u32.txt"};
        uint32_t *sups[2] = {sup_y, sup_x};
        uint64_t *vecs[2] = {y, x};
        for (int k = 0; k < 2; k++) {
            FILE *f = out_open(sn[k]);
            fprintf(f, "# %s step %d: support of %s, %d distinct positions in [0, %d) in sampling order.\n", CRYPTO_ALGNAME, 4 + k,
                    vn[k], PARAM_OMEGA, PARAM_N);
            fprintf(f, "# line 1: positions in sampling order; line 2: sorted; then one line per position: pos word_index(pos>>6) bit(pos&63)\n");
            for (int i = 0; i < PARAM_OMEGA; i++) fprintf(f, "%s%u", i ? " " : "", sups[k][i]);
            fputc('\n', f);
            uint32_t sorted[PARAM_OMEGA_R];
            memcpy(sorted, sups[k], PARAM_OMEGA * 4);
            for (int i = 1; i < PARAM_OMEGA; i++) {
                uint32_t v = sorted[i];
                int j = i - 1;
                while (j >= 0 && sorted[j] > v) {
                    sorted[j + 1] = sorted[j];
                    j--;
                }
                sorted[j + 1] = v;
            }
            for (int i = 0; i < PARAM_OMEGA; i++) fprintf(f, "%s%u", i ? " " : "", sorted[i]);
            fputc('\n', f);
            for (int i = 0; i < PARAM_OMEGA; i++) fprintf(f, "%u %u %u\n", sups[k][i], sups[k][i] >> 6, sups[k][i] & 63);
            fclose(f);

            f = out_open(vecn[k]);
            fprintf(f, "# %s step %d: %s as an n-bit vector, %d bytes, bit i = bit (i mod 8) of byte (i div 8) (same layout as ek).\n",
                    CRYPTO_ALGNAME, 4 + k, vn[k], VEC_N_SIZE_BYTES);
            put_hex(f, (const uint8_t *)vecs[k], VEC_N_SIZE_BYTES);
            fclose(f);

            f = out_open(u32n[k]);
            fprintf(f, "# %s step %d: %s as %d little-endian 32-bit words (word j = bits 32j..32j+31), one per line.\n",
                    CRYPTO_ALGNAME, 4 + k, vn[k], 2 * VEC_N_SIZE_64);
            put_u32_words(f, vecs[k], VEC_N_SIZE_64);
            fclose(f);
        }
    }

    /* ---------------- step 6: h = XOF(seed_ek) first n bits ---------------- */
    static uint64_t h[VEC_N_SIZE_64];
    size_t ek_blocks = (VEC_N_SIZE_BYTES + SHAKE256_RATE - 1) / SHAKE256_RATE;
    {
        shake256_xof_ctx ek_ctx = {0};
        xof_init(&ek_ctx, seed_ek, SEED_BYTES);
        shake256_xof_ctx ek_finalized = ek_ctx;
        memset(h, 0, sizeof h);
        vect_set_random(&ek_ctx, h);
        perm_count += ek_blocks;

        FILE *fb = out_open("06_xof_ek_blocks.txt");
        fprintf(fb, "# %s step 6: SHAKE256(seed_ek) stream that becomes h: %zu blocks of 136 bytes (%d bytes needed).\n",
                CRYPTO_ALGNAME, ek_blocks, VEC_N_SIZE_BYTES);
        fprintf(fb, "# absorbed message (%d bytes) = seed_ek; padding 0x1f at byte %d, 0x80 at byte %d\n", SEED_BYTES, SEED_BYTES,
                SHAKE256_RATE - 1);
        put_hex(fb, seed_ek, SEED_BYTES);
        fprintf(fb, "# state before the first permutation\n");
        put_lanes(fb, ek_finalized.ctx);
        static uint8_t stream[SHAKE256_RATE * 64];
        dump_blocks(fb, &ek_finalized, ek_blocks, stream);
        fclose(fb);

        static uint64_t mine[VEC_N_SIZE_64];
        memset(mine, 0, sizeof mine);
        memcpy(mine, stream, VEC_N_SIZE_BYTES);
        FILE *f = out_open("06_h_raw.txt");
        fprintf(f, "# %s step 6: the first %d squeezed bytes before masking (vect_set_random then clears bits >= n in the last word).\n",
                CRYPTO_ALGNAME, VEC_N_SIZE_BYTES);
        put_hex(f, (const uint8_t *)mine, VEC_N_SIZE_BYTES);
        fclose(f);
        mine[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64);
        check(memcmp(mine, h, sizeof h) == 0, "step 6: squeezed blocks truncated to n bits == vect_set_random output h");
        check(ek_ctx.ctx[25] == ek_blocks * SHAKE256_RATE - VEC_N_SIZE_BYTES,
              "step 6: library consumed exactly ceil(VEC_N_SIZE_BYTES/136) blocks");

        f = out_open("06_h.txt");
        fprintf(f, "# %s step 6: h, n-bit vector, %d bytes (bits >= n cleared).\n", CRYPTO_ALGNAME, VEC_N_SIZE_BYTES);
        put_hex(f, (const uint8_t *)h, VEC_N_SIZE_BYTES);
        fclose(f);
        f = out_open("06_h_u32.txt");
        fprintf(f, "# %s step 6: h as %d little-endian 32-bit words, one per line.\n", CRYPTO_ALGNAME, 2 * VEC_N_SIZE_64);
        put_u32_words(f, h, VEC_N_SIZE_64);
        fclose(f);
    }

    /* ---------------- step 7: y * h mod X^n - 1 ---------------- */
    static uint64_t yh[VEC_N_SIZE_64], unred[2 * VEC_N_SIZE_64], yh2[VEC_N_SIZE_64], yh3[VEC_N_SIZE_64];
    {
        memset(yh, 0, sizeof yh);
        vect_mul(yh, y, h);
        sparse_dense_unreduced(unred, sup_y, PARAM_OMEGA, h);
        fold_mod_xn_minus_1(yh2, unred);
        cyclic_rotation_sum(yh3, sup_y, PARAM_OMEGA, h);
        check(memcmp(yh, yh2, sizeof yh) == 0, "step 7: vect_mul(y,h) == word-shift sparse product folded mod X^n-1");
        check(memcmp(yh, yh3, sizeof yh) == 0, "step 7: vect_mul(y,h) == bit-level sum of cyclic rotations rot(h, p), p in supp(y)");

        size_t unred_bytes = (2 * (size_t)PARAM_N - 1 + 7) / 8;
        FILE *f = out_open("07_yh_unreduced.txt");
        fprintf(f, "# %s step 7a: linear product y(X)*h(X) over GF(2) before reduction, degree < 2n-1, %zu bytes.\n", CRYPTO_ALGNAME,
                unred_bytes);
        fprintf(f, "# computed as XOR over p in supp(y) of (h << p); reduction mod X^n-1 folds bit i (i >= n) onto bit i-n.\n");
        put_hex(f, (const uint8_t *)unred, unred_bytes);
        fclose(f);
        f = out_open("07_yh.txt");
        fprintf(f, "# %s step 7b: y*h mod X^n-1, n-bit vector, %d bytes (vect_mul).\n", CRYPTO_ALGNAME, VEC_N_SIZE_BYTES);
        put_hex(f, (const uint8_t *)yh, VEC_N_SIZE_BYTES);
        fclose(f);
        f = out_open("07_yh_u32.txt");
        fprintf(f, "# %s step 7b: y*h mod X^n-1 as %d little-endian 32-bit words, one per line.\n", CRYPTO_ALGNAME, 2 * VEC_N_SIZE_64);
        put_u32_words(f, yh, VEC_N_SIZE_64);
        fclose(f);
    }

    /* ---------------- step 8: s = x + y*h ---------------- */
    static uint64_t s[VEC_N_SIZE_64];
    {
        vect_add(s, x, yh, VEC_N_SIZE_64);
        static uint64_t s2[VEC_N_SIZE_64];
        memcpy(s2, yh, sizeof s2);
        for (int i = 0; i < PARAM_OMEGA; i++) xorbit(s2, sup_x[i]); /* index-based add */
        check(memcmp(s, s2, sizeof s) == 0, "step 8: vect_add(x, yh) == yh with the omega positions of supp(x) toggled");
        FILE *f = out_open("08_s.txt");
        fprintf(f, "# %s step 8: s = x + y*h (XOR), n-bit vector, %d bytes.\n", CRYPTO_ALGNAME, VEC_N_SIZE_BYTES);
        put_hex(f, (const uint8_t *)s, VEC_N_SIZE_BYTES);
        fclose(f);
        f = out_open("08_s_u32.txt");
        fprintf(f, "# %s step 8: s as %d little-endian 32-bit words, one per line.\n", CRYPTO_ALGNAME, 2 * VEC_N_SIZE_64);
        put_u32_words(f, s, VEC_N_SIZE_64);
        fclose(f);
    }

    /* ---------------- step 9: keys ---------------- */
    static uint8_t ek[PUBLIC_KEY_BYTES_PKE], dk_pke[SECRET_KEY_BYTES_PKE];
    {
        memcpy(ek, seed_ek, SEED_BYTES);
        memcpy(ek + SEED_BYTES, s, VEC_N_SIZE_BYTES);
        memcpy(dk_pke, seed_dk, SEED_BYTES);

        static uint8_t ek_lib[PUBLIC_KEY_BYTES_PKE], dk_lib[SECRET_KEY_BYTES_PKE];
        hqc_pke_keygen(ek_lib, dk_lib, seed_pke);
        check(memcmp(ek_lib, ek, sizeof ek) == 0, "step 9: assembled ek_pke == hqc_pke_keygen(seed_pke) ek");
        check(memcmp(dk_lib, dk_pke, sizeof dk_pke) == 0, "step 9: dk_pke == hqc_pke_keygen(seed_pke) dk (= seed_dk)");

        static uint8_t pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
        rng_load(seed_kem, SEED_BYTES);
        int rc = hqc_kem_keygen(pk, sk);
        check(rc == 0 && memcmp(pk, ek, sizeof ek) == 0, "step 9: hqc_kem_keygen() with randombytes = seed_kem gives ek_kem == ek_pke");
        check(rc == 0 && memcmp(sk, seed_kem, SEED_BYTES) == 0, "step 9: hqc_kem_keygen() dk_kem == seed_kem");

        FILE *f = out_open("09_ek.txt");
        fprintf(f, "# %s step 9: ek_pke = ek_kem = seed_ek (%d bytes) || s (%d bytes) = %d bytes (KAT 'pk').\n", CRYPTO_ALGNAME,
                SEED_BYTES, VEC_N_SIZE_BYTES, PUBLIC_KEY_BYTES_PKE);
        put_hex(f, ek, sizeof ek);
        fclose(f);
        f = out_open("09_dk.txt");
        fprintf(f, "# %s step 9: line 1 dk_pke = seed_dk (%d bytes); line 2 dk_kem = seed_kem (%u bytes, KAT 'sk').\n", CRYPTO_ALGNAME,
                SECRET_KEY_BYTES_PKE, (unsigned)CRYPTO_SECRETKEYBYTES);
        put_hex(f, dk_pke, sizeof dk_pke);
        put_hex(f, seed_kem, SEED_BYTES);
        fclose(f);
    }

    note("Keccak-f[1600] permutations in this KeyGen: 1 (seed_pke) + 1 (hash_i) + %zu (dk XOF, sampling) + %zu (ek XOF, h) = %zu",
         dk_blocks, ek_blocks, perm_count);

    /* ---------------- verification against official files ---------------- */
    if (imd_path) {
        compare_with_intermediates(imd_path, "seed_kem", seed_kem, SEED_BYTES);
        compare_with_intermediates(imd_path, "seed_pke", seed_pke, SEED_BYTES);
        compare_with_intermediates(imd_path, "seed_dk", seed_dk, SEED_BYTES);
        compare_with_intermediates(imd_path, "seed_ek", seed_ek, SEED_BYTES);
        compare_with_intermediates(imd_path, "y", (const uint8_t *)y, VEC_N_SIZE_BYTES);
        compare_with_intermediates(imd_path, "x", (const uint8_t *)x, VEC_N_SIZE_BYTES);
        compare_with_intermediates(imd_path, "h", (const uint8_t *)h, VEC_N_SIZE_BYTES);
        compare_with_intermediates(imd_path, "s", (const uint8_t *)s, VEC_N_SIZE_BYTES);
        compare_with_intermediates(imd_path, "ek_pke", ek, PUBLIC_KEY_BYTES_PKE);
        compare_with_intermediates(imd_path, "dk_pke", dk_pke, SECRET_KEY_BYTES_PKE);
    }
    if (kat_path) run_kat(kat_path);

    note("RESULT: %s", g_fail ? "SOME CHECKS FAILED" : "ALL CHECKS PASSED");
    fclose(g_summary);
    return g_fail ? 1 : 0;
}
