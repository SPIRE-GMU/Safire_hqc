#include "../../t1_common.h"

namespace {
#include "../../layers/scalar_v26.h"
#include "../../layers/scalar_v27.h"
#include "../../layers/scalar_v28.h"
#include "../../layers/n4096/s2_fwd.h"
#include "../../layers/n4096/s1_fwd.h"
#include "../../layers/n4096/s0_fwd.h"
}

// Template argument: BLOCK is the block position of the tile, passed on to the layer routines.
template<int BLOCK>
void k_fl_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[256];
    t1_read<256>(in, poly);
    t1_fwd_s2(poly, 256, BLOCK, t1_scalar_v26);
    t1_fwd_s1(poly, 256, BLOCK, t1_scalar_v27);
    t1_fwd_s0(poly, 256, BLOCK, t1_scalar_v28);
    t1_write<256>(out, poly);
}
