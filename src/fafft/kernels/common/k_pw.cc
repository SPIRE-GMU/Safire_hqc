#include "../../t1_common.h"
namespace {
#include "gfv_tower.h"

static inline void t1_pw_group(uint32_t* __restrict product,
                                const uint32_t* __restrict a,
                                const uint32_t* __restrict b) {
    t1_assert_disjoint(product, 32 * sizeof(uint32_t), a, 32 * sizeof(uint32_t));
    t1_assert_disjoint(product, 32 * sizeof(uint32_t), b, 32 * sizeof(uint32_t));
    t1_assert_disjoint(a, 32 * sizeof(uint32_t), b, 32 * sizeof(uint32_t));
    gf232v_mul(product, a, b);
}
}

// Template argument: BLOCKS is the number of 32-word groups multiplied per call.
template<int BLOCKS>
void k_pw(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in0, sizeof(*in0), in1, sizeof(*in1));
    t1_assert_disjoint(in0, sizeof(*in0), out, sizeof(*out));
    t1_assert_disjoint(in1, sizeof(*in1), out, sizeof(*out));
#endif
    for (int block = 0; block < BLOCKS; ++block)
        chess_loop_range(BLOCKS, BLOCKS)
    {
        uint32_t a[32];
        uint32_t b[32];
        uint32_t product[32];
        t1_read<32>(in0, a);
        t1_read<32>(in1, b);
        t1_pw_group(product, a, b);
        t1_write<32>(out, product);
    }
}
