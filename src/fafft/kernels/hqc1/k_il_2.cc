#include "../../t1_common.h"

namespace {
#include "../../layers/scalar_v24.h"
#include "../../layers/scalar_v23.h"
#include "../../layers/n2048/s3_inv.h"
#include "../../layers/n2048/s4_inv.h"
}

// Template argument: BLOCK is the block position of the tile, passed on to the layer routines.
template<int BLOCK>
void k_il_2(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[128];
    for (int frame = 0; frame < 1; ++frame) {
        t1_read<128>(in, poly);
        t1_inv_s3(poly, 128, BLOCK, t1_scalar_v24);
        t1_inv_s4(poly, 128, BLOCK, t1_scalar_v23);
        t1_write<128>(out, poly);
    }
}
