/* Correctness: ROUNDS x (keypair, enc, dec), shared secrets equal; counts vect_mul calls per op
 * via linker wrapping (-Wl,--wrap=<ns>vect_mul). */
#include "common.h"
#include "api.h"
#include "parameters.h"
#include "randombytes.h"

static unsigned long g_mulcalls = 0;
void CAT2(__real_, H(vect_mul))(uint64_t *o, const uint64_t *v1, const uint64_t *v2);
void CAT2(__wrap_, H(vect_mul))(uint64_t *o, const uint64_t *v1, const uint64_t *v2) {
    g_mulcalls++;
    CAT2(__real_, H(vect_mul))(o, v1, v2);
}

int main(int argc, char **argv) {
    int rounds = argc > 1 ? atoi(argv[1]) : 1000;
    static uint8_t pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES], ct[CRYPTO_CIPHERTEXTBYTES];
    static uint8_t ss1[CRYPTO_BYTES], ss2[CRYPTO_BYTES];
    uint8_t seed[32]; fill_random_bytes(seed, 32); randombytes_reseed(seed, 32);
    int fails = 0, rc_fail = 0;
    unsigned long c_kp = 0, c_enc = 0, c_dec = 0; int count_consistent = 1;
    for (int r = 0; r < rounds; r++) {
        g_mulcalls = 0; int rc = crypto_kem_keypair(pk, sk); unsigned long a = g_mulcalls;
        g_mulcalls = 0; rc |= crypto_kem_enc(ct, ss1, pk); unsigned long b = g_mulcalls;
        g_mulcalls = 0; rc |= crypto_kem_dec(ss2, ct, sk); unsigned long c = g_mulcalls;
        if (rc) rc_fail++;
        if (memcmp(ss1, ss2, CRYPTO_BYTES)) fails++;
        if (r == 0) { c_kp = a; c_enc = b; c_dec = c; }
        else if (a != c_kp || b != c_enc || c != c_dec) count_consistent = 0;
    }
    /* also check that a corrupted ciphertext yields a different secret (sanity of dec path) */
    ct[0] ^= 1; crypto_kem_dec(ss2, ct, sk); int reject_ok = memcmp(ss1, ss2, CRYPTO_BYTES) != 0;
    printf("KEMCHECK %s %s rounds=%d ss_mismatch=%d rc_nonzero=%d corrupt_ct_rejected=%d "
           "vect_mul_calls keypair=%lu enc=%lu dec=%lu consistent=%d => %s\n",
           SET_NAME, OPT_NAME, rounds, fails, rc_fail, reject_ok, c_kp, c_enc, c_dec, count_consistent,
           (fails == 0 && rc_fail == 0 && reject_ok && count_consistent) ? "PASS" : "FAIL");
    return (fails || rc_fail || !reject_ok) ? 1 : 0;
}
