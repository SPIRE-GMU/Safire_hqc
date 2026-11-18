#include "../../t1_common.h"

namespace {
#include "../../layers/scalar_v21.h"
#include "../../layers/scalar_v22.h"
#include "../../layers/n2048/high_fwd.h"
}

// Template argument: BLOCK is the block position of the tile, passed on to the layer routines.
template<int BLOCK>
void k_fl_1(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[128];
    t1_read<128>(in, poly);
    t1_fwd_high(poly, 128, BLOCK, 6, t1_scalar_v21);
    t1_fwd_high(poly, 128, BLOCK, 5, t1_scalar_v22);
    t1_write<128>(out, poly);
}
