// Packed Karatsuba. Caller owns all buffers; no runtime recursion.
#ifndef KARA_CTRL_H
#define KARA_CTRL_H
#include "leaf.h"
namespace kara {
template<uint32_t DEPTH> struct KaraNode {
    static const uint32_t WORDS = LEAF_WORDS << DEPTH;
    static const uint32_t HALF = WORDS / 2u;
    static void run(const uint32_t* __restrict a, const uint32_t* __restrict b,
                    uint32_t* __restrict out, uint32_t* __restrict scratch) {
        // Reserve two HALF-word preadds and one WORDS-word w at this level.
        uint32_t* pre_a = scratch;
        uint32_t* pre_b = scratch + HALF;
        uint32_t* w = scratch + WORDS;
        uint32_t* child_scratch = scratch + 2u * WORDS;
        KaraNode<DEPTH - 1u>::run(a, b, out, child_scratch);
        KaraNode<DEPTH - 1u>::run(a + HALF, b + HALF, out + WORDS, child_scratch);
        for (uint32_t i = 0; i < HALF; ++i)
        KARA_PIPELINE
        {
            pre_a[i] = a[i] ^ a[HALF + i];
            pre_b[i] = b[i] ^ b[HALF + i];
        }
        KaraNode<DEPTH - 1u>::run(pre_a, pre_b, w, child_scratch);
        // This loop reads out only. Finish it before any overlapping out write.
        for (uint32_t i = 0; i < WORDS; ++i)
        KARA_PIPELINE
        { w[i] ^= out[i] ^ out[WORDS + i]; }
        for (uint32_t i = 0; i < WORDS; ++i)
        KARA_PIPELINE
        { out[HALF + i] ^= w[i]; }
    }
};
template<> struct KaraNode<0u> {
    static void run(const uint32_t* __restrict a, const uint32_t* __restrict b,
                    uint32_t* __restrict out, uint32_t*) {
        leaf_conv(a, b, out);
    }
};
inline void compute(const uint32_t* __restrict a, const uint32_t* __restrict b,
                    uint32_t* __restrict product, uint32_t* __restrict scratch) {
    KaraNode<LOCAL_LEVELS>::run(a, b, product, scratch);
}
} // namespace kara
#endif
