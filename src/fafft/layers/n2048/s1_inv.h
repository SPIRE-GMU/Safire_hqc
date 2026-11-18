#ifndef T1_LAYERS_S1_INV_H
#define T1_LAYERS_S1_INV_H
#include "../access.h"
#include "../combine.h"
#include "const_s1.h"
#include "gfv_tower.h"
static void t1_inv_s1(uint32_t* data, int words,
        int start_block, t1_circuit circuit) {
    T1_LAYER_ASSERT(words >= 64 && words <= 2048 && words % 64 == 0);
    T1_LAYER_ASSERT(start_block >= 0 && start_block % 2 == 0 &&
                    start_block * 32 + words <= 2048);
    for (int offset = 0; offset < words; offset += 64)
        chess_prepare_for_pipelining
        chess_loop_range(1, 32)
    {
        uint32_t* __restrict x = data + offset;
        uint32_t* __restrict y = data + offset + 32;
        T1_DISJOINT(x, 32, y, 32);
        const int pair = (start_block * 32 + offset) / 64;
        uint32_t product[32];
        uint32_t tower[32];
        T1_DISJOINT(product, 32, y, 32);
        T1_DISJOINT(tower, 32, y, 32);
        t1_inv_first(x, y);
        if (pair < 8) {
            const uint32_t* coeff = t1_s1_gf256_vec + pair * 8;
            T1_DISJOINT(tower, 32, coeff, 8);
            T1_DISJOINT(y, 32, coeff, 8);
            gf232v_mul_gf256(tower, y, coeff);
        } else {
            const uint32_t* coeff = t1_s1_gf216_vec + (pair - 8) * 16;
            T1_DISJOINT(tower, 32, coeff, 16);
            T1_DISJOINT(y, 32, coeff, 16);
            gf232v_mul_gf216(tower, y, coeff);
        }
        circuit(product, y);
        t1_inv_combine_tower(x, y, product, tower);
        for (int j = 0; j < 32; j += 4)
            chess_loop_range(8, 8)
        {
            const uint32_t x0 = x[j + 0];
            const uint32_t x1 = x[j + 1];
            const uint32_t x2 = x[j + 2];
            const uint32_t x3 = x[j + 3];
            const uint32_t y0 = y[j + 0];
            const uint32_t y1 = y[j + 1];
            const uint32_t y2 = y[j + 2];
            const uint32_t y3 = y[j + 3];
            const uint32_t d0 = ((x0 >> 2) ^ y0) & ~0xccccccccu;
            const uint32_t d1 = ((x1 >> 2) ^ y1) & ~0xccccccccu;
            const uint32_t d2 = ((x2 >> 2) ^ y2) & ~0xccccccccu;
            const uint32_t d3 = ((x3 >> 2) ^ y3) & ~0xccccccccu;
            const uint32_t lo0 = x0 ^ (d0 << 2);
            const uint32_t lo1 = x1 ^ (d1 << 2);
            const uint32_t lo2 = x2 ^ (d2 << 2);
            const uint32_t lo3 = x3 ^ (d3 << 2);
            const uint32_t hi0 = y0 ^ d0;
            const uint32_t hi1 = y1 ^ d1;
            const uint32_t hi2 = y2 ^ d2;
            const uint32_t hi3 = y3 ^ d3;
            x[j + 0] = lo0;
            x[j + 1] = lo1;
            x[j + 2] = lo2;
            x[j + 3] = lo3;
            y[j + 0] = hi0;
            y[j + 1] = hi1;
            y[j + 2] = hi2;
            y[j + 3] = hi3;
        }
    }
}
#endif
