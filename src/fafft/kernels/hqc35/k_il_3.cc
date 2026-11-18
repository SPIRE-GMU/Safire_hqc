#include "../../t1_common.h"

namespace {
#include "../../layers/scalar_v23.h"
#include "../../layers/scalar_v22.h"
#include "../../layers/scalar_v21.h"
#include "../../layers/n4096/high_inv.h"
}

// Template argument: BLOCK is the block position of the tile, passed on to the layer routines.
template<int BLOCK>
void k_il_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t poly[256];
    for (int frame = 0; frame < 1; ++frame) {
        t1_read<256>(in, poly);
        t1_inv_high(poly, 256, BLOCK, 5, t1_scalar_v23);
        t1_inv_high(poly, 256, BLOCK, 6, t1_scalar_v22);
        t1_inv_high(poly, 256, BLOCK, 7, t1_scalar_v21);
        t1_write<256>(out, poly);
    }
}
