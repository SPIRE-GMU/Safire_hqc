#include "../../t1_common.h"
#include "parameters.h"
static_assert(N == 35851 && VEC_N_SIZE_32 == 1121, "HQC-3 reduction");

void k_bout_hqc3(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[4096];
    for (int i = 4095; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(4096, 4096)
    {
        poly[i] = static_cast<uint32_t>(readincr(in));
    }
    // Same expression and mask as the upstream reduce.c; emit in ascending order.
    for (int i = 0; i < 1120; i += 1)
        chess_prepare_for_pipelining
        chess_loop_range(1120, 1120)
    {
        const uint32_t r = poly[i + VEC_N_SIZE_32 - 1] >> (N & 0x1F);
        const uint32_t carry = poly[i + VEC_N_SIZE_32] << (32 - (N & 0x1F));
        writeincr(out, static_cast<int32>(poly[i] ^ r ^ carry));
    }
    const uint32_t last = poly[1120] ^ (poly[2240] >> (N & 0x1F))
                          ^ (poly[2241] << (32 - (N & 0x1F)));
    writeincr(out, static_cast<int32>(last & ((1u << (N & 0x1F)) - 1u)));
    writeincr(out, 0);
}
