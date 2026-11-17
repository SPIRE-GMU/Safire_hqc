// Fold only the valid 2*n-1 bits; all frame padding is zero.
#ifndef KARA_FOLD_H
#define KARA_FOLD_H
#include "config.h"
namespace kara {
inline void fold(const uint32_t* __restrict F,
                 uint32_t* __restrict out) {
    for (uint32_t i = 0; i < 552u; ++i)
    KARA_PIPELINE
    { out[i] = F[i] ^ ((F[i + 552u] >> 5u) | (F[i + 553u] << 27u)); }
    out[552u] = (F[552u] & 0x0000001fu)
        ^ (((F[1104u] >> 5u) | (F[1105u] << 27u)) & 0x0000000fu);
    for (uint32_t i = 553u; i < FRAME_WORDS; ++i)
    KARA_PIPELINE
    { out[i] = 0u; }
}
} // namespace kara
#endif
