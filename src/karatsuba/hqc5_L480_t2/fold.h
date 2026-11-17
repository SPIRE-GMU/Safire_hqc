// Fold only the valid 2*n-1 bits; all frame padding is zero.
#ifndef KARA_FOLD_H
#define KARA_FOLD_H
#include "config.h"
namespace kara {
inline void fold(const uint32_t* __restrict F,
                 uint32_t* __restrict out) {
    for (uint32_t i = 0; i < 1801u; ++i)
    KARA_PIPELINE
    { out[i] = F[i] ^ ((F[i + 1801u] >> 5u) | (F[i + 1802u] << 27u)); }
    out[1801u] = (F[1801u] & 0x0000001fu)
        ^ (((F[3602u] >> 5u) | (F[3603u] << 27u)) & 0x0000000fu);
}
} // namespace kara
#endif
