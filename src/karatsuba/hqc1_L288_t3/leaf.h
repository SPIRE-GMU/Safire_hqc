// Leaf ABI: little-endian packed GF(2) input/output, full linear product.
#ifndef KARA_LEAF_H
#define KARA_LEAF_H
#include "config.h"
namespace kara {
// a and b: L_BITS/32 words; out: 2*L_BITS/32 words. No aliasing.
void leaf_conv(const uint32_t* __restrict a, const uint32_t* __restrict b,
               uint32_t* __restrict out);
} // namespace kara
#endif
