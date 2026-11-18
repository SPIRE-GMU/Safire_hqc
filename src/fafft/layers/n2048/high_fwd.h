#ifndef T1_LAYERS_HIGH_FWD_H
#define T1_LAYERS_HIGH_FWD_H
#include "../access.h"
#include "../combine.h"
#include "const_high.h"
#include "gfv_tower.h"
static void t1_fwd_high_pair(uint32_t* __restrict x,
        uint32_t* __restrict y, int block, t1_circuit circuit) {
    T1_DISJOINT(x, 32, y, 32);
    uint32_t product[32];
    uint32_t tower[32];
    T1_DISJOINT(product, 32, y, 32);
    T1_DISJOINT(tower, 32, y, 32);
    if (block == 0) {
        circuit(product, y);
        t1_fwd_combine(x, y, product);
        return;
    }
    if (block == 1) gf232v_mul_0x2(tower, y);
    else if (block == 2) gf232v_mul_0x5(tower, y);
    else if (t1_s0_gft[block] < 16)
        gf232v_mulscalar_gf16(tower, y, (uint8_t)t1_s0_gft[block]);
    else gf232v_mulscalar_gf256(tower, y, (uint8_t)t1_s0_gft[block]);
    circuit(product, y);
    t1_fwd_combine_tower(x, y, product, tower);
}

static T1_LAYER_NOINLINE void t1_fwd_high(uint32_t* data,
        int words, int start_block, int si, t1_circuit circuit) {
    T1_LAYER_ASSERT(si >= 5 && si <= 10);
    const int half = 1 << si;
    const int span = half * 2;
    const int origin = start_block * 32;
    T1_LAYER_ASSERT(words >= span && words <= 2048 && words % span == 0);
    T1_LAYER_ASSERT(origin >= 0 && origin + words <= 2048 && origin % span == 0);
    for (int offset = 0; offset < words; offset += span)
        chess_prepare_for_pipelining
        chess_loop_range(1, 32)
    {
        const int block = (origin + offset) >> (si + 1);
        for (int i = 0; i < half; i += 32)
            chess_prepare_for_pipelining
            chess_loop_range(1, 32)
        {
            t1_fwd_high_pair(data + offset + i, data + offset + half + i,
                                    block, circuit);
        }
    }
}
#endif
