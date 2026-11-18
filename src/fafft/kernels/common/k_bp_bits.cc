#include "../../t1_common.h"

namespace {
// Each inner loop only writes or only reads each buffer. The block
// helper is noinline so the outer loop does not fuse buffer lifetimes.
template<int S, int O>
static __attribute__((noinline)) void bp_bits_block(
        input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
    static X86SIM_THREAD_LOCAL uint32_t source_only[S];
    for (int i = S - 1; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(S, S)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        source_only[i] = value;
        writeincr(out, static_cast<int32>(value));
    }
    const uint32_t value_h = static_cast<uint32_t>(readincr(in));
    const uint32_t result_h = value_h ^ (source_only[S - 1] >> (32 - O));
    const uint32_t overlap = value_h;
    writeincr(out, static_cast<int32>(result_h));
    for (int i = S; i > 1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(S - 1, S - 1)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        const uint32_t result = value ^ (source_only[i - 2] >> (32 - O)) ^ (source_only[i - 1] << O);
        writeincr(out, static_cast<int32>(result));
    }
    const uint32_t value_1 = static_cast<uint32_t>(readincr(in));
    writeincr(out, static_cast<int32>(value_1 ^ (overlap >> (32 - O)) ^ (source_only[0] << O)));
    const uint32_t value_0 = static_cast<uint32_t>(readincr(in));
    writeincr(out, static_cast<int32>(value_0 ^ (overlap << O)));
}
}

// Template arguments: S is the size of the buffer source_only in words, the words are combined
// with shifts by O and 32 - O bits, BLOCKS is the number of blocks per call.
template<int S, int O, int BLOCKS>
void k_bp_bits(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    for (int block = 0; block < BLOCKS; block += 1)
        chess_prepare_for_pipelining
        chess_loop_range(BLOCKS, BLOCKS)
    {
        bp_bits_block<S, O>(in, out);
    }
}
