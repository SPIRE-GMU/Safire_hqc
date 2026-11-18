#include "../../t1_common.h"

// Template arguments: FRAME_WORDS words of one operand come in; they are padded with zeros to
// POLY_WORDS words and go out last word first.
template<int FRAME_WORDS, int POLY_WORDS>
void k_fin(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[POLY_WORDS];
    t1_read<FRAME_WORDS>(in, poly);
    volatile uint32_t* zero_dst = poly;
    for (int i = FRAME_WORDS; i < POLY_WORDS; i += 1)
        chess_prepare_for_pipelining
        chess_loop_range(POLY_WORDS - FRAME_WORDS, POLY_WORDS - FRAME_WORDS)
    {
        zero_dst[i] = 0;
    }
    for (int i = POLY_WORDS - 1; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(POLY_WORDS, POLY_WORDS)
    {
        writeincr(out, static_cast<int32>(poly[i]));
    }
}
