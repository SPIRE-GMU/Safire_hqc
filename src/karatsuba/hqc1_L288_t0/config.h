// Configuration constants; all metadata is compile-time.
#ifndef KARA_CONFIG_H
#define KARA_CONFIG_H
#include <stdint.h>
#ifndef KARA_PIPELINE
#define KARA_PIPELINE
#endif
namespace kara {
static const uint32_t N_BITS = 17669u;
static const uint32_t FRAME_WORDS = 554u;
static const uint32_t L_BITS = 288u;
static const uint32_t LEVELS = 6u;
static const uint32_t PAD_BITS = 18432u;
static const uint32_t SPLIT = 0u;
static const uint32_t G = 1u;
static const uint32_t SUB_WORDS = 576u;
static const uint32_t PRODUCT_WORDS = 1152u;
static const uint32_t F_WORDS = 1152u;
static const uint32_t SCRATCH_WORDS = 2268u;
static const uint32_t LEAF_WORDS = 9u;
static const uint32_t CONTRIBUTION_WORDS = 0u;
static const uint32_t LOCAL_LEVELS = 6u;
static const uint32_t INPUT_SEGMENTS = 1u;
static const uint32_t WORK_TILE_BYTES = 20504u;
static const uint32_t FOLD_TILE_BYTES = 0u;
static const uint32_t AIE_WORK_TILE_BYTES = 25880u;
static const uint32_t AIE_FOLD_TILE_BYTES = 0u;
template<uint32_t TILE> struct TileMeta;
template<uint32_t TILE, uint32_t INDEX> struct TileOffset;
// Tile 0: a_segments = b_segments = {0}.
template<> struct TileMeta<0u> {
    static const uint32_t SEGMENT_MASK = 0x00000001u;
    static const uint32_t OFFSET_COUNT = 1u;
};
template<> struct TileOffset<0u, 0u> {
    static const uint32_t WORD = 0u;
};
} // namespace kara
#endif
