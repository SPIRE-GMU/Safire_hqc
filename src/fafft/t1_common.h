#ifndef T1_COMMON_H
#define T1_COMMON_H
// ADF and system headers must precede repository parameters.h.
#include <adf.h>
#include <aie_api/aie.hpp>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
// Kernel declarations are supplied separately for each tree configuration.

static_assert(sizeof(int32) == sizeof(uint32_t), "32-bit stream words required");
static inline void t1_assert_disjoint(const void* p, size_t np,
                                      const void* q, size_t nq) {
#ifdef T1_HOST_CHECK
    const uintptr_t a = reinterpret_cast<uintptr_t>(p);
    const uintptr_t b = reinterpret_cast<uintptr_t>(q);
    assert(a <= UINTPTR_MAX - np && b <= UINTPTR_MAX - nq);
    assert(a + np <= b || b + nq <= a);
#else
    (void)p; (void)np; (void)q; (void)nq;
#endif
}

template<int Words>
static inline void t1_read(input_stream_int32* __restrict in,
                           uint32_t* __restrict words) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), words, Words * sizeof(uint32_t));
#endif
    for (int i = 0; i < Words; ++i)
        chess_prepare_for_pipelining
        chess_loop_range(Words, Words)
    {
        words[i] = static_cast<uint32_t>(readincr(in));
    }
}

template<int Words>
static inline void t1_write(output_stream_int32* __restrict out,
                            const uint32_t* __restrict words) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(out, sizeof(*out), words, Words * sizeof(uint32_t));
#endif
    for (int i = 0; i < Words; ++i)
        chess_prepare_for_pipelining
        chess_loop_range(Words, Words)
    {
        writeincr(out, static_cast<int32>(words[i]));
    }
}
#endif
