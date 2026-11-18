#include "../../t1_common.h"
namespace {
#include "../../layers/scalar_v17.h"
#include "../../layers/n4096/high_inv.h"
}

void k_xi_11(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in0, sizeof(*in0), in1, sizeof(*in1));
    t1_assert_disjoint(in0, sizeof(*in0), out0, sizeof(*out0));
    t1_assert_disjoint(in0, sizeof(*in0), out1, sizeof(*out1));
    t1_assert_disjoint(in1, sizeof(*in1), out0, sizeof(*out0));
    t1_assert_disjoint(in1, sizeof(*in1), out1, sizeof(*out1));
    t1_assert_disjoint(out0, sizeof(*out0), out1, sizeof(*out1));
#endif
    for (int g = 0; g < 8; g += 1)
        chess_prepare_for_pipelining
        chess_loop_range(8, 8)
    {
        uint32_t x[32];
        uint32_t y[32];
        t1_read<32>(in0, x);
        t1_read<32>(in1, y);
        t1_inv_high_pair(x, y, 0, t1_scalar_v17);
        t1_write<32>(out0, x);
        t1_write<32>(out1, y);
    }
}
