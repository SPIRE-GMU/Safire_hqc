// Fold only the valid 2*n-1 bits; all frame padding is zero.
#ifndef KARA_FOLD_H
#define KARA_FOLD_H
#include "config.h"
namespace kara {
inline void fold(const uint32_t* __restrict F,
                 uint32_t* __restrict out) {
    for (uint32_t i = 0; i < 1120u; ++i)
    KARA_PIPELINE
    { out[i] = F[i] ^ ((F[i + 1120u] >> 11u) | (F[i + 1121u] << 21u)); }
    out[1120u] = (F[1120u] & 0x000007ffu)
        ^ (((F[2240u] >> 11u) | (F[2241u] << 21u)) & 0x000003ffu);
    for (uint32_t i = 1121u; i < FRAME_WORDS; ++i)
    KARA_PIPELINE
    { out[i] = 0u; }
}
} // namespace kara
#endif
