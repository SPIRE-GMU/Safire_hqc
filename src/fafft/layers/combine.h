#ifndef T1_LAYERS_COMBINE_H
#define T1_LAYERS_COMBINE_H
#include "access.h"
static void t1_fwd_combine(uint32_t* __restrict x, uint32_t* __restrict y, const uint32_t* __restrict product) {
    T1_DISJOINT(x, 32, y, 32);
    T1_DISJOINT(x, 32, product, 32);
    T1_DISJOINT(y, 32, product, 32);
    for (int j = 0; j < 32; j += 4)
        chess_loop_range(8, 8)
    {
        const uint32_t p0 = product[j + 0];
        const uint32_t p1 = product[j + 1];
        const uint32_t p2 = product[j + 2];
        const uint32_t p3 = product[j + 3];
        const uint32_t x0 = x[j + 0];
        const uint32_t x1 = x[j + 1];
        const uint32_t x2 = x[j + 2];
        const uint32_t x3 = x[j + 3];
        const uint32_t y0 = y[j + 0];
        const uint32_t y1 = y[j + 1];
        const uint32_t y2 = y[j + 2];
        const uint32_t y3 = y[j + 3];
        const uint32_t nx0 = x0 ^ p0;
        const uint32_t nx1 = x1 ^ p1;
        const uint32_t nx2 = x2 ^ p2;
        const uint32_t nx3 = x3 ^ p3;
        const uint32_t ny0 = y0 ^ nx0;
        const uint32_t ny1 = y1 ^ nx1;
        const uint32_t ny2 = y2 ^ nx2;
        const uint32_t ny3 = y3 ^ nx3;
        x[j + 0] = nx0;
        x[j + 1] = nx1;
        x[j + 2] = nx2;
        x[j + 3] = nx3;
        y[j + 0] = ny0;
        y[j + 1] = ny1;
        y[j + 2] = ny2;
        y[j + 3] = ny3;
    }
}
static void t1_fwd_combine_tower(uint32_t* __restrict x, uint32_t* __restrict y, const uint32_t* __restrict product, const uint32_t* __restrict tower) {
    T1_DISJOINT(x, 32, y, 32);
    T1_DISJOINT(x, 32, product, 32);
    T1_DISJOINT(x, 32, tower, 32);
    T1_DISJOINT(y, 32, product, 32);
    T1_DISJOINT(y, 32, tower, 32);
    T1_DISJOINT(product, 32, tower, 32);
    for (int j = 0; j < 32; j += 4)
        chess_loop_range(8, 8)
    {
        const uint32_t p0 = product[j + 0];
        const uint32_t p1 = product[j + 1];
        const uint32_t p2 = product[j + 2];
        const uint32_t p3 = product[j + 3];
        const uint32_t t0 = tower[j + 0];
        const uint32_t t1 = tower[j + 1];
        const uint32_t t2 = tower[j + 2];
        const uint32_t t3 = tower[j + 3];
        const uint32_t x0 = x[j + 0];
        const uint32_t x1 = x[j + 1];
        const uint32_t x2 = x[j + 2];
        const uint32_t x3 = x[j + 3];
        const uint32_t y0 = y[j + 0];
        const uint32_t y1 = y[j + 1];
        const uint32_t y2 = y[j + 2];
        const uint32_t y3 = y[j + 3];
        const uint32_t nx0 = x0 ^ t0 ^ p0;
        const uint32_t nx1 = x1 ^ t1 ^ p1;
        const uint32_t nx2 = x2 ^ t2 ^ p2;
        const uint32_t nx3 = x3 ^ t3 ^ p3;
        const uint32_t ny0 = y0 ^ nx0;
        const uint32_t ny1 = y1 ^ nx1;
        const uint32_t ny2 = y2 ^ nx2;
        const uint32_t ny3 = y3 ^ nx3;
        x[j + 0] = nx0;
        x[j + 1] = nx1;
        x[j + 2] = nx2;
        x[j + 3] = nx3;
        y[j + 0] = ny0;
        y[j + 1] = ny1;
        y[j + 2] = ny2;
        y[j + 3] = ny3;
    }
}
static void t1_inv_combine(uint32_t* __restrict x, uint32_t* __restrict y, const uint32_t* __restrict product) {
    T1_DISJOINT(x, 32, y, 32);
    T1_DISJOINT(x, 32, product, 32);
    T1_DISJOINT(y, 32, product, 32);
    (void)y;
    for (int j = 0; j < 32; j += 4)
        chess_loop_range(8, 8)
    {
        const uint32_t p0 = product[j + 0];
        const uint32_t p1 = product[j + 1];
        const uint32_t p2 = product[j + 2];
        const uint32_t p3 = product[j + 3];
        const uint32_t x0 = x[j + 0];
        const uint32_t x1 = x[j + 1];
        const uint32_t x2 = x[j + 2];
        const uint32_t x3 = x[j + 3];
        const uint32_t nx0 = x0 ^ p0;
        const uint32_t nx1 = x1 ^ p1;
        const uint32_t nx2 = x2 ^ p2;
        const uint32_t nx3 = x3 ^ p3;
        x[j + 0] = nx0;
        x[j + 1] = nx1;
        x[j + 2] = nx2;
        x[j + 3] = nx3;
    }
}
static void t1_inv_combine_tower(uint32_t* __restrict x, uint32_t* __restrict y, const uint32_t* __restrict product, const uint32_t* __restrict tower) {
    T1_DISJOINT(x, 32, y, 32);
    T1_DISJOINT(x, 32, product, 32);
    T1_DISJOINT(x, 32, tower, 32);
    T1_DISJOINT(y, 32, product, 32);
    T1_DISJOINT(y, 32, tower, 32);
    T1_DISJOINT(product, 32, tower, 32);
    (void)y;
    for (int j = 0; j < 32; j += 4)
        chess_loop_range(8, 8)
    {
        const uint32_t p0 = product[j + 0];
        const uint32_t p1 = product[j + 1];
        const uint32_t p2 = product[j + 2];
        const uint32_t p3 = product[j + 3];
        const uint32_t t0 = tower[j + 0];
        const uint32_t t1 = tower[j + 1];
        const uint32_t t2 = tower[j + 2];
        const uint32_t t3 = tower[j + 3];
        const uint32_t x0 = x[j + 0];
        const uint32_t x1 = x[j + 1];
        const uint32_t x2 = x[j + 2];
        const uint32_t x3 = x[j + 3];
        const uint32_t nx0 = x0 ^ t0 ^ p0;
        const uint32_t nx1 = x1 ^ t1 ^ p1;
        const uint32_t nx2 = x2 ^ t2 ^ p2;
        const uint32_t nx3 = x3 ^ t3 ^ p3;
        x[j + 0] = nx0;
        x[j + 1] = nx1;
        x[j + 2] = nx2;
        x[j + 3] = nx3;
    }
}
static void t1_inv_first(uint32_t* __restrict x, uint32_t* __restrict y) {
    T1_DISJOINT(x, 32, y, 32);
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
        const uint32_t ny0 = y0 ^ x0;
        const uint32_t ny1 = y1 ^ x1;
        const uint32_t ny2 = y2 ^ x2;
        const uint32_t ny3 = y3 ^ x3;
        y[j + 0] = ny0;
        y[j + 1] = ny1;
        y[j + 2] = ny2;
        y[j + 3] = ny3;
    }
}
#endif
