// AIE1 int16 leaf, Vitis 2023.2. Include once per kernel source file.
#ifndef KARA_LEAF_AIE_H
#define KARA_LEAF_AIE_H
#include <adf.h>
#include <aie_api/aie.hpp>
#include "leaf.h"

#ifndef KARA_PARITY_P1
#define KARA_PARITY_P1 0
#endif
#ifndef KARA_MASK_BITREV
#define KARA_MASK_BITREV 0
#endif
#ifndef KARA_LEAF_PROFILE
#define KARA_LEAF_PROFILE 0
#endif
#ifndef KARA_STATIC_TRIPS
#define KARA_STATIC_TRIPS 0
#endif
#ifndef KARA_NOINLINE
#define KARA_NOINLINE __attribute__((noinline))
#endif
// One kernel source file may serve many kernel instances (the work tiles of a chain).
// On the AI Engine every core is a program of its own, so nothing is shared.
// x86simulator links all instances into one image and runs each in its own thread:
// there the leaf must be a single inline definition and its work arrays per thread.
#ifdef __X86SIM__
#define KARA_LEAF_LINKAGE inline
#else
#define KARA_LEAF_LINKAGE
#endif
#ifndef X86SIM_THREAD_LOCAL   // defined by adf.h: thread_local in x86simulator, empty otherwise
#ifdef __X86SIM__
#define X86SIM_THREAD_LOCAL thread_local
#else
#define X86SIM_THREAD_LOCAL
#endif
#endif
#if KARA_STATIC_TRIPS
// Share the two MAC bodies across all fixed-trip wrappers.
#define KARA_LEAF_GROUP_ATTR KARA_NOINLINE
#else
#define KARA_LEAF_GROUP_ATTR inline
#endif

namespace kara {
// Each 16-output block covers coefficient interval [begin,end), aligned to 32.
// For k=16*block and reversed bR[t]=b[L-1-t], lane l computes
//   sum_t bR[t] * a[k-L+1+t+l] = (a*b)[k+l].
// The union of nonzero coefficient indices is
// [max(0,L-16-k), min(L,2*L-1-k)), before alignment.
// The default path dispatches the public block's trip count to a template
// instance with a compile-time loop bound. The MAC bodies remain shared.
constexpr unsigned leaf_block_begin(unsigned block) {
    return (16u * block + 16u < L_BITS)
         ? ((L_BITS - 16u * block - 16u) & ~31u) : 0u;
}
constexpr unsigned leaf_block_end(unsigned block) {
    return (((2u * L_BITS - 1u - 16u * block < L_BITS)
          ? (2u * L_BITS - 1u - 16u * block) : L_BITS) + 31u) & ~31u;
}
constexpr unsigned leaf_block_instructions(unsigned block) {
    return (leaf_block_end(block) - leaf_block_begin(block)) / 2u;
}
constexpr unsigned leaf_mac_instructions() {
    // Exactly L*L/32 useful mul/mac-equivalent work, plus L alignment calls.
    // Includes the multiply that initializes each block and all following MACs.
    return L_BITS * L_BITS / 32u + L_BITS;
}
constexpr unsigned leaf_static_bytes() { return 4u * L_BITS + 128u; }

#if KARA_LEAF_PROFILE
inline uint64_t* leaf_profile_cycles() {
    // Last leaf invocation only; one copy per kernel instance.
    static X86SIM_THREAD_LOCAL uint64_t cycles[3];
    return cycles;
}
#endif

inline uint32_t leaf_reverse_word(uint32_t word) {
    word = ((word & 0x55555555u) << 1u) | ((word >> 1u) & 0x55555555u);
    word = ((word & 0x33333333u) << 2u) | ((word >> 2u) & 0x33333333u);
    word = ((word & 0x0f0f0f0fu) << 4u) | ((word >> 4u) & 0x0f0f0f0fu);
    word = ((word & 0x00ff00ffu) << 8u) | ((word >> 8u) & 0x00ff00ffu);
    return (word << 16u) | (word >> 16u);
}

template<bool FIRST> struct LeafGroupStart;
template<> struct LeafGroupStart<true> {
    static void run(aie::accum<acc48, 16>& acc,
                    const aie::vector<int16, 16>& coeff,
                    const aie::vector<int16, 32>& data) {
        acc = aie::sliding_mul<16, 2, 1, 1, 1, acc48>(coeff, 0, data, 1);
    }
};
template<> struct LeafGroupStart<false> {
    static void run(aie::accum<acc48, 16>& acc,
                    const aie::vector<int16, 16>& coeff,
                    const aie::vector<int16, 32>& data) {
        acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, coeff, 0, data, 1);
    }
};

// cb0/cb1 are 256-bit coefficient registers; dbuf is a 32-lane circular window.
// On entry dbuf's low half contains pw[0..15]. Each load is 16-lane aligned.
// On exit its low half contains pw[32..47], ready for the next 32 coefficients.
template<bool FIRST>
KARA_LEAF_GROUP_ATTR void leaf_group(aie::accum<acc48, 16>& acc,
                       aie::vector<int16, 32>& dbuf,
                       const int16* __restrict pw,
                       const int16* __restrict cw) {
    dbuf.insert(1, aie::load_v<16>(pw + 16));
    const aie::vector<int16, 16> cb0 = aie::load_v<16>(cw);
    const aie::vector<int16, 16> cb1 = aie::load_v<16>(cw + 16);
    LeafGroupStart<FIRST>::run(acc, cb0, dbuf);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0,  2, dbuf,  3);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0,  4, dbuf,  5);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0,  6, dbuf,  7);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0,  8, dbuf,  9);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0, 10, dbuf, 11);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0, 12, dbuf, 13);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb0, 14, dbuf, 15);
    dbuf.insert(0, aie::load_v<16>(pw + 32));
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1,  0, dbuf, 17);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1,  2, dbuf, 19);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1,  4, dbuf, 21);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1,  6, dbuf, 23);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1,  8, dbuf, 25);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1, 10, dbuf, 27);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1, 12, dbuf, 29);
    acc = aie::sliding_mac<16, 2, 1, 1, 1>(acc, cb1, 14, dbuf, 31);
}

#if KARA_STATIC_TRIPS
// One small wrapper per distinct trip count, rather than one MAC body per block.
// Its loop bound is a template constant. Group calls stay out of line so that
// specializing TRIPS does not replicate the 16-instruction MAC group.
template<unsigned TRIPS>
KARA_NOINLINE static aie::accum<acc48, 16> leaf_block_fixed(
        const int16* __restrict data, const int16* __restrict coeff,
        unsigned block) {
    static_assert(TRIPS >= 1u && TRIPS <= L_BITS / 32u, "invalid leaf trip count");
    const unsigned begin = leaf_block_begin(block);
    const int base = int(16u * block) - int(L_BITS) + int(begin);
    const int16* __restrict pw = data + 32 + base;
    const int16* __restrict cw = coeff + begin;
    aie::vector<int16, 32> dbuf;
    dbuf.insert(0, aie::load_v<16>(pw));
    aie::accum<acc48, 16> acc;
    leaf_group<true>(acc, dbuf, pw, cw);
    for (unsigned c = 1; c < TRIPS; ++c)
    chess_prepare_for_pipelining
    chess_loop_range(0, TRIPS)
    {
        leaf_group<false>(acc, dbuf, pw + 32u * c, cw + 32u * c);
    }
    return acc;
}

// Only block geometry selects an instance. No input bits control this dispatch.
template<unsigned TRIPS> struct LeafTripDispatch {
    static aie::accum<acc48, 16> run(
            const int16* __restrict data, const int16* __restrict coeff,
            unsigned block, unsigned trips) {
        if (trips == TRIPS) return leaf_block_fixed<TRIPS>(data, coeff, block);
        return LeafTripDispatch<TRIPS - 1u>::run(data, coeff, block, trips);
    }
};
template<> struct LeafTripDispatch<1u> {
    static aie::accum<acc48, 16> run(
            const int16* __restrict data, const int16* __restrict coeff,
            unsigned block, unsigned) {
        return leaf_block_fixed<1u>(data, coeff, block);
    }
};
KARA_NOINLINE static aie::accum<acc48, 16> leaf_block(
        const int16* __restrict data, const int16* __restrict coeff,
        unsigned block) {
    const unsigned trips = (leaf_block_end(block) - leaf_block_begin(block)) / 32u;
    return LeafTripDispatch<L_BITS / 32u>::run(data, coeff, block, trips);
}
#else
// Optional compact dynamic-trip path. Its MAC group may be inlined into this
// common loop, avoiding the default path's extra per-group calls.
KARA_NOINLINE static aie::accum<acc48, 16> leaf_block(
        const int16* __restrict data, const int16* __restrict coeff,
        unsigned block) {
    const unsigned begin = leaf_block_begin(block);
    const unsigned trips = (leaf_block_end(block) - begin) / 32u;
    const int base = int(16u * block) - int(L_BITS) + int(begin);
    const int16* __restrict pw = data + 32 + base;
    const int16* __restrict cw = coeff + begin;
    aie::vector<int16, 32> dbuf;
    dbuf.insert(0, aie::load_v<16>(pw));
    aie::accum<acc48, 16> acc;
    leaf_group<true>(acc, dbuf, pw, cw);
    for (unsigned c = 1; c < trips; ++c)
    chess_prepare_for_pipelining
    chess_loop_range(0, L_BITS / 32u)
    {
        leaf_group<false>(acc, dbuf, pw + 32u * c, cw + 32u * c);
    }
    return acc;
}
#endif

KARA_LEAF_LINKAGE KARA_NOINLINE void leaf_conv(const uint32_t* __restrict a,
                             const uint32_t* __restrict b,
                             uint32_t* __restrict out) {
    aie::set_saturation(aie::saturation_mode::none);
    aie::set_rounding(aie::rounding_mode::floor);
    static_assert(L_BITS >= 32u && L_BITS % 32u == 0u,
                  "AIE leaf length must be a positive multiple of 32");
    alignas(32) static X86SIM_THREAD_LOCAL int16 data[L_BITS + 64u];
    alignas(32) static X86SIM_THREAD_LOCAL int16 coeff[L_BITS];
#if KARA_LEAF_PROFILE
    uint64_t* const profile = leaf_profile_cycles();
    profile[0] = profile[1] = profile[2] = 0u;
    const uint64_t expand_start = aie::tile::current().cycles();
#endif
    const aie::vector<int16, 32> zeros = aie::zeros<int16, 32>();
    const aie::vector<int16, 32> ones = aie::broadcast<int16, 32>(1);
#if KARA_PARITY_P1
    const aie::vector<int16, 32> coeff_one = ones;
#else
    const aie::vector<int16, 32> coeff_one = aie::broadcast<int16, 32>(-32768);
#endif
    // Every call rewrites every lane, including both padding regions.
    aie::store_v(data, zeros);
    aie::store_v(data + 32u + L_BITS, zeros);
    for (unsigned word = 0; word < LEAF_WORDS; ++word)
    chess_prepare_for_pipelining
    chess_loop_range(1, L_BITS / 32u)
    {
        aie::store_v(data + 32u + 32u * word,
                     aie::select(zeros, ones, aie::mask<32>::from_uint32(a[word])));
        const aie::vector<int16, 32> expanded = aie::select(
            zeros, coeff_one, aie::mask<32>::from_uint32(b[word]));
        aie::store_v(coeff + L_BITS - 32u * (word + 1u), aie::reverse(expanded));
    }
#if KARA_LEAF_PROFILE
    profile[0] = aie::tile::current().cycles() - expand_start;
#endif
    for (unsigned word = 0; word < 2u * LEAF_WORDS; ++word) {
#if KARA_LEAF_PROFILE
        const uint64_t mac_start = aie::tile::current().cycles();
#endif
        const aie::accum<acc48, 16> lo = leaf_block(data, coeff, 2u * word);
        const aie::accum<acc48, 16> hi = leaf_block(data, coeff, 2u * word + 1u);
#if KARA_LEAF_PROFILE
        const uint64_t mac_end = aie::tile::current().cycles();
        profile[1] += mac_end - mac_start;
        const uint64_t pack_start = aie::tile::current().cycles();
#endif
        aie::vector<int16, 32> packed_lanes = aie::concat(
            lo.to_vector<int16>(0), hi.to_vector<int16>(0));
#if KARA_PARITY_P1
        // With none/floor, c - 2*floor(c/2) is 0 or 1, including wrapped c.
        packed_lanes = aie::sub(packed_lanes,
            aie::upshift(aie::downshift(packed_lanes, 1), 1));
        uint32_t result = aie::lt(zeros, packed_lanes).to_uint32();
#else
        uint32_t result = aie::lt(packed_lanes, zeros).to_uint32();
#endif
#if KARA_MASK_BITREV
        result = leaf_reverse_word(result);
#endif
        out[word] = result;
#if KARA_LEAF_PROFILE
        profile[2] += aie::tile::current().cycles() - pack_start;
#endif
    }
}
} // namespace kara
#endif
