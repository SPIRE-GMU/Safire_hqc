#include "../../t1_common.h"

// Template argument: WORDS is the number of words taken from each input in turn.
template<int WORDS>
void k_join(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in0, sizeof(*in0), in1, sizeof(*in1));
    t1_assert_disjoint(in0, sizeof(*in0), out, sizeof(*out));
    t1_assert_disjoint(in1, sizeof(*in1), out, sizeof(*out));
#endif
    for (int j = 0; j < 32; j += 1)
        chess_prepare_for_pipelining
        chess_loop_range(32, 32)
    {
        for (int i = 0; i < WORDS; i += 1)
            chess_prepare_for_pipelining
            chess_loop_range(WORDS, WORDS)
        {
            writeincr(out, readincr(in0));
        }
        for (int i = 0; i < WORDS; i += 1)
            chess_prepare_for_pipelining
            chess_loop_range(WORDS, WORDS)
        {
            writeincr(out, readincr(in1));
        }
    }
}
