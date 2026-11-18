#include "../../t1_common.h"

void k_bs16(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t hi[2048];
    // Store original high words; read the buffer only after this loop.
    for (int k = 2047; k > 0; k += -1)
        chess_prepare_for_pipelining
        chess_loop_range(2047, 2047)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        hi[k] = value;
        writeincr(out, static_cast<int32>(value));
    }
    const uint32_t high_zero = static_cast<uint32_t>(readincr(in));
    hi[0] = high_zero;
    writeincr(out, static_cast<int32>(high_zero ^ (hi[2047] >> 31)));
    for (int k = 2047; k > 0; k += -1)
        chess_prepare_for_pipelining
        chess_loop_range(2047, 2047)
    {
        const uint32_t value = static_cast<uint32_t>(readincr(in));
        const uint32_t result = value ^ (hi[k] << 1) ^ (hi[k - 1] >> 31);
        writeincr(out, static_cast<int32>(result));
    }
    const uint32_t low_zero = static_cast<uint32_t>(readincr(in));
    writeincr(out, static_cast<int32>(low_zero ^ (hi[0] << 1)));
}
