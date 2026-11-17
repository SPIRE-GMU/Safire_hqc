/* Layer 1: crypto_kem_keypair / enc / dec rewritten statement-by-statement from kem.c + hqc.c
 * (+ parsing.c's key_from_string bodies inlined so that h-expansion and x,y recovery get their own
 * stages), with rdtsc between statements.  Same deterministic randombytes stream as the library;
 * outputs are verified byte-for-byte against the library functions before timing.
 * Categories: MUL=vect_mul, FW=fixed-weight sampling, HEXP=h expansion (vect_set_random),
 * SHAKE=shake256_512_ds G/K and seedexpander init/release, CODE=code_encode/decode, OTHER=rest
 * (RNG = our deterministic randombytes stub, reported separately and folded into OTHER). */
#include "common.h"
#include "api.h"
#include "parameters.h"
#include "domains.h"
#include "fips202.h"
#include "randombytes.h"
#include "shake_ds.h"
#include "shake_prng.h"
#include "vector.h"
#include "parsing.h"
#include "gf2x.h"
#include "code.h"

/* ---- staged keypair (hqc_pke_keygen) ---- */
static void staged_keypair(uint8_t *pk, uint8_t *sk) {
    PROF_DECL; uint64_t _t0 = tsc_begin(); PROF_START();
    seedexpander_state sk_seedexpander;
    seedexpander_state pk_seedexpander;
    uint8_t sk_seed[SEED_BYTES] = {0};
    uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
    uint8_t pk_seed[SEED_BYTES] = {0};
    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};
    PROF_NEXT("keypair/00 zero-init locals", "OTHER");
    randombytes(sk_seed, SEED_BYTES);
    randombytes(sigma, VEC_K_SIZE_BYTES);
    PROF_NEXT("keypair/01 randombytes(sk_seed,sigma)", "RNG");
    H(seedexpander_init)(&sk_seedexpander, sk_seed, SEED_BYTES);
    PROF_NEXT("keypair/02 seedexpander_init(sk)", "SHAKE");
    randombytes(pk_seed, SEED_BYTES);
    PROF_NEXT("keypair/03 randombytes(pk_seed)", "RNG");
    H(seedexpander_init)(&pk_seedexpander, pk_seed, SEED_BYTES);
    PROF_NEXT("keypair/04 seedexpander_init(pk)", "SHAKE");
    H(vect_set_random_fixed_weight)(&sk_seedexpander, x, PARAM_OMEGA);
    PROF_NEXT("keypair/05 vect_set_random_fixed_weight(x)", "FW");
    H(vect_set_random_fixed_weight)(&sk_seedexpander, y, PARAM_OMEGA);
    PROF_NEXT("keypair/06 vect_set_random_fixed_weight(y)", "FW");
    H(vect_set_random)(&pk_seedexpander, h);
    PROF_NEXT("keypair/07 vect_set_random(h)", "HEXP");
    H(vect_mul)(s, y, h);
    PROF_NEXT("keypair/08 vect_mul(s=y*h)", "MUL");
    H(vect_add)(s, x, s, VEC_N_SIZE_64);
    PROF_NEXT("keypair/09 vect_add(s+=x)", "OTHER");
    H(hqc_public_key_to_string)(pk, pk_seed, s);
    PROF_NEXT("keypair/10 hqc_public_key_to_string", "OTHER");
    H(hqc_secret_key_to_string)(sk, sk_seed, sigma, pk);
    PROF_NEXT("keypair/11 hqc_secret_key_to_string", "OTHER");
    H(seedexpander_release)(&pk_seedexpander);
    H(seedexpander_release)(&sk_seedexpander);
    PROF_STOP("keypair/12 seedexpander_release x2", "SHAKE");
    { uint64_t _e = tsc_end(); static int _id = -1; if (_id < 0) _id = stage_id("keypair/TOTAL(staged)", "TOTAL"); stage_add(_id, _e - _t0); }
}

/* ---- staged hqc_pke_encrypt, with hqc_public_key_from_string inlined; used by enc and dec ---- */
#define STAGED_ENCRYPT(PFX, u, v, m, theta, pk)                                                    \
    do {                                                                                           \
        seedexpander_state vec_seedexpander;                                                       \
        uint64_t h[VEC_N_SIZE_64] = {0};                                                           \
        uint64_t s[VEC_N_SIZE_64] = {0};                                                           \
        uint64_t r1[VEC_N_SIZE_64] = {0};                                                          \
        uint64_t r2[VEC_N_SIZE_64] = {0};                                                          \
        uint64_t e[VEC_N_SIZE_64] = {0};                                                           \
        uint64_t tmp1[VEC_N_SIZE_64] = {0};                                                        \
        uint64_t tmp2[VEC_N_SIZE_64] = {0};                                                        \
        seedexpander_state pk_seedexpander;                                                        \
        PROF_NEXT(PFX "05 encrypt: zero-init locals", "OTHER");                                   \
        H(seedexpander_init)(&vec_seedexpander, theta, SEED_BYTES);                                \
        PROF_NEXT(PFX "06 seedexpander_init(theta)", "SHAKE");                                     \
        /* hqc_public_key_from_string(h, s, pk) inlined: */                                        \
        H(seedexpander_init)(&pk_seedexpander, pk, SEED_BYTES);                                    \
        PROF_NEXT(PFX "07 pk_from_string: seedexpander_init(pk_seed)", "SHAKE");                   \
        H(vect_set_random)(&pk_seedexpander, h);                                                   \
        PROF_NEXT(PFX "08 pk_from_string: vect_set_random(h)", "HEXP");                            \
        H(load8_arr)(s, VEC_N_SIZE_64, pk + SEED_BYTES, VEC_N_SIZE_BYTES);                         \
        PROF_NEXT(PFX "09 pk_from_string: load8_arr(s)", "OTHER");                                 \
        H(seedexpander_release)(&pk_seedexpander);                                                 \
        PROF_NEXT(PFX "10 pk_from_string: seedexpander_release", "SHAKE");                         \
        H(vect_set_random_fixed_weight)(&vec_seedexpander, r1, PARAM_OMEGA_R);                     \
        PROF_NEXT(PFX "11 vect_set_random_fixed_weight(r1)", "FW");                                \
        H(vect_set_random_fixed_weight)(&vec_seedexpander, r2, PARAM_OMEGA_R);                     \
        PROF_NEXT(PFX "12 vect_set_random_fixed_weight(r2)", "FW");                                \
        H(vect_set_random_fixed_weight)(&vec_seedexpander, e, PARAM_OMEGA_E);                      \
        PROF_NEXT(PFX "13 vect_set_random_fixed_weight(e)", "FW");                                 \
        H(vect_mul)(u, r2, h);                                                                     \
        PROF_NEXT(PFX "14 vect_mul(u=r2*h)", "MUL");                                               \
        H(vect_add)(u, r1, u, VEC_N_SIZE_64);                                                      \
        PROF_NEXT(PFX "15 vect_add(u+=r1)", "OTHER");                                              \
        H(code_encode)(v, m);                                                                      \
        PROF_NEXT(PFX "16 code_encode(v=mG)", "CODE");                                             \
        H(vect_resize)(tmp1, PARAM_N, v, PARAM_N1N2);                                              \
        PROF_NEXT(PFX "17 vect_resize(tmp1<-v)", "OTHER");                                         \
        H(vect_mul)(tmp2, r2, s);                                                                  \
        PROF_NEXT(PFX "18 vect_mul(tmp2=r2*s)", "MUL");                                            \
        H(vect_add)(tmp2, e, tmp2, VEC_N_SIZE_64);                                                 \
        H(vect_add)(tmp2, tmp1, tmp2, VEC_N_SIZE_64);                                              \
        PROF_NEXT(PFX "19 vect_add x2 (tmp2+=e, +=tmp1)", "OTHER");                                \
        H(vect_resize)(v, PARAM_N1N2, tmp2, PARAM_N);                                              \
        PROF_NEXT(PFX "20 vect_resize(v<-tmp2)", "OTHER");                                         \
        H(seedexpander_release)(&vec_seedexpander);                                                \
        PROF_NEXT(PFX "21 seedexpander_release(theta)", "SHAKE");                                  \
    } while (0)

static void staged_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    PROF_DECL; uint64_t _t0 = tsc_begin(); PROF_START();
    uint8_t theta[SHAKE256_512_BYTES] = {0};
    uint64_t u[VEC_N_SIZE_64] = {0};
    uint64_t v[VEC_N1N2_SIZE_64] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
    uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
    uint8_t *m = tmp;
    uint8_t *salt = tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES;
    shake256incctx shake256state;
    PROF_NEXT("enc/00 zero-init locals", "OTHER");
    randombytes(m, VEC_K_SIZE_BYTES);
    PROF_NEXT("enc/01 randombytes(m)", "RNG");
    randombytes(salt, SALT_SIZE_BYTES);
    PROF_NEXT("enc/02 randombytes(salt)", "RNG");
    memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
    PROF_NEXT("enc/03 memcpy(pk->tmp)", "OTHER");
    H(shake256_512_ds)(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);
    PROF_NEXT("enc/04 shake256_512_ds G(m||pk||salt)", "SHAKE");
    STAGED_ENCRYPT("enc/", u, v, m, theta, pk);
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    H(store8_arr)(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
    H(store8_arr)(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    PROF_NEXT("enc/22 memcpy(m)+store8_arr(u,v)->mc", "OTHER");
    H(shake256_512_ds)(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);
    PROF_NEXT("enc/23 shake256_512_ds K(m||u||v)", "SHAKE");
    H(hqc_ciphertext_to_string)(ct, u, v, salt);
    PROF_STOP("enc/24 hqc_ciphertext_to_string", "OTHER");
    { uint64_t _e = tsc_end(); static int _id = -1; if (_id < 0) _id = stage_id("enc/TOTAL(staged)", "TOTAL"); stage_add(_id, _e - _t0); }
}

static int staged_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    PROF_DECL; uint64_t _t0 = tsc_begin(); PROF_START();
    uint8_t result;
    uint64_t u[VEC_N_SIZE_64] = {0};
    uint64_t v[VEC_N1N2_SIZE_64] = {0};
    const uint8_t *pk = sk + SEED_BYTES + VEC_K_SIZE_BYTES;
    uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[SHAKE256_512_BYTES] = {0};
    uint64_t u2[VEC_N_SIZE_64] = {0};
    uint64_t v2[VEC_N1N2_SIZE_64] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
    uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
    uint8_t *m = tmp;
    uint8_t *salt = tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES;
    shake256incctx shake256state;
    /* hqc_pke_decrypt locals */
    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint8_t pk2[PUBLIC_KEY_BYTES] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};
    seedexpander_state sk_seedexpander;
    PROF_NEXT("dec/00 zero-init locals (kem+decrypt)", "OTHER");
    H(hqc_ciphertext_from_string)(u, v, salt, ct);
    PROF_NEXT("dec/01 hqc_ciphertext_from_string", "OTHER");
    /* hqc_pke_decrypt(m, sigma, u, v, sk) with hqc_secret_key_from_string inlined: */
    memcpy(sigma, sk + SEED_BYTES, VEC_K_SIZE_BYTES);
    PROF_NEXT("dec/02 sk_from_string: memcpy(sigma)", "OTHER");
    H(seedexpander_init)(&sk_seedexpander, sk, SEED_BYTES);
    PROF_NEXT("dec/03 sk_from_string: seedexpander_init(sk_seed)", "SHAKE");
    H(vect_set_random_fixed_weight)(&sk_seedexpander, x, PARAM_OMEGA);
    PROF_NEXT("dec/04 sk_from_string: vect_set_random_fixed_weight(x)", "FW");
    H(vect_set_random_fixed_weight)(&sk_seedexpander, y, PARAM_OMEGA);
    PROF_NEXT("dec/05 sk_from_string: vect_set_random_fixed_weight(y)", "FW");
    memcpy(pk2, sk + SEED_BYTES + VEC_K_SIZE_BYTES, PUBLIC_KEY_BYTES);
    PROF_NEXT("dec/06 sk_from_string: memcpy(pk)", "OTHER");
    H(seedexpander_release)(&sk_seedexpander);
    PROF_NEXT("dec/07 sk_from_string: seedexpander_release", "SHAKE");
    H(vect_resize)(tmp1, PARAM_N, v, PARAM_N1N2);
    PROF_NEXT("dec/08 vect_resize(tmp1<-v)", "OTHER");
    H(vect_mul)(tmp2, y, u);
    PROF_NEXT("dec/09 vect_mul(tmp2=y*u)", "MUL");
    H(vect_add)(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
    PROF_NEXT("dec/10 vect_add(tmp2+=tmp1)", "OTHER");
    H(code_decode)(m, tmp2);
    PROF_NEXT("dec/11 code_decode(m)", "CODE");
    result = 0;
    /* back in crypto_kem_dec */
    memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
    PROF_NEXT("dec/12 memcpy(pk->tmp)", "OTHER");
    H(shake256_512_ds)(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);
    PROF_NEXT("dec/13 shake256_512_ds G(m'||pk||salt)", "SHAKE");
    STAGED_ENCRYPT("dec/reenc ", u2, v2, m, theta, pk);
    result |= H(vect_compare)((uint8_t *)u, (uint8_t *)u2, VEC_N_SIZE_BYTES);
    result |= H(vect_compare)((uint8_t *)v, (uint8_t *)v2, VEC_N1N2_SIZE_BYTES);
    PROF_NEXT("dec/22 vect_compare(u,u') + (v,v')", "OTHER");
    result -= 1;
    for (size_t i = 0; i < VEC_K_SIZE_BYTES; ++i) {
        mc[i] = (m[i] & result) ^ (sigma[i] & ~result);
    }
    PROF_NEXT("dec/23 ct-select m/sigma -> mc", "OTHER");
    H(store8_arr)(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
    H(store8_arr)(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    PROF_NEXT("dec/24 store8_arr(u,v)->mc", "OTHER");
    H(shake256_512_ds)(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);
    PROF_STOP("dec/25 shake256_512_ds K(m||u||v)", "SHAKE");
    { uint64_t _e = tsc_end(); static int _id = -1; if (_id < 0) _id = stage_id("dec/TOTAL(staged)", "TOTAL"); stage_add(_id, _e - _t0); }
    return (result & 1) - 1;
}

static void seed_for(uint64_t i, uint8_t *seed) { memset(seed, 0, 32); memcpy(seed, &i, 8); seed[31] = 0x5A; }

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2000;
    int warm = argc > 2 ? atoi(argv[2]) : 100;
    const char *csv = argc > 3 ? argv[3] : NULL;
    static uint8_t pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES], ct[CRYPTO_CIPHERTEXTBYTES], ss1[CRYPTO_BYTES], ss2[CRYPTO_BYTES];
    static uint8_t pkS[CRYPTO_PUBLICKEYBYTES], skS[CRYPTO_SECRETKEYBYTES], ctS[CRYPTO_CIPHERTEXTBYTES], ss1S[CRYPTO_BYTES], ss2S[CRYPTO_BYTES];
    uint8_t seed[32];

    /* --- verification: staged == library on 50 independent random streams --- */
    int bad = 0;
    for (uint64_t i = 0; i < 50; i++) {
        seed_for(1000 + i, seed);
        randombytes_reseed(seed, 32); crypto_kem_keypair(pk, sk); crypto_kem_enc(ct, ss1, pk); int r1 = crypto_kem_dec(ss2, ct, sk);
        randombytes_reseed(seed, 32); staged_keypair(pkS, skS); staged_enc(ctS, ss1S, pkS); int r2 = staged_dec(ss2S, ctS, skS);
        if (memcmp(pk, pkS, sizeof pk) || memcmp(sk, skS, sizeof sk) || memcmp(ct, ctS, sizeof ct) ||
            memcmp(ss1, ss1S, CRYPTO_BYTES) || memcmp(ss2, ss2S, CRYPTO_BYTES) || memcmp(ss1, ss2, CRYPTO_BYTES) || r1 != r2) bad++;
    }
    printf("VERIFY staged==library (pk,sk,ct,ss_enc,ss_dec, 50 streams): %s\n", bad ? "MISMATCH" : "OK");
    if (bad) return 1;
    for (int i = 0; i < g_nstages; i++) g_stages[i].n = 0; /* drop verification samples */

    /* --- library totals (un-instrumented) --- */
    int id_kp = stage_id("keypair/TOTAL(library)", "TOTAL"), id_enc = stage_id("enc/TOTAL(library)", "TOTAL"), id_dec = stage_id("dec/TOTAL(library)", "TOTAL");
    for (int i = 0; i < warm + n; i++) {
        seed_for(i, seed); randombytes_reseed(seed, 32);
        uint64_t t0 = tsc_begin(); crypto_kem_keypair(pk, sk); uint64_t t1 = tsc_end();
        uint64_t t2 = tsc_begin(); crypto_kem_enc(ct, ss1, pk); uint64_t t3 = tsc_end();
        uint64_t t4 = tsc_begin(); crypto_kem_dec(ss2, ct, sk); uint64_t t5 = tsc_end();
        if (memcmp(ss1, ss2, CRYPTO_BYTES)) { printf("ss mismatch in timing loop\n"); return 1; }
        if (i >= warm) { stage_add(id_kp, t1 - t0); stage_add(id_enc, t3 - t2); stage_add(id_dec, t5 - t4); }
    }
    /* --- staged (instrumented) --- */
    for (int i = 0; i < warm; i++) { seed_for(i, seed); randombytes_reseed(seed, 32); staged_keypair(pk, sk); staged_enc(ct, ss1, pk); staged_dec(ss2, ct, sk); }
    for (int i = 0; i < g_nstages; i++) if (strcmp(g_stages[i].cat, "TOTAL") || strstr(g_stages[i].name, "staged")) g_stages[i].n = 0;
    for (int i = 0; i < n; i++) { seed_for(i, seed); randombytes_reseed(seed, 32); staged_keypair(pk, sk); staged_enc(ct, ss1, pk); staged_dec(ss2, ct, sk);
        if (memcmp(ss1, ss2, CRYPTO_BYTES)) { printf("ss mismatch in staged loop\n"); return 1; } }

    double ghz = tsc_ghz_measure();
    printf("samples: n=%d warmup=%d\n", n, warm);
    print_report(ghz, csv, "kem");
    return 0;
}
