#include "../../t1_common.h"

namespace {
// Each inner loop only writes or only reads each buffer. The block
// helper is noinline so the outer loop does not fuse buffer lifetimes.
template<int S, int O>
static __attribute__((noinline)) void fp_block(
        input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
    static X86SIM_THREAD_LOCAL uint32_t source_only[S];
    static X86SIM_THREAD_LOCAL uint32_t overlap[O];
    t1_assert_disjoint(source_only, sizeof(source_only), overlap, sizeof(overlap));
    for (int i = S - 1; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(S, S)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        source_only[i] = value;
        writeincr(out, static_cast<int32>(value));
    }
    for (int i = O - 1; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(O, O)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        const uint32_t result = value ^ source_only[i + S - O];
        overlap[i] = result;
        writeincr(out, static_cast<int32>(result));
    }
    for (int i = S + O - 1; i > 2 * O - 1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(S - O, S - O)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        writeincr(out, static_cast<int32>(value ^ source_only[i - 2 * O]));
    }
    for (int i = O - 1; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(O, O)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        writeincr(out, static_cast<int32>(value ^ overlap[i]));
    }
    for (int i = 0; i < O; i += 1)
        chess_prepare_for_pipelining
        chess_loop_range(O, O)
    {
        writeincr(out, readincr(in));
    }
}
}

// Template arguments: S and O are the sizes of the buffers source_only and overlap in words,
// BLOCKS is the number of blocks per call.
template<int S, int O, int BLOCKS>
void k_fp(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    for (int block = 0; block < BLOCKS; block += 1)
        chess_prepare_for_pipelining
        chess_loop_range(BLOCKS, BLOCKS)
    {
        fp_block<S, O>(in, out);
    }
}
