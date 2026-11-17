// Configuration constants; all metadata is compile-time.
#ifndef KARA_CONFIG_H
#define KARA_CONFIG_H
#include <stdint.h>
#ifndef KARA_PIPELINE
#define KARA_PIPELINE
#endif
namespace kara {
static const uint32_t N_BITS = 57637u;
static const uint32_t FRAME_WORDS = 1802u;
static const uint32_t L_BITS = 480u;
static const uint32_t LEVELS = 7u;
static const uint32_t PAD_BITS = 61440u;
static const uint32_t SPLIT = 5u;
static const uint32_t G = 243u;
static const uint32_t SUB_WORDS = 60u;
static const uint32_t PRODUCT_WORDS = 120u;
static const uint32_t F_WORDS = 3840u;
static const uint32_t SCRATCH_WORDS = 180u;
static const uint32_t LEAF_WORDS = 15u;
static const uint32_t CONTRIBUTION_WORDS = 0u;
static const uint32_t LOCAL_LEVELS = 2u;
static const uint32_t INPUT_SEGMENTS = 31u;
static const uint32_t WORK_TILE_BYTES = 1680u;
static const uint32_t FOLD_TILE_BYTES = 22568u;
static const uint32_t AIE_WORK_TILE_BYTES = 7824u;
static const uint32_t AIE_FOLD_TILE_BYTES = 26664u;
template<uint32_t TILE> struct TileMeta;
template<uint32_t TILE, uint32_t INDEX> struct TileOffset;
// Tile 0: a_segments = b_segments = {0}.
template<> struct TileMeta<0u> {
    static const uint32_t SEGMENT_MASK = 0x00000001u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<0u, 0u> {
    static const uint32_t WORD = 0u;
};
template<> struct TileOffset<0u, 1u> {
    static const uint32_t WORD = 60u;
};
template<> struct TileOffset<0u, 2u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<0u, 3u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<0u, 4u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<0u, 5u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<0u, 6u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<0u, 7u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<0u, 8u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<0u, 9u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<0u, 10u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<0u, 11u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<0u, 12u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<0u, 13u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<0u, 14u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<0u, 15u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<0u, 16u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<0u, 17u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<0u, 18u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<0u, 19u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<0u, 20u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<0u, 21u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<0u, 22u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<0u, 23u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<0u, 24u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<0u, 25u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<0u, 26u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<0u, 27u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<0u, 28u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<0u, 29u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<0u, 30u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<0u, 31u> {
    static const uint32_t WORD = 1860u;
};
// Tile 1: a_segments = b_segments = {1}.
template<> struct TileMeta<1u> {
    static const uint32_t SEGMENT_MASK = 0x00000002u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<1u, 0u> {
    static const uint32_t WORD = 60u;
};
template<> struct TileOffset<1u, 1u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<1u, 2u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<1u, 3u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<1u, 4u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<1u, 5u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<1u, 6u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<1u, 7u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<1u, 8u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<1u, 9u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<1u, 10u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<1u, 11u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<1u, 12u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<1u, 13u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<1u, 14u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<1u, 15u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<1u, 16u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<1u, 17u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<1u, 18u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<1u, 19u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<1u, 20u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<1u, 21u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<1u, 22u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<1u, 23u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<1u, 24u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<1u, 25u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<1u, 26u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<1u, 27u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<1u, 28u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<1u, 29u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<1u, 30u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<1u, 31u> {
    static const uint32_t WORD = 1920u;
};
// Tile 2: a_segments = b_segments = {0,1}.
template<> struct TileMeta<2u> {
    static const uint32_t SEGMENT_MASK = 0x00000003u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<2u, 0u> {
    static const uint32_t WORD = 60u;
};
template<> struct TileOffset<2u, 1u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<2u, 2u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<2u, 3u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<2u, 4u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<2u, 5u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<2u, 6u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<2u, 7u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<2u, 8u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<2u, 9u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<2u, 10u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<2u, 11u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<2u, 12u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<2u, 13u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<2u, 14u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<2u, 15u> {
    static const uint32_t WORD = 1860u;
};
// Tile 3: a_segments = b_segments = {2}.
template<> struct TileMeta<3u> {
    static const uint32_t SEGMENT_MASK = 0x00000004u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<3u, 0u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<3u, 1u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<3u, 2u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<3u, 3u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<3u, 4u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<3u, 5u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<3u, 6u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<3u, 7u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<3u, 8u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<3u, 9u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<3u, 10u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<3u, 11u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<3u, 12u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<3u, 13u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<3u, 14u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<3u, 15u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<3u, 16u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<3u, 17u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<3u, 18u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<3u, 19u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<3u, 20u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<3u, 21u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<3u, 22u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<3u, 23u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<3u, 24u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<3u, 25u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<3u, 26u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<3u, 27u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<3u, 28u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<3u, 29u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<3u, 30u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<3u, 31u> {
    static const uint32_t WORD = 1980u;
};
// Tile 4: a_segments = b_segments = {3}.
template<> struct TileMeta<4u> {
    static const uint32_t SEGMENT_MASK = 0x00000008u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<4u, 0u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<4u, 1u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<4u, 2u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<4u, 3u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<4u, 4u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<4u, 5u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<4u, 6u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<4u, 7u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<4u, 8u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<4u, 9u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<4u, 10u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<4u, 11u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<4u, 12u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<4u, 13u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<4u, 14u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<4u, 15u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<4u, 16u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<4u, 17u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<4u, 18u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<4u, 19u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<4u, 20u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<4u, 21u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<4u, 22u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<4u, 23u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<4u, 24u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<4u, 25u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<4u, 26u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<4u, 27u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<4u, 28u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<4u, 29u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<4u, 30u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<4u, 31u> {
    static const uint32_t WORD = 2040u;
};
// Tile 5: a_segments = b_segments = {2,3}.
template<> struct TileMeta<5u> {
    static const uint32_t SEGMENT_MASK = 0x0000000cu;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<5u, 0u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<5u, 1u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<5u, 2u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<5u, 3u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<5u, 4u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<5u, 5u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<5u, 6u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<5u, 7u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<5u, 8u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<5u, 9u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<5u, 10u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<5u, 11u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<5u, 12u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<5u, 13u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<5u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<5u, 15u> {
    static const uint32_t WORD = 1980u;
};
// Tile 6: a_segments = b_segments = {0,2}.
template<> struct TileMeta<6u> {
    static const uint32_t SEGMENT_MASK = 0x00000005u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<6u, 0u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<6u, 1u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<6u, 2u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<6u, 3u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<6u, 4u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<6u, 5u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<6u, 6u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<6u, 7u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<6u, 8u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<6u, 9u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<6u, 10u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<6u, 11u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<6u, 12u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<6u, 13u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<6u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<6u, 15u> {
    static const uint32_t WORD = 1860u;
};
// Tile 7: a_segments = b_segments = {1,3}.
template<> struct TileMeta<7u> {
    static const uint32_t SEGMENT_MASK = 0x0000000au;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<7u, 0u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<7u, 1u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<7u, 2u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<7u, 3u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<7u, 4u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<7u, 5u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<7u, 6u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<7u, 7u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<7u, 8u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<7u, 9u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<7u, 10u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<7u, 11u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<7u, 12u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<7u, 13u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<7u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<7u, 15u> {
    static const uint32_t WORD = 1920u;
};
// Tile 8: a_segments = b_segments = {0,1,2,3}.
template<> struct TileMeta<8u> {
    static const uint32_t SEGMENT_MASK = 0x0000000fu;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<8u, 0u> {
    static const uint32_t WORD = 180u;
};
template<> struct TileOffset<8u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<8u, 2u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<8u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<8u, 4u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<8u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<8u, 6u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<8u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 9: a_segments = b_segments = {4}.
template<> struct TileMeta<9u> {
    static const uint32_t SEGMENT_MASK = 0x00000010u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<9u, 0u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<9u, 1u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<9u, 2u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<9u, 3u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<9u, 4u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<9u, 5u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<9u, 6u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<9u, 7u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<9u, 8u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<9u, 9u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<9u, 10u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<9u, 11u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<9u, 12u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<9u, 13u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<9u, 14u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<9u, 15u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<9u, 16u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<9u, 17u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<9u, 18u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<9u, 19u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<9u, 20u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<9u, 21u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<9u, 22u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<9u, 23u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<9u, 24u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<9u, 25u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<9u, 26u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<9u, 27u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<9u, 28u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<9u, 29u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<9u, 30u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<9u, 31u> {
    static const uint32_t WORD = 2100u;
};
// Tile 10: a_segments = b_segments = {5}.
template<> struct TileMeta<10u> {
    static const uint32_t SEGMENT_MASK = 0x00000020u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<10u, 0u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<10u, 1u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<10u, 2u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<10u, 3u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<10u, 4u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<10u, 5u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<10u, 6u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<10u, 7u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<10u, 8u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<10u, 9u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<10u, 10u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<10u, 11u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<10u, 12u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<10u, 13u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<10u, 14u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<10u, 15u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<10u, 16u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<10u, 17u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<10u, 18u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<10u, 19u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<10u, 20u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<10u, 21u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<10u, 22u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<10u, 23u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<10u, 24u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<10u, 25u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<10u, 26u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<10u, 27u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<10u, 28u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<10u, 29u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<10u, 30u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<10u, 31u> {
    static const uint32_t WORD = 2160u;
};
// Tile 11: a_segments = b_segments = {4,5}.
template<> struct TileMeta<11u> {
    static const uint32_t SEGMENT_MASK = 0x00000030u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<11u, 0u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<11u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<11u, 2u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<11u, 3u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<11u, 4u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<11u, 5u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<11u, 6u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<11u, 7u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<11u, 8u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<11u, 9u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<11u, 10u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<11u, 11u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<11u, 12u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<11u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<11u, 14u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<11u, 15u> {
    static const uint32_t WORD = 2100u;
};
// Tile 12: a_segments = b_segments = {6}.
template<> struct TileMeta<12u> {
    static const uint32_t SEGMENT_MASK = 0x00000040u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<12u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<12u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<12u, 2u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<12u, 3u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<12u, 4u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<12u, 5u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<12u, 6u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<12u, 7u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<12u, 8u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<12u, 9u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<12u, 10u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<12u, 11u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<12u, 12u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<12u, 13u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<12u, 14u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<12u, 15u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<12u, 16u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<12u, 17u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<12u, 18u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<12u, 19u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<12u, 20u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<12u, 21u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<12u, 22u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<12u, 23u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<12u, 24u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<12u, 25u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<12u, 26u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<12u, 27u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<12u, 28u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<12u, 29u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<12u, 30u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<12u, 31u> {
    static const uint32_t WORD = 2220u;
};
// Tile 13: a_segments = b_segments = {7}.
template<> struct TileMeta<13u> {
    static const uint32_t SEGMENT_MASK = 0x00000080u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<13u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<13u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<13u, 2u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<13u, 3u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<13u, 4u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<13u, 5u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<13u, 6u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<13u, 7u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<13u, 8u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<13u, 9u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<13u, 10u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<13u, 11u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<13u, 12u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<13u, 13u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<13u, 14u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<13u, 15u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<13u, 16u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<13u, 17u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<13u, 18u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<13u, 19u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<13u, 20u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<13u, 21u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<13u, 22u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<13u, 23u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<13u, 24u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<13u, 25u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<13u, 26u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<13u, 27u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<13u, 28u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<13u, 29u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<13u, 30u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<13u, 31u> {
    static const uint32_t WORD = 2280u;
};
// Tile 14: a_segments = b_segments = {6,7}.
template<> struct TileMeta<14u> {
    static const uint32_t SEGMENT_MASK = 0x000000c0u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<14u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<14u, 1u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<14u, 2u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<14u, 3u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<14u, 4u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<14u, 5u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<14u, 6u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<14u, 7u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<14u, 8u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<14u, 9u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<14u, 10u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<14u, 11u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<14u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<14u, 13u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<14u, 14u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<14u, 15u> {
    static const uint32_t WORD = 2220u;
};
// Tile 15: a_segments = b_segments = {4,6}.
template<> struct TileMeta<15u> {
    static const uint32_t SEGMENT_MASK = 0x00000050u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<15u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<15u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<15u, 2u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<15u, 3u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<15u, 4u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<15u, 5u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<15u, 6u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<15u, 7u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<15u, 8u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<15u, 9u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<15u, 10u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<15u, 11u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<15u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<15u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<15u, 14u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<15u, 15u> {
    static const uint32_t WORD = 2100u;
};
// Tile 16: a_segments = b_segments = {5,7}.
template<> struct TileMeta<16u> {
    static const uint32_t SEGMENT_MASK = 0x000000a0u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<16u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<16u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<16u, 2u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<16u, 3u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<16u, 4u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<16u, 5u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<16u, 6u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<16u, 7u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<16u, 8u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<16u, 9u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<16u, 10u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<16u, 11u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<16u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<16u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<16u, 14u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<16u, 15u> {
    static const uint32_t WORD = 2160u;
};
// Tile 17: a_segments = b_segments = {4,5,6,7}.
template<> struct TileMeta<17u> {
    static const uint32_t SEGMENT_MASK = 0x000000f0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<17u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<17u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<17u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<17u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<17u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<17u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<17u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<17u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 18: a_segments = b_segments = {0,4}.
template<> struct TileMeta<18u> {
    static const uint32_t SEGMENT_MASK = 0x00000011u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<18u, 0u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<18u, 1u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<18u, 2u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<18u, 3u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<18u, 4u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<18u, 5u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<18u, 6u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<18u, 7u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<18u, 8u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<18u, 9u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<18u, 10u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<18u, 11u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<18u, 12u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<18u, 13u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<18u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<18u, 15u> {
    static const uint32_t WORD = 1860u;
};
// Tile 19: a_segments = b_segments = {1,5}.
template<> struct TileMeta<19u> {
    static const uint32_t SEGMENT_MASK = 0x00000022u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<19u, 0u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<19u, 1u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<19u, 2u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<19u, 3u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<19u, 4u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<19u, 5u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<19u, 6u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<19u, 7u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<19u, 8u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<19u, 9u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<19u, 10u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<19u, 11u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<19u, 12u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<19u, 13u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<19u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<19u, 15u> {
    static const uint32_t WORD = 1920u;
};
// Tile 20: a_segments = b_segments = {0,1,4,5}.
template<> struct TileMeta<20u> {
    static const uint32_t SEGMENT_MASK = 0x00000033u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<20u, 0u> {
    static const uint32_t WORD = 300u;
};
template<> struct TileOffset<20u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<20u, 2u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<20u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<20u, 4u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<20u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<20u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<20u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 21: a_segments = b_segments = {2,6}.
template<> struct TileMeta<21u> {
    static const uint32_t SEGMENT_MASK = 0x00000044u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<21u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<21u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<21u, 2u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<21u, 3u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<21u, 4u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<21u, 5u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<21u, 6u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<21u, 7u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<21u, 8u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<21u, 9u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<21u, 10u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<21u, 11u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<21u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<21u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<21u, 14u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<21u, 15u> {
    static const uint32_t WORD = 1980u;
};
// Tile 22: a_segments = b_segments = {3,7}.
template<> struct TileMeta<22u> {
    static const uint32_t SEGMENT_MASK = 0x00000088u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<22u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<22u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<22u, 2u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<22u, 3u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<22u, 4u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<22u, 5u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<22u, 6u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<22u, 7u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<22u, 8u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<22u, 9u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<22u, 10u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<22u, 11u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<22u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<22u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<22u, 14u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<22u, 15u> {
    static const uint32_t WORD = 2040u;
};
// Tile 23: a_segments = b_segments = {2,3,6,7}.
template<> struct TileMeta<23u> {
    static const uint32_t SEGMENT_MASK = 0x000000ccu;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<23u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<23u, 1u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<23u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<23u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<23u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<23u, 5u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<23u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<23u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 24: a_segments = b_segments = {0,2,4,6}.
template<> struct TileMeta<24u> {
    static const uint32_t SEGMENT_MASK = 0x00000055u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<24u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<24u, 1u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<24u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<24u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<24u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<24u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<24u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<24u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 25: a_segments = b_segments = {1,3,5,7}.
template<> struct TileMeta<25u> {
    static const uint32_t SEGMENT_MASK = 0x000000aau;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<25u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<25u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<25u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<25u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<25u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<25u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<25u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<25u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 26: a_segments = b_segments = {0,1,2,3,4,5,6,7}.
template<> struct TileMeta<26u> {
    static const uint32_t SEGMENT_MASK = 0x000000ffu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<26u, 0u> {
    static const uint32_t WORD = 420u;
};
template<> struct TileOffset<26u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<26u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<26u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 27: a_segments = b_segments = {8}.
template<> struct TileMeta<27u> {
    static const uint32_t SEGMENT_MASK = 0x00000100u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<27u, 0u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<27u, 1u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<27u, 2u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<27u, 3u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<27u, 4u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<27u, 5u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<27u, 6u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<27u, 7u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<27u, 8u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<27u, 9u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<27u, 10u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<27u, 11u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<27u, 12u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<27u, 13u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<27u, 14u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<27u, 15u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<27u, 16u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<27u, 17u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<27u, 18u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<27u, 19u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<27u, 20u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<27u, 21u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<27u, 22u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<27u, 23u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<27u, 24u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<27u, 25u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<27u, 26u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<27u, 27u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<27u, 28u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<27u, 29u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<27u, 30u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<27u, 31u> {
    static const uint32_t WORD = 2340u;
};
// Tile 28: a_segments = b_segments = {9}.
template<> struct TileMeta<28u> {
    static const uint32_t SEGMENT_MASK = 0x00000200u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<28u, 0u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<28u, 1u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<28u, 2u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<28u, 3u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<28u, 4u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<28u, 5u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<28u, 6u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<28u, 7u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<28u, 8u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<28u, 9u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<28u, 10u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<28u, 11u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<28u, 12u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<28u, 13u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<28u, 14u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<28u, 15u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<28u, 16u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<28u, 17u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<28u, 18u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<28u, 19u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<28u, 20u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<28u, 21u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<28u, 22u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<28u, 23u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<28u, 24u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<28u, 25u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<28u, 26u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<28u, 27u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<28u, 28u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<28u, 29u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<28u, 30u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<28u, 31u> {
    static const uint32_t WORD = 2400u;
};
// Tile 29: a_segments = b_segments = {8,9}.
template<> struct TileMeta<29u> {
    static const uint32_t SEGMENT_MASK = 0x00000300u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<29u, 0u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<29u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<29u, 2u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<29u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<29u, 4u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<29u, 5u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<29u, 6u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<29u, 7u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<29u, 8u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<29u, 9u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<29u, 10u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<29u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<29u, 12u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<29u, 13u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<29u, 14u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<29u, 15u> {
    static const uint32_t WORD = 2340u;
};
// Tile 30: a_segments = b_segments = {10}.
template<> struct TileMeta<30u> {
    static const uint32_t SEGMENT_MASK = 0x00000400u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<30u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<30u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<30u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<30u, 3u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<30u, 4u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<30u, 5u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<30u, 6u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<30u, 7u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<30u, 8u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<30u, 9u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<30u, 10u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<30u, 11u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<30u, 12u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<30u, 13u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<30u, 14u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<30u, 15u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<30u, 16u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<30u, 17u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<30u, 18u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<30u, 19u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<30u, 20u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<30u, 21u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<30u, 22u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<30u, 23u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<30u, 24u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<30u, 25u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<30u, 26u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<30u, 27u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<30u, 28u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<30u, 29u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<30u, 30u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<30u, 31u> {
    static const uint32_t WORD = 2460u;
};
// Tile 31: a_segments = b_segments = {11}.
template<> struct TileMeta<31u> {
    static const uint32_t SEGMENT_MASK = 0x00000800u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<31u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<31u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<31u, 2u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<31u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<31u, 4u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<31u, 5u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<31u, 6u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<31u, 7u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<31u, 8u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<31u, 9u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<31u, 10u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<31u, 11u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<31u, 12u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<31u, 13u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<31u, 14u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<31u, 15u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<31u, 16u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<31u, 17u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<31u, 18u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<31u, 19u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<31u, 20u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<31u, 21u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<31u, 22u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<31u, 23u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<31u, 24u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<31u, 25u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<31u, 26u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<31u, 27u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<31u, 28u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<31u, 29u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<31u, 30u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<31u, 31u> {
    static const uint32_t WORD = 2520u;
};
// Tile 32: a_segments = b_segments = {10,11}.
template<> struct TileMeta<32u> {
    static const uint32_t SEGMENT_MASK = 0x00000c00u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<32u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<32u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<32u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<32u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<32u, 4u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<32u, 5u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<32u, 6u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<32u, 7u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<32u, 8u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<32u, 9u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<32u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<32u, 11u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<32u, 12u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<32u, 13u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<32u, 14u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<32u, 15u> {
    static const uint32_t WORD = 2460u;
};
// Tile 33: a_segments = b_segments = {8,10}.
template<> struct TileMeta<33u> {
    static const uint32_t SEGMENT_MASK = 0x00000500u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<33u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<33u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<33u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<33u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<33u, 4u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<33u, 5u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<33u, 6u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<33u, 7u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<33u, 8u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<33u, 9u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<33u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<33u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<33u, 12u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<33u, 13u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<33u, 14u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<33u, 15u> {
    static const uint32_t WORD = 2340u;
};
// Tile 34: a_segments = b_segments = {9,11}.
template<> struct TileMeta<34u> {
    static const uint32_t SEGMENT_MASK = 0x00000a00u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<34u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<34u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<34u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<34u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<34u, 4u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<34u, 5u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<34u, 6u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<34u, 7u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<34u, 8u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<34u, 9u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<34u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<34u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<34u, 12u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<34u, 13u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<34u, 14u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<34u, 15u> {
    static const uint32_t WORD = 2400u;
};
// Tile 35: a_segments = b_segments = {8,9,10,11}.
template<> struct TileMeta<35u> {
    static const uint32_t SEGMENT_MASK = 0x00000f00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<35u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<35u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<35u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<35u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<35u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<35u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<35u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<35u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 36: a_segments = b_segments = {12}.
template<> struct TileMeta<36u> {
    static const uint32_t SEGMENT_MASK = 0x00001000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<36u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<36u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<36u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<36u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<36u, 4u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<36u, 5u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<36u, 6u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<36u, 7u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<36u, 8u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<36u, 9u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<36u, 10u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<36u, 11u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<36u, 12u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<36u, 13u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<36u, 14u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<36u, 15u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<36u, 16u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<36u, 17u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<36u, 18u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<36u, 19u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<36u, 20u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<36u, 21u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<36u, 22u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<36u, 23u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<36u, 24u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<36u, 25u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<36u, 26u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<36u, 27u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<36u, 28u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<36u, 29u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<36u, 30u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<36u, 31u> {
    static const uint32_t WORD = 2580u;
};
// Tile 37: a_segments = b_segments = {13}.
template<> struct TileMeta<37u> {
    static const uint32_t SEGMENT_MASK = 0x00002000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<37u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<37u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<37u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<37u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<37u, 4u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<37u, 5u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<37u, 6u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<37u, 7u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<37u, 8u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<37u, 9u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<37u, 10u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<37u, 11u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<37u, 12u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<37u, 13u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<37u, 14u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<37u, 15u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<37u, 16u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<37u, 17u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<37u, 18u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<37u, 19u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<37u, 20u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<37u, 21u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<37u, 22u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<37u, 23u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<37u, 24u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<37u, 25u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<37u, 26u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<37u, 27u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<37u, 28u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<37u, 29u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<37u, 30u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<37u, 31u> {
    static const uint32_t WORD = 2640u;
};
// Tile 38: a_segments = b_segments = {12,13}.
template<> struct TileMeta<38u> {
    static const uint32_t SEGMENT_MASK = 0x00003000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<38u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<38u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<38u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<38u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<38u, 4u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<38u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<38u, 6u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<38u, 7u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<38u, 8u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<38u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<38u, 10u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<38u, 11u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<38u, 12u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<38u, 13u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<38u, 14u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<38u, 15u> {
    static const uint32_t WORD = 2580u;
};
// Tile 39: a_segments = b_segments = {14}.
template<> struct TileMeta<39u> {
    static const uint32_t SEGMENT_MASK = 0x00004000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<39u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<39u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<39u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<39u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<39u, 4u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<39u, 5u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<39u, 6u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<39u, 7u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<39u, 8u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<39u, 9u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<39u, 10u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<39u, 11u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<39u, 12u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<39u, 13u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<39u, 14u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<39u, 15u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<39u, 16u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<39u, 17u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<39u, 18u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<39u, 19u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<39u, 20u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<39u, 21u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<39u, 22u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<39u, 23u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<39u, 24u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<39u, 25u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<39u, 26u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<39u, 27u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<39u, 28u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<39u, 29u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<39u, 30u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<39u, 31u> {
    static const uint32_t WORD = 2700u;
};
// Tile 40: a_segments = b_segments = {15}.
template<> struct TileMeta<40u> {
    static const uint32_t SEGMENT_MASK = 0x00008000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<40u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<40u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<40u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<40u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<40u, 4u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<40u, 5u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<40u, 6u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<40u, 7u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<40u, 8u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<40u, 9u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<40u, 10u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<40u, 11u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<40u, 12u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<40u, 13u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<40u, 14u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<40u, 15u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<40u, 16u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<40u, 17u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<40u, 18u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<40u, 19u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<40u, 20u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<40u, 21u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<40u, 22u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<40u, 23u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<40u, 24u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<40u, 25u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<40u, 26u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<40u, 27u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<40u, 28u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<40u, 29u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<40u, 30u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<40u, 31u> {
    static const uint32_t WORD = 2760u;
};
// Tile 41: a_segments = b_segments = {14,15}.
template<> struct TileMeta<41u> {
    static const uint32_t SEGMENT_MASK = 0x0000c000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<41u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<41u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<41u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<41u, 3u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<41u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<41u, 5u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<41u, 6u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<41u, 7u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<41u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<41u, 9u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<41u, 10u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<41u, 11u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<41u, 12u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<41u, 13u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<41u, 14u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<41u, 15u> {
    static const uint32_t WORD = 2700u;
};
// Tile 42: a_segments = b_segments = {12,14}.
template<> struct TileMeta<42u> {
    static const uint32_t SEGMENT_MASK = 0x00005000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<42u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<42u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<42u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<42u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<42u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<42u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<42u, 6u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<42u, 7u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<42u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<42u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<42u, 10u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<42u, 11u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<42u, 12u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<42u, 13u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<42u, 14u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<42u, 15u> {
    static const uint32_t WORD = 2580u;
};
// Tile 43: a_segments = b_segments = {13,15}.
template<> struct TileMeta<43u> {
    static const uint32_t SEGMENT_MASK = 0x0000a000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<43u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<43u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<43u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<43u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<43u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<43u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<43u, 6u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<43u, 7u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<43u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<43u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<43u, 10u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<43u, 11u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<43u, 12u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<43u, 13u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<43u, 14u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<43u, 15u> {
    static const uint32_t WORD = 2640u;
};
// Tile 44: a_segments = b_segments = {12,13,14,15}.
template<> struct TileMeta<44u> {
    static const uint32_t SEGMENT_MASK = 0x0000f000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<44u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<44u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<44u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<44u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<44u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<44u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<44u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<44u, 7u> {
    static const uint32_t WORD = 2580u;
};
// Tile 45: a_segments = b_segments = {8,12}.
template<> struct TileMeta<45u> {
    static const uint32_t SEGMENT_MASK = 0x00001100u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<45u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<45u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<45u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<45u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<45u, 4u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<45u, 5u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<45u, 6u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<45u, 7u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<45u, 8u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<45u, 9u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<45u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<45u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<45u, 12u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<45u, 13u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<45u, 14u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<45u, 15u> {
    static const uint32_t WORD = 2340u;
};
// Tile 46: a_segments = b_segments = {9,13}.
template<> struct TileMeta<46u> {
    static const uint32_t SEGMENT_MASK = 0x00002200u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<46u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<46u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<46u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<46u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<46u, 4u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<46u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<46u, 6u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<46u, 7u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<46u, 8u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<46u, 9u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<46u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<46u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<46u, 12u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<46u, 13u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<46u, 14u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<46u, 15u> {
    static const uint32_t WORD = 2400u;
};
// Tile 47: a_segments = b_segments = {8,9,12,13}.
template<> struct TileMeta<47u> {
    static const uint32_t SEGMENT_MASK = 0x00003300u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<47u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<47u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<47u, 2u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<47u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<47u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<47u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<47u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<47u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 48: a_segments = b_segments = {10,14}.
template<> struct TileMeta<48u> {
    static const uint32_t SEGMENT_MASK = 0x00004400u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<48u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<48u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<48u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<48u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<48u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<48u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<48u, 6u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<48u, 7u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<48u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<48u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<48u, 10u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<48u, 11u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<48u, 12u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<48u, 13u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<48u, 14u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<48u, 15u> {
    static const uint32_t WORD = 2460u;
};
// Tile 49: a_segments = b_segments = {11,15}.
template<> struct TileMeta<49u> {
    static const uint32_t SEGMENT_MASK = 0x00008800u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<49u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<49u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<49u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<49u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<49u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<49u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<49u, 6u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<49u, 7u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<49u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<49u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<49u, 10u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<49u, 11u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<49u, 12u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<49u, 13u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<49u, 14u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<49u, 15u> {
    static const uint32_t WORD = 2520u;
};
// Tile 50: a_segments = b_segments = {10,11,14,15}.
template<> struct TileMeta<50u> {
    static const uint32_t SEGMENT_MASK = 0x0000cc00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<50u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<50u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<50u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<50u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<50u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<50u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<50u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<50u, 7u> {
    static const uint32_t WORD = 2460u;
};
// Tile 51: a_segments = b_segments = {8,10,12,14}.
template<> struct TileMeta<51u> {
    static const uint32_t SEGMENT_MASK = 0x00005500u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<51u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<51u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<51u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<51u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<51u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<51u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<51u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<51u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 52: a_segments = b_segments = {9,11,13,15}.
template<> struct TileMeta<52u> {
    static const uint32_t SEGMENT_MASK = 0x0000aa00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<52u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<52u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<52u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<52u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<52u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<52u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<52u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<52u, 7u> {
    static const uint32_t WORD = 2400u;
};
// Tile 53: a_segments = b_segments = {8,9,10,11,12,13,14,15}.
template<> struct TileMeta<53u> {
    static const uint32_t SEGMENT_MASK = 0x0000ff00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<53u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<53u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<53u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<53u, 3u> {
    static const uint32_t WORD = 2340u;
};
// Tile 54: a_segments = b_segments = {0,8}.
template<> struct TileMeta<54u> {
    static const uint32_t SEGMENT_MASK = 0x00000101u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<54u, 0u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<54u, 1u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<54u, 2u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<54u, 3u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<54u, 4u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<54u, 5u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<54u, 6u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<54u, 7u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<54u, 8u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<54u, 9u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<54u, 10u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<54u, 11u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<54u, 12u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<54u, 13u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<54u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<54u, 15u> {
    static const uint32_t WORD = 1860u;
};
// Tile 55: a_segments = b_segments = {1,9}.
template<> struct TileMeta<55u> {
    static const uint32_t SEGMENT_MASK = 0x00000202u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<55u, 0u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<55u, 1u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<55u, 2u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<55u, 3u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<55u, 4u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<55u, 5u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<55u, 6u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<55u, 7u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<55u, 8u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<55u, 9u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<55u, 10u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<55u, 11u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<55u, 12u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<55u, 13u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<55u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<55u, 15u> {
    static const uint32_t WORD = 1920u;
};
// Tile 56: a_segments = b_segments = {0,1,8,9}.
template<> struct TileMeta<56u> {
    static const uint32_t SEGMENT_MASK = 0x00000303u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<56u, 0u> {
    static const uint32_t WORD = 540u;
};
template<> struct TileOffset<56u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<56u, 2u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<56u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<56u, 4u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<56u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<56u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<56u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 57: a_segments = b_segments = {2,10}.
template<> struct TileMeta<57u> {
    static const uint32_t SEGMENT_MASK = 0x00000404u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<57u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<57u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<57u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<57u, 3u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<57u, 4u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<57u, 5u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<57u, 6u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<57u, 7u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<57u, 8u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<57u, 9u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<57u, 10u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<57u, 11u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<57u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<57u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<57u, 14u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<57u, 15u> {
    static const uint32_t WORD = 1980u;
};
// Tile 58: a_segments = b_segments = {3,11}.
template<> struct TileMeta<58u> {
    static const uint32_t SEGMENT_MASK = 0x00000808u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<58u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<58u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<58u, 2u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<58u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<58u, 4u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<58u, 5u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<58u, 6u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<58u, 7u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<58u, 8u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<58u, 9u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<58u, 10u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<58u, 11u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<58u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<58u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<58u, 14u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<58u, 15u> {
    static const uint32_t WORD = 2040u;
};
// Tile 59: a_segments = b_segments = {2,3,10,11}.
template<> struct TileMeta<59u> {
    static const uint32_t SEGMENT_MASK = 0x00000c0cu;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<59u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<59u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<59u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<59u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<59u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<59u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<59u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<59u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 60: a_segments = b_segments = {0,2,8,10}.
template<> struct TileMeta<60u> {
    static const uint32_t SEGMENT_MASK = 0x00000505u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<60u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<60u, 1u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<60u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<60u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<60u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<60u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<60u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<60u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 61: a_segments = b_segments = {1,3,9,11}.
template<> struct TileMeta<61u> {
    static const uint32_t SEGMENT_MASK = 0x00000a0au;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<61u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<61u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<61u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<61u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<61u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<61u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<61u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<61u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 62: a_segments = b_segments = {0,1,2,3,8,9,10,11}.
template<> struct TileMeta<62u> {
    static const uint32_t SEGMENT_MASK = 0x00000f0fu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<62u, 0u> {
    static const uint32_t WORD = 660u;
};
template<> struct TileOffset<62u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<62u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<62u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 63: a_segments = b_segments = {4,12}.
template<> struct TileMeta<63u> {
    static const uint32_t SEGMENT_MASK = 0x00001010u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<63u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<63u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<63u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<63u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<63u, 4u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<63u, 5u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<63u, 6u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<63u, 7u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<63u, 8u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<63u, 9u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<63u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<63u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<63u, 12u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<63u, 13u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<63u, 14u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<63u, 15u> {
    static const uint32_t WORD = 2100u;
};
// Tile 64: a_segments = b_segments = {5,13}.
template<> struct TileMeta<64u> {
    static const uint32_t SEGMENT_MASK = 0x00002020u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<64u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<64u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<64u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<64u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<64u, 4u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<64u, 5u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<64u, 6u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<64u, 7u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<64u, 8u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<64u, 9u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<64u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<64u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<64u, 12u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<64u, 13u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<64u, 14u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<64u, 15u> {
    static const uint32_t WORD = 2160u;
};
// Tile 65: a_segments = b_segments = {4,5,12,13}.
template<> struct TileMeta<65u> {
    static const uint32_t SEGMENT_MASK = 0x00003030u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<65u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<65u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<65u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<65u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<65u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<65u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<65u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<65u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 66: a_segments = b_segments = {6,14}.
template<> struct TileMeta<66u> {
    static const uint32_t SEGMENT_MASK = 0x00004040u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<66u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<66u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<66u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<66u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<66u, 4u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<66u, 5u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<66u, 6u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<66u, 7u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<66u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<66u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<66u, 10u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<66u, 11u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<66u, 12u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<66u, 13u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<66u, 14u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<66u, 15u> {
    static const uint32_t WORD = 2220u;
};
// Tile 67: a_segments = b_segments = {7,15}.
template<> struct TileMeta<67u> {
    static const uint32_t SEGMENT_MASK = 0x00008080u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<67u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<67u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<67u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<67u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<67u, 4u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<67u, 5u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<67u, 6u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<67u, 7u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<67u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<67u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<67u, 10u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<67u, 11u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<67u, 12u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<67u, 13u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<67u, 14u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<67u, 15u> {
    static const uint32_t WORD = 2280u;
};
// Tile 68: a_segments = b_segments = {6,7,14,15}.
template<> struct TileMeta<68u> {
    static const uint32_t SEGMENT_MASK = 0x0000c0c0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<68u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<68u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<68u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<68u, 3u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<68u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<68u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<68u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<68u, 7u> {
    static const uint32_t WORD = 2220u;
};
// Tile 69: a_segments = b_segments = {4,6,12,14}.
template<> struct TileMeta<69u> {
    static const uint32_t SEGMENT_MASK = 0x00005050u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<69u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<69u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<69u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<69u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<69u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<69u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<69u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<69u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 70: a_segments = b_segments = {5,7,13,15}.
template<> struct TileMeta<70u> {
    static const uint32_t SEGMENT_MASK = 0x0000a0a0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<70u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<70u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<70u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<70u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<70u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<70u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<70u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<70u, 7u> {
    static const uint32_t WORD = 2160u;
};
// Tile 71: a_segments = b_segments = {4,5,6,7,12,13,14,15}.
template<> struct TileMeta<71u> {
    static const uint32_t SEGMENT_MASK = 0x0000f0f0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<71u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<71u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<71u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<71u, 3u> {
    static const uint32_t WORD = 2100u;
};
// Tile 72: a_segments = b_segments = {0,4,8,12}.
template<> struct TileMeta<72u> {
    static const uint32_t SEGMENT_MASK = 0x00001111u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<72u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<72u, 1u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<72u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<72u, 3u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<72u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<72u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<72u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<72u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 73: a_segments = b_segments = {1,5,9,13}.
template<> struct TileMeta<73u> {
    static const uint32_t SEGMENT_MASK = 0x00002222u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<73u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<73u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<73u, 2u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<73u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<73u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<73u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<73u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<73u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 74: a_segments = b_segments = {0,1,4,5,8,9,12,13}.
template<> struct TileMeta<74u> {
    static const uint32_t SEGMENT_MASK = 0x00003333u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<74u, 0u> {
    static const uint32_t WORD = 780u;
};
template<> struct TileOffset<74u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<74u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<74u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 75: a_segments = b_segments = {2,6,10,14}.
template<> struct TileMeta<75u> {
    static const uint32_t SEGMENT_MASK = 0x00004444u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<75u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<75u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<75u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<75u, 3u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<75u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<75u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<75u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<75u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 76: a_segments = b_segments = {3,7,11,15}.
template<> struct TileMeta<76u> {
    static const uint32_t SEGMENT_MASK = 0x00008888u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<76u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<76u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<76u, 2u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<76u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<76u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<76u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<76u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<76u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 77: a_segments = b_segments = {2,3,6,7,10,11,14,15}.
template<> struct TileMeta<77u> {
    static const uint32_t SEGMENT_MASK = 0x0000ccccu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<77u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<77u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<77u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<77u, 3u> {
    static const uint32_t WORD = 1980u;
};
// Tile 78: a_segments = b_segments = {0,2,4,6,8,10,12,14}.
template<> struct TileMeta<78u> {
    static const uint32_t SEGMENT_MASK = 0x00005555u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<78u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<78u, 1u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<78u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<78u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 79: a_segments = b_segments = {1,3,5,7,9,11,13,15}.
template<> struct TileMeta<79u> {
    static const uint32_t SEGMENT_MASK = 0x0000aaaau;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<79u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<79u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<79u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<79u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 80: a_segments = b_segments = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}.
template<> struct TileMeta<80u> {
    static const uint32_t SEGMENT_MASK = 0x0000ffffu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<80u, 0u> {
    static const uint32_t WORD = 900u;
};
template<> struct TileOffset<80u, 1u> {
    static const uint32_t WORD = 1860u;
};
// Tile 81: a_segments = b_segments = {16}.
template<> struct TileMeta<81u> {
    static const uint32_t SEGMENT_MASK = 0x00010000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<81u, 0u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<81u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<81u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<81u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<81u, 4u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<81u, 5u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<81u, 6u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<81u, 7u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<81u, 8u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<81u, 9u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<81u, 10u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<81u, 11u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<81u, 12u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<81u, 13u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<81u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<81u, 15u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<81u, 16u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<81u, 17u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<81u, 18u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<81u, 19u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<81u, 20u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<81u, 21u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<81u, 22u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<81u, 23u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<81u, 24u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<81u, 25u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<81u, 26u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<81u, 27u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<81u, 28u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<81u, 29u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<81u, 30u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<81u, 31u> {
    static const uint32_t WORD = 2820u;
};
// Tile 82: a_segments = b_segments = {17}.
template<> struct TileMeta<82u> {
    static const uint32_t SEGMENT_MASK = 0x00020000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<82u, 0u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<82u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<82u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<82u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<82u, 4u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<82u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<82u, 6u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<82u, 7u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<82u, 8u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<82u, 9u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<82u, 10u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<82u, 11u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<82u, 12u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<82u, 13u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<82u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<82u, 15u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<82u, 16u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<82u, 17u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<82u, 18u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<82u, 19u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<82u, 20u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<82u, 21u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<82u, 22u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<82u, 23u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<82u, 24u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<82u, 25u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<82u, 26u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<82u, 27u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<82u, 28u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<82u, 29u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<82u, 30u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<82u, 31u> {
    static const uint32_t WORD = 2880u;
};
// Tile 83: a_segments = b_segments = {16,17}.
template<> struct TileMeta<83u> {
    static const uint32_t SEGMENT_MASK = 0x00030000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<83u, 0u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<83u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<83u, 2u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<83u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<83u, 4u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<83u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<83u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<83u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<83u, 8u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<83u, 9u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<83u, 10u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<83u, 11u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<83u, 12u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<83u, 13u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<83u, 14u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<83u, 15u> {
    static const uint32_t WORD = 2820u;
};
// Tile 84: a_segments = b_segments = {18}.
template<> struct TileMeta<84u> {
    static const uint32_t SEGMENT_MASK = 0x00040000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<84u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<84u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<84u, 2u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<84u, 3u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<84u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<84u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<84u, 6u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<84u, 7u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<84u, 8u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<84u, 9u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<84u, 10u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<84u, 11u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<84u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<84u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<84u, 14u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<84u, 15u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<84u, 16u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<84u, 17u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<84u, 18u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<84u, 19u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<84u, 20u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<84u, 21u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<84u, 22u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<84u, 23u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<84u, 24u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<84u, 25u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<84u, 26u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<84u, 27u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<84u, 28u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<84u, 29u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<84u, 30u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<84u, 31u> {
    static const uint32_t WORD = 2940u;
};
// Tile 85: a_segments = b_segments = {19}.
template<> struct TileMeta<85u> {
    static const uint32_t SEGMENT_MASK = 0x00080000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<85u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<85u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<85u, 2u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<85u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<85u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<85u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<85u, 6u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<85u, 7u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<85u, 8u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<85u, 9u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<85u, 10u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<85u, 11u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<85u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<85u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<85u, 14u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<85u, 15u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<85u, 16u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<85u, 17u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<85u, 18u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<85u, 19u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<85u, 20u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<85u, 21u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<85u, 22u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<85u, 23u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<85u, 24u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<85u, 25u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<85u, 26u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<85u, 27u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<85u, 28u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<85u, 29u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<85u, 30u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<85u, 31u> {
    static const uint32_t WORD = 3000u;
};
// Tile 86: a_segments = b_segments = {18,19}.
template<> struct TileMeta<86u> {
    static const uint32_t SEGMENT_MASK = 0x000c0000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<86u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<86u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<86u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<86u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<86u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<86u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<86u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<86u, 7u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<86u, 8u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<86u, 9u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<86u, 10u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<86u, 11u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<86u, 12u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<86u, 13u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<86u, 14u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<86u, 15u> {
    static const uint32_t WORD = 2940u;
};
// Tile 87: a_segments = b_segments = {16,18}.
template<> struct TileMeta<87u> {
    static const uint32_t SEGMENT_MASK = 0x00050000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<87u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<87u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<87u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<87u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<87u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<87u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<87u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<87u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<87u, 8u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<87u, 9u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<87u, 10u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<87u, 11u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<87u, 12u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<87u, 13u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<87u, 14u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<87u, 15u> {
    static const uint32_t WORD = 2820u;
};
// Tile 88: a_segments = b_segments = {17,19}.
template<> struct TileMeta<88u> {
    static const uint32_t SEGMENT_MASK = 0x000a0000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<88u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<88u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<88u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<88u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<88u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<88u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<88u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<88u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<88u, 8u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<88u, 9u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<88u, 10u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<88u, 11u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<88u, 12u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<88u, 13u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<88u, 14u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<88u, 15u> {
    static const uint32_t WORD = 2880u;
};
// Tile 89: a_segments = b_segments = {16,17,18,19}.
template<> struct TileMeta<89u> {
    static const uint32_t SEGMENT_MASK = 0x000f0000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<89u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<89u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<89u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<89u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<89u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<89u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<89u, 6u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<89u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 90: a_segments = b_segments = {20}.
template<> struct TileMeta<90u> {
    static const uint32_t SEGMENT_MASK = 0x00100000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<90u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<90u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<90u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<90u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<90u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<90u, 5u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<90u, 6u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<90u, 7u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<90u, 8u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<90u, 9u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<90u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<90u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<90u, 12u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<90u, 13u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<90u, 14u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<90u, 15u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<90u, 16u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<90u, 17u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<90u, 18u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<90u, 19u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<90u, 20u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<90u, 21u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<90u, 22u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<90u, 23u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<90u, 24u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<90u, 25u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<90u, 26u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<90u, 27u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<90u, 28u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<90u, 29u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<90u, 30u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<90u, 31u> {
    static const uint32_t WORD = 3060u;
};
// Tile 91: a_segments = b_segments = {21}.
template<> struct TileMeta<91u> {
    static const uint32_t SEGMENT_MASK = 0x00200000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<91u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<91u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<91u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<91u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<91u, 4u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<91u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<91u, 6u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<91u, 7u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<91u, 8u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<91u, 9u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<91u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<91u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<91u, 12u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<91u, 13u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<91u, 14u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<91u, 15u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<91u, 16u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<91u, 17u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<91u, 18u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<91u, 19u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<91u, 20u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<91u, 21u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<91u, 22u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<91u, 23u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<91u, 24u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<91u, 25u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<91u, 26u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<91u, 27u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<91u, 28u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<91u, 29u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<91u, 30u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<91u, 31u> {
    static const uint32_t WORD = 3120u;
};
// Tile 92: a_segments = b_segments = {20,21}.
template<> struct TileMeta<92u> {
    static const uint32_t SEGMENT_MASK = 0x00300000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<92u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<92u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<92u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<92u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<92u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<92u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<92u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<92u, 7u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<92u, 8u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<92u, 9u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<92u, 10u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<92u, 11u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<92u, 12u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<92u, 13u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<92u, 14u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<92u, 15u> {
    static const uint32_t WORD = 3060u;
};
// Tile 93: a_segments = b_segments = {22}.
template<> struct TileMeta<93u> {
    static const uint32_t SEGMENT_MASK = 0x00400000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<93u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<93u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<93u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<93u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<93u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<93u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<93u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<93u, 7u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<93u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<93u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<93u, 10u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<93u, 11u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<93u, 12u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<93u, 13u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<93u, 14u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<93u, 15u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<93u, 16u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<93u, 17u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<93u, 18u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<93u, 19u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<93u, 20u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<93u, 21u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<93u, 22u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<93u, 23u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<93u, 24u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<93u, 25u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<93u, 26u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<93u, 27u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<93u, 28u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<93u, 29u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<93u, 30u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<93u, 31u> {
    static const uint32_t WORD = 3180u;
};
// Tile 94: a_segments = b_segments = {23}.
template<> struct TileMeta<94u> {
    static const uint32_t SEGMENT_MASK = 0x00800000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<94u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<94u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<94u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<94u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<94u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<94u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<94u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<94u, 7u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<94u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<94u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<94u, 10u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<94u, 11u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<94u, 12u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<94u, 13u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<94u, 14u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<94u, 15u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<94u, 16u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<94u, 17u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<94u, 18u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<94u, 19u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<94u, 20u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<94u, 21u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<94u, 22u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<94u, 23u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<94u, 24u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<94u, 25u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<94u, 26u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<94u, 27u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<94u, 28u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<94u, 29u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<94u, 30u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<94u, 31u> {
    static const uint32_t WORD = 3240u;
};
// Tile 95: a_segments = b_segments = {22,23}.
template<> struct TileMeta<95u> {
    static const uint32_t SEGMENT_MASK = 0x00c00000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<95u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<95u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<95u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<95u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<95u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<95u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<95u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<95u, 7u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<95u, 8u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<95u, 9u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<95u, 10u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<95u, 11u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<95u, 12u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<95u, 13u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<95u, 14u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<95u, 15u> {
    static const uint32_t WORD = 3180u;
};
// Tile 96: a_segments = b_segments = {20,22}.
template<> struct TileMeta<96u> {
    static const uint32_t SEGMENT_MASK = 0x00500000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<96u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<96u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<96u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<96u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<96u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<96u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<96u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<96u, 7u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<96u, 8u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<96u, 9u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<96u, 10u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<96u, 11u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<96u, 12u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<96u, 13u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<96u, 14u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<96u, 15u> {
    static const uint32_t WORD = 3060u;
};
// Tile 97: a_segments = b_segments = {21,23}.
template<> struct TileMeta<97u> {
    static const uint32_t SEGMENT_MASK = 0x00a00000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<97u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<97u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<97u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<97u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<97u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<97u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<97u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<97u, 7u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<97u, 8u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<97u, 9u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<97u, 10u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<97u, 11u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<97u, 12u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<97u, 13u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<97u, 14u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<97u, 15u> {
    static const uint32_t WORD = 3120u;
};
// Tile 98: a_segments = b_segments = {20,21,22,23}.
template<> struct TileMeta<98u> {
    static const uint32_t SEGMENT_MASK = 0x00f00000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<98u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<98u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<98u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<98u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<98u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<98u, 5u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<98u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<98u, 7u> {
    static const uint32_t WORD = 3060u;
};
// Tile 99: a_segments = b_segments = {16,20}.
template<> struct TileMeta<99u> {
    static const uint32_t SEGMENT_MASK = 0x00110000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<99u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<99u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<99u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<99u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<99u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<99u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<99u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<99u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<99u, 8u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<99u, 9u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<99u, 10u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<99u, 11u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<99u, 12u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<99u, 13u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<99u, 14u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<99u, 15u> {
    static const uint32_t WORD = 2820u;
};
// Tile 100: a_segments = b_segments = {17,21}.
template<> struct TileMeta<100u> {
    static const uint32_t SEGMENT_MASK = 0x00220000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<100u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<100u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<100u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<100u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<100u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<100u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<100u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<100u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<100u, 8u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<100u, 9u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<100u, 10u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<100u, 11u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<100u, 12u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<100u, 13u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<100u, 14u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<100u, 15u> {
    static const uint32_t WORD = 2880u;
};
// Tile 101: a_segments = b_segments = {16,17,20,21}.
template<> struct TileMeta<101u> {
    static const uint32_t SEGMENT_MASK = 0x00330000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<101u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<101u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<101u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<101u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<101u, 4u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<101u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<101u, 6u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<101u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 102: a_segments = b_segments = {18,22}.
template<> struct TileMeta<102u> {
    static const uint32_t SEGMENT_MASK = 0x00440000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<102u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<102u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<102u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<102u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<102u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<102u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<102u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<102u, 7u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<102u, 8u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<102u, 9u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<102u, 10u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<102u, 11u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<102u, 12u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<102u, 13u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<102u, 14u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<102u, 15u> {
    static const uint32_t WORD = 2940u;
};
// Tile 103: a_segments = b_segments = {19,23}.
template<> struct TileMeta<103u> {
    static const uint32_t SEGMENT_MASK = 0x00880000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<103u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<103u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<103u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<103u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<103u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<103u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<103u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<103u, 7u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<103u, 8u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<103u, 9u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<103u, 10u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<103u, 11u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<103u, 12u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<103u, 13u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<103u, 14u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<103u, 15u> {
    static const uint32_t WORD = 3000u;
};
// Tile 104: a_segments = b_segments = {18,19,22,23}.
template<> struct TileMeta<104u> {
    static const uint32_t SEGMENT_MASK = 0x00cc0000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<104u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<104u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<104u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<104u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<104u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<104u, 5u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<104u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<104u, 7u> {
    static const uint32_t WORD = 2940u;
};
// Tile 105: a_segments = b_segments = {16,18,20,22}.
template<> struct TileMeta<105u> {
    static const uint32_t SEGMENT_MASK = 0x00550000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<105u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<105u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<105u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<105u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<105u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<105u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<105u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<105u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 106: a_segments = b_segments = {17,19,21,23}.
template<> struct TileMeta<106u> {
    static const uint32_t SEGMENT_MASK = 0x00aa0000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<106u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<106u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<106u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<106u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<106u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<106u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<106u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<106u, 7u> {
    static const uint32_t WORD = 2880u;
};
// Tile 107: a_segments = b_segments = {16,17,18,19,20,21,22,23}.
template<> struct TileMeta<107u> {
    static const uint32_t SEGMENT_MASK = 0x00ff0000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<107u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<107u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<107u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<107u, 3u> {
    static const uint32_t WORD = 2820u;
};
// Tile 108: a_segments = b_segments = {24}.
template<> struct TileMeta<108u> {
    static const uint32_t SEGMENT_MASK = 0x01000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<108u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<108u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<108u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<108u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<108u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<108u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<108u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<108u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<108u, 8u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<108u, 9u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<108u, 10u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<108u, 11u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<108u, 12u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<108u, 13u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<108u, 14u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<108u, 15u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<108u, 16u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<108u, 17u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<108u, 18u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<108u, 19u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<108u, 20u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<108u, 21u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<108u, 22u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<108u, 23u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<108u, 24u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<108u, 25u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<108u, 26u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<108u, 27u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<108u, 28u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<108u, 29u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<108u, 30u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<108u, 31u> {
    static const uint32_t WORD = 3300u;
};
// Tile 109: a_segments = b_segments = {25}.
template<> struct TileMeta<109u> {
    static const uint32_t SEGMENT_MASK = 0x02000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<109u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<109u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<109u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<109u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<109u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<109u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<109u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<109u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<109u, 8u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<109u, 9u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<109u, 10u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<109u, 11u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<109u, 12u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<109u, 13u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<109u, 14u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<109u, 15u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<109u, 16u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<109u, 17u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<109u, 18u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<109u, 19u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<109u, 20u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<109u, 21u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<109u, 22u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<109u, 23u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<109u, 24u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<109u, 25u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<109u, 26u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<109u, 27u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<109u, 28u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<109u, 29u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<109u, 30u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<109u, 31u> {
    static const uint32_t WORD = 3360u;
};
// Tile 110: a_segments = b_segments = {24,25}.
template<> struct TileMeta<110u> {
    static const uint32_t SEGMENT_MASK = 0x03000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<110u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<110u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<110u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<110u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<110u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<110u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<110u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<110u, 7u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<110u, 8u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<110u, 9u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<110u, 10u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<110u, 11u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<110u, 12u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<110u, 13u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<110u, 14u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<110u, 15u> {
    static const uint32_t WORD = 3300u;
};
// Tile 111: a_segments = b_segments = {26}.
template<> struct TileMeta<111u> {
    static const uint32_t SEGMENT_MASK = 0x04000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<111u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<111u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<111u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<111u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<111u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<111u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<111u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<111u, 7u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<111u, 8u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<111u, 9u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<111u, 10u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<111u, 11u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<111u, 12u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<111u, 13u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<111u, 14u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<111u, 15u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<111u, 16u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<111u, 17u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<111u, 18u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<111u, 19u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<111u, 20u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<111u, 21u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<111u, 22u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<111u, 23u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<111u, 24u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<111u, 25u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<111u, 26u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<111u, 27u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<111u, 28u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<111u, 29u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<111u, 30u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<111u, 31u> {
    static const uint32_t WORD = 3420u;
};
// Tile 112: a_segments = b_segments = {27}.
template<> struct TileMeta<112u> {
    static const uint32_t SEGMENT_MASK = 0x08000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<112u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<112u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<112u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<112u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<112u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<112u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<112u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<112u, 7u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<112u, 8u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<112u, 9u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<112u, 10u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<112u, 11u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<112u, 12u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<112u, 13u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<112u, 14u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<112u, 15u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<112u, 16u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<112u, 17u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<112u, 18u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<112u, 19u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<112u, 20u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<112u, 21u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<112u, 22u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<112u, 23u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<112u, 24u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<112u, 25u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<112u, 26u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<112u, 27u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<112u, 28u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<112u, 29u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<112u, 30u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<112u, 31u> {
    static const uint32_t WORD = 3480u;
};
// Tile 113: a_segments = b_segments = {26,27}.
template<> struct TileMeta<113u> {
    static const uint32_t SEGMENT_MASK = 0x0c000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<113u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<113u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<113u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<113u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<113u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<113u, 5u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<113u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<113u, 7u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<113u, 8u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<113u, 9u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<113u, 10u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<113u, 11u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<113u, 12u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<113u, 13u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<113u, 14u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<113u, 15u> {
    static const uint32_t WORD = 3420u;
};
// Tile 114: a_segments = b_segments = {24,26}.
template<> struct TileMeta<114u> {
    static const uint32_t SEGMENT_MASK = 0x05000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<114u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<114u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<114u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<114u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<114u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<114u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<114u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<114u, 7u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<114u, 8u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<114u, 9u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<114u, 10u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<114u, 11u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<114u, 12u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<114u, 13u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<114u, 14u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<114u, 15u> {
    static const uint32_t WORD = 3300u;
};
// Tile 115: a_segments = b_segments = {25,27}.
template<> struct TileMeta<115u> {
    static const uint32_t SEGMENT_MASK = 0x0a000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<115u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<115u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<115u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<115u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<115u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<115u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<115u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<115u, 7u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<115u, 8u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<115u, 9u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<115u, 10u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<115u, 11u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<115u, 12u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<115u, 13u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<115u, 14u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<115u, 15u> {
    static const uint32_t WORD = 3360u;
};
// Tile 116: a_segments = b_segments = {24,25,26,27}.
template<> struct TileMeta<116u> {
    static const uint32_t SEGMENT_MASK = 0x0f000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<116u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<116u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<116u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<116u, 3u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<116u, 4u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<116u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<116u, 6u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<116u, 7u> {
    static const uint32_t WORD = 3300u;
};
// Tile 117: a_segments = b_segments = {28}.
template<> struct TileMeta<117u> {
    static const uint32_t SEGMENT_MASK = 0x10000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<117u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<117u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<117u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<117u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<117u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<117u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<117u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<117u, 7u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<117u, 8u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<117u, 9u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<117u, 10u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<117u, 11u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<117u, 12u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<117u, 13u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<117u, 14u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<117u, 15u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<117u, 16u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<117u, 17u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<117u, 18u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<117u, 19u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<117u, 20u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<117u, 21u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<117u, 22u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<117u, 23u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<117u, 24u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<117u, 25u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<117u, 26u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<117u, 27u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<117u, 28u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<117u, 29u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<117u, 30u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<117u, 31u> {
    static const uint32_t WORD = 3540u;
};
// Tile 118: a_segments = b_segments = {29}.
template<> struct TileMeta<118u> {
    static const uint32_t SEGMENT_MASK = 0x20000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<118u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<118u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<118u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<118u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<118u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<118u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<118u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<118u, 7u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<118u, 8u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<118u, 9u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<118u, 10u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<118u, 11u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<118u, 12u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<118u, 13u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<118u, 14u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<118u, 15u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<118u, 16u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<118u, 17u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<118u, 18u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<118u, 19u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<118u, 20u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<118u, 21u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<118u, 22u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<118u, 23u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<118u, 24u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<118u, 25u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<118u, 26u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<118u, 27u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<118u, 28u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<118u, 29u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<118u, 30u> {
    static const uint32_t WORD = 3540u;
};
template<> struct TileOffset<118u, 31u> {
    static const uint32_t WORD = 3600u;
};
// Tile 119: a_segments = b_segments = {28,29}.
template<> struct TileMeta<119u> {
    static const uint32_t SEGMENT_MASK = 0x30000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<119u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<119u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<119u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<119u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<119u, 4u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<119u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<119u, 6u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<119u, 7u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<119u, 8u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<119u, 9u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<119u, 10u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<119u, 11u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<119u, 12u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<119u, 13u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<119u, 14u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<119u, 15u> {
    static const uint32_t WORD = 3540u;
};
// Tile 120: a_segments = b_segments = {30}.
template<> struct TileMeta<120u> {
    static const uint32_t SEGMENT_MASK = 0x40000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<120u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<120u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<120u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<120u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<120u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<120u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<120u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<120u, 7u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<120u, 8u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<120u, 9u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<120u, 10u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<120u, 11u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<120u, 12u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<120u, 13u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<120u, 14u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<120u, 15u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<120u, 16u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<120u, 17u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<120u, 18u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<120u, 19u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<120u, 20u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<120u, 21u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<120u, 22u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<120u, 23u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<120u, 24u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<120u, 25u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<120u, 26u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<120u, 27u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<120u, 28u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<120u, 29u> {
    static const uint32_t WORD = 3540u;
};
template<> struct TileOffset<120u, 30u> {
    static const uint32_t WORD = 3600u;
};
template<> struct TileOffset<120u, 31u> {
    static const uint32_t WORD = 3660u;
};
// Tile 121: a_segments = b_segments = {31}.
template<> struct TileMeta<121u> {
    static const uint32_t SEGMENT_MASK = 0x80000000u;
    static const uint32_t OFFSET_COUNT = 32u;
};
template<> struct TileOffset<121u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<121u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<121u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<121u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<121u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<121u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<121u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<121u, 7u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<121u, 8u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<121u, 9u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<121u, 10u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<121u, 11u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<121u, 12u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<121u, 13u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<121u, 14u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<121u, 15u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<121u, 16u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<121u, 17u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<121u, 18u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<121u, 19u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<121u, 20u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<121u, 21u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<121u, 22u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<121u, 23u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<121u, 24u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<121u, 25u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<121u, 26u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<121u, 27u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<121u, 28u> {
    static const uint32_t WORD = 3540u;
};
template<> struct TileOffset<121u, 29u> {
    static const uint32_t WORD = 3600u;
};
template<> struct TileOffset<121u, 30u> {
    static const uint32_t WORD = 3660u;
};
template<> struct TileOffset<121u, 31u> {
    static const uint32_t WORD = 3720u;
};
// Tile 122: a_segments = b_segments = {30,31}.
template<> struct TileMeta<122u> {
    static const uint32_t SEGMENT_MASK = 0xc0000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<122u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<122u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<122u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<122u, 3u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<122u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<122u, 5u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<122u, 6u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<122u, 7u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<122u, 8u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<122u, 9u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<122u, 10u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<122u, 11u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<122u, 12u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<122u, 13u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<122u, 14u> {
    static const uint32_t WORD = 3540u;
};
template<> struct TileOffset<122u, 15u> {
    static const uint32_t WORD = 3660u;
};
// Tile 123: a_segments = b_segments = {28,30}.
template<> struct TileMeta<123u> {
    static const uint32_t SEGMENT_MASK = 0x50000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<123u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<123u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<123u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<123u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<123u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<123u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<123u, 6u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<123u, 7u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<123u, 8u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<123u, 9u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<123u, 10u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<123u, 11u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<123u, 12u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<123u, 13u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<123u, 14u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<123u, 15u> {
    static const uint32_t WORD = 3540u;
};
// Tile 124: a_segments = b_segments = {29,31}.
template<> struct TileMeta<124u> {
    static const uint32_t SEGMENT_MASK = 0xa0000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<124u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<124u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<124u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<124u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<124u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<124u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<124u, 6u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<124u, 7u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<124u, 8u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<124u, 9u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<124u, 10u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<124u, 11u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<124u, 12u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<124u, 13u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<124u, 14u> {
    static const uint32_t WORD = 3540u;
};
template<> struct TileOffset<124u, 15u> {
    static const uint32_t WORD = 3600u;
};
// Tile 125: a_segments = b_segments = {28,29,30,31}.
template<> struct TileMeta<125u> {
    static const uint32_t SEGMENT_MASK = 0xf0000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<125u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<125u, 1u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<125u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<125u, 3u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<125u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<125u, 5u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<125u, 6u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<125u, 7u> {
    static const uint32_t WORD = 3540u;
};
// Tile 126: a_segments = b_segments = {24,28}.
template<> struct TileMeta<126u> {
    static const uint32_t SEGMENT_MASK = 0x11000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<126u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<126u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<126u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<126u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<126u, 4u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<126u, 5u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<126u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<126u, 7u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<126u, 8u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<126u, 9u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<126u, 10u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<126u, 11u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<126u, 12u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<126u, 13u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<126u, 14u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<126u, 15u> {
    static const uint32_t WORD = 3300u;
};
// Tile 127: a_segments = b_segments = {25,29}.
template<> struct TileMeta<127u> {
    static const uint32_t SEGMENT_MASK = 0x22000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<127u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<127u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<127u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<127u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<127u, 4u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<127u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<127u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<127u, 7u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<127u, 8u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<127u, 9u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<127u, 10u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<127u, 11u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<127u, 12u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<127u, 13u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<127u, 14u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<127u, 15u> {
    static const uint32_t WORD = 3360u;
};
// Tile 128: a_segments = b_segments = {24,25,28,29}.
template<> struct TileMeta<128u> {
    static const uint32_t SEGMENT_MASK = 0x33000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<128u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<128u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<128u, 2u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<128u, 3u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<128u, 4u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<128u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<128u, 6u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<128u, 7u> {
    static const uint32_t WORD = 3300u;
};
// Tile 129: a_segments = b_segments = {26,30}.
template<> struct TileMeta<129u> {
    static const uint32_t SEGMENT_MASK = 0x44000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<129u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<129u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<129u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<129u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<129u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<129u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<129u, 6u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<129u, 7u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<129u, 8u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<129u, 9u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<129u, 10u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<129u, 11u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<129u, 12u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<129u, 13u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<129u, 14u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<129u, 15u> {
    static const uint32_t WORD = 3420u;
};
// Tile 130: a_segments = b_segments = {27,31}.
template<> struct TileMeta<130u> {
    static const uint32_t SEGMENT_MASK = 0x88000000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<130u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<130u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<130u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<130u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<130u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<130u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<130u, 6u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<130u, 7u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<130u, 8u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<130u, 9u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<130u, 10u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<130u, 11u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<130u, 12u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<130u, 13u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<130u, 14u> {
    static const uint32_t WORD = 3420u;
};
template<> struct TileOffset<130u, 15u> {
    static const uint32_t WORD = 3480u;
};
// Tile 131: a_segments = b_segments = {26,27,30,31}.
template<> struct TileMeta<131u> {
    static const uint32_t SEGMENT_MASK = 0xcc000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<131u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<131u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<131u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<131u, 3u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<131u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<131u, 5u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<131u, 6u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<131u, 7u> {
    static const uint32_t WORD = 3420u;
};
// Tile 132: a_segments = b_segments = {24,26,28,30}.
template<> struct TileMeta<132u> {
    static const uint32_t SEGMENT_MASK = 0x55000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<132u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<132u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<132u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<132u, 3u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<132u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<132u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<132u, 6u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<132u, 7u> {
    static const uint32_t WORD = 3300u;
};
// Tile 133: a_segments = b_segments = {25,27,29,31}.
template<> struct TileMeta<133u> {
    static const uint32_t SEGMENT_MASK = 0xaa000000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<133u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<133u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<133u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<133u, 3u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<133u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<133u, 5u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<133u, 6u> {
    static const uint32_t WORD = 3300u;
};
template<> struct TileOffset<133u, 7u> {
    static const uint32_t WORD = 3360u;
};
// Tile 134: a_segments = b_segments = {24,25,26,27,28,29,30,31}.
template<> struct TileMeta<134u> {
    static const uint32_t SEGMENT_MASK = 0xff000000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<134u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<134u, 1u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<134u, 2u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<134u, 3u> {
    static const uint32_t WORD = 3300u;
};
// Tile 135: a_segments = b_segments = {16,24}.
template<> struct TileMeta<135u> {
    static const uint32_t SEGMENT_MASK = 0x01010000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<135u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<135u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<135u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<135u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<135u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<135u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<135u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<135u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<135u, 8u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<135u, 9u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<135u, 10u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<135u, 11u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<135u, 12u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<135u, 13u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<135u, 14u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<135u, 15u> {
    static const uint32_t WORD = 2820u;
};
// Tile 136: a_segments = b_segments = {17,25}.
template<> struct TileMeta<136u> {
    static const uint32_t SEGMENT_MASK = 0x02020000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<136u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<136u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<136u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<136u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<136u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<136u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<136u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<136u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<136u, 8u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<136u, 9u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<136u, 10u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<136u, 11u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<136u, 12u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<136u, 13u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<136u, 14u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<136u, 15u> {
    static const uint32_t WORD = 2880u;
};
// Tile 137: a_segments = b_segments = {16,17,24,25}.
template<> struct TileMeta<137u> {
    static const uint32_t SEGMENT_MASK = 0x03030000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<137u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<137u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<137u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<137u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<137u, 4u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<137u, 5u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<137u, 6u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<137u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 138: a_segments = b_segments = {18,26}.
template<> struct TileMeta<138u> {
    static const uint32_t SEGMENT_MASK = 0x04040000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<138u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<138u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<138u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<138u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<138u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<138u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<138u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<138u, 7u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<138u, 8u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<138u, 9u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<138u, 10u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<138u, 11u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<138u, 12u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<138u, 13u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<138u, 14u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<138u, 15u> {
    static const uint32_t WORD = 2940u;
};
// Tile 139: a_segments = b_segments = {19,27}.
template<> struct TileMeta<139u> {
    static const uint32_t SEGMENT_MASK = 0x08080000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<139u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<139u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<139u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<139u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<139u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<139u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<139u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<139u, 7u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<139u, 8u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<139u, 9u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<139u, 10u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<139u, 11u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<139u, 12u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<139u, 13u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<139u, 14u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<139u, 15u> {
    static const uint32_t WORD = 3000u;
};
// Tile 140: a_segments = b_segments = {18,19,26,27}.
template<> struct TileMeta<140u> {
    static const uint32_t SEGMENT_MASK = 0x0c0c0000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<140u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<140u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<140u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<140u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<140u, 4u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<140u, 5u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<140u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<140u, 7u> {
    static const uint32_t WORD = 2940u;
};
// Tile 141: a_segments = b_segments = {16,18,24,26}.
template<> struct TileMeta<141u> {
    static const uint32_t SEGMENT_MASK = 0x05050000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<141u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<141u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<141u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<141u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<141u, 4u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<141u, 5u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<141u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<141u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 142: a_segments = b_segments = {17,19,25,27}.
template<> struct TileMeta<142u> {
    static const uint32_t SEGMENT_MASK = 0x0a0a0000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<142u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<142u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<142u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<142u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<142u, 4u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<142u, 5u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<142u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<142u, 7u> {
    static const uint32_t WORD = 2880u;
};
// Tile 143: a_segments = b_segments = {16,17,18,19,24,25,26,27}.
template<> struct TileMeta<143u> {
    static const uint32_t SEGMENT_MASK = 0x0f0f0000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<143u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<143u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<143u, 2u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<143u, 3u> {
    static const uint32_t WORD = 2820u;
};
// Tile 144: a_segments = b_segments = {20,28}.
template<> struct TileMeta<144u> {
    static const uint32_t SEGMENT_MASK = 0x10100000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<144u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<144u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<144u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<144u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<144u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<144u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<144u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<144u, 7u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<144u, 8u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<144u, 9u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<144u, 10u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<144u, 11u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<144u, 12u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<144u, 13u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<144u, 14u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<144u, 15u> {
    static const uint32_t WORD = 3060u;
};
// Tile 145: a_segments = b_segments = {21,29}.
template<> struct TileMeta<145u> {
    static const uint32_t SEGMENT_MASK = 0x20200000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<145u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<145u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<145u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<145u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<145u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<145u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<145u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<145u, 7u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<145u, 8u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<145u, 9u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<145u, 10u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<145u, 11u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<145u, 12u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<145u, 13u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<145u, 14u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<145u, 15u> {
    static const uint32_t WORD = 3120u;
};
// Tile 146: a_segments = b_segments = {20,21,28,29}.
template<> struct TileMeta<146u> {
    static const uint32_t SEGMENT_MASK = 0x30300000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<146u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<146u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<146u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<146u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<146u, 4u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<146u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<146u, 6u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<146u, 7u> {
    static const uint32_t WORD = 3060u;
};
// Tile 147: a_segments = b_segments = {22,30}.
template<> struct TileMeta<147u> {
    static const uint32_t SEGMENT_MASK = 0x40400000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<147u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<147u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<147u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<147u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<147u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<147u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<147u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<147u, 7u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<147u, 8u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<147u, 9u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<147u, 10u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<147u, 11u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<147u, 12u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<147u, 13u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<147u, 14u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<147u, 15u> {
    static const uint32_t WORD = 3180u;
};
// Tile 148: a_segments = b_segments = {23,31}.
template<> struct TileMeta<148u> {
    static const uint32_t SEGMENT_MASK = 0x80800000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<148u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<148u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<148u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<148u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<148u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<148u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<148u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<148u, 7u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<148u, 8u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<148u, 9u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<148u, 10u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<148u, 11u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<148u, 12u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<148u, 13u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<148u, 14u> {
    static const uint32_t WORD = 3180u;
};
template<> struct TileOffset<148u, 15u> {
    static const uint32_t WORD = 3240u;
};
// Tile 149: a_segments = b_segments = {22,23,30,31}.
template<> struct TileMeta<149u> {
    static const uint32_t SEGMENT_MASK = 0xc0c00000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<149u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<149u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<149u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<149u, 3u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<149u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<149u, 5u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<149u, 6u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<149u, 7u> {
    static const uint32_t WORD = 3180u;
};
// Tile 150: a_segments = b_segments = {20,22,28,30}.
template<> struct TileMeta<150u> {
    static const uint32_t SEGMENT_MASK = 0x50500000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<150u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<150u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<150u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<150u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<150u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<150u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<150u, 6u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<150u, 7u> {
    static const uint32_t WORD = 3060u;
};
// Tile 151: a_segments = b_segments = {21,23,29,31}.
template<> struct TileMeta<151u> {
    static const uint32_t SEGMENT_MASK = 0xa0a00000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<151u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<151u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<151u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<151u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<151u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<151u, 5u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<151u, 6u> {
    static const uint32_t WORD = 3060u;
};
template<> struct TileOffset<151u, 7u> {
    static const uint32_t WORD = 3120u;
};
// Tile 152: a_segments = b_segments = {20,21,22,23,28,29,30,31}.
template<> struct TileMeta<152u> {
    static const uint32_t SEGMENT_MASK = 0xf0f00000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<152u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<152u, 1u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<152u, 2u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<152u, 3u> {
    static const uint32_t WORD = 3060u;
};
// Tile 153: a_segments = b_segments = {16,20,24,28}.
template<> struct TileMeta<153u> {
    static const uint32_t SEGMENT_MASK = 0x11110000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<153u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<153u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<153u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<153u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<153u, 4u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<153u, 5u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<153u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<153u, 7u> {
    static const uint32_t WORD = 2820u;
};
// Tile 154: a_segments = b_segments = {17,21,25,29}.
template<> struct TileMeta<154u> {
    static const uint32_t SEGMENT_MASK = 0x22220000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<154u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<154u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<154u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<154u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<154u, 4u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<154u, 5u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<154u, 6u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<154u, 7u> {
    static const uint32_t WORD = 2880u;
};
// Tile 155: a_segments = b_segments = {16,17,20,21,24,25,28,29}.
template<> struct TileMeta<155u> {
    static const uint32_t SEGMENT_MASK = 0x33330000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<155u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<155u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<155u, 2u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<155u, 3u> {
    static const uint32_t WORD = 2820u;
};
// Tile 156: a_segments = b_segments = {18,22,26,30}.
template<> struct TileMeta<156u> {
    static const uint32_t SEGMENT_MASK = 0x44440000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<156u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<156u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<156u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<156u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<156u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<156u, 5u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<156u, 6u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<156u, 7u> {
    static const uint32_t WORD = 2940u;
};
// Tile 157: a_segments = b_segments = {19,23,27,31}.
template<> struct TileMeta<157u> {
    static const uint32_t SEGMENT_MASK = 0x88880000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<157u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<157u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<157u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<157u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<157u, 4u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<157u, 5u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<157u, 6u> {
    static const uint32_t WORD = 2940u;
};
template<> struct TileOffset<157u, 7u> {
    static const uint32_t WORD = 3000u;
};
// Tile 158: a_segments = b_segments = {18,19,22,23,26,27,30,31}.
template<> struct TileMeta<158u> {
    static const uint32_t SEGMENT_MASK = 0xcccc0000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<158u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<158u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<158u, 2u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<158u, 3u> {
    static const uint32_t WORD = 2940u;
};
// Tile 159: a_segments = b_segments = {16,18,20,22,24,26,28,30}.
template<> struct TileMeta<159u> {
    static const uint32_t SEGMENT_MASK = 0x55550000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<159u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<159u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<159u, 2u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<159u, 3u> {
    static const uint32_t WORD = 2820u;
};
// Tile 160: a_segments = b_segments = {17,19,21,23,25,27,29,31}.
template<> struct TileMeta<160u> {
    static const uint32_t SEGMENT_MASK = 0xaaaa0000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<160u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<160u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<160u, 2u> {
    static const uint32_t WORD = 2820u;
};
template<> struct TileOffset<160u, 3u> {
    static const uint32_t WORD = 2880u;
};
// Tile 161: a_segments = b_segments = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}.
template<> struct TileMeta<161u> {
    static const uint32_t SEGMENT_MASK = 0xffff0000u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<161u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<161u, 1u> {
    static const uint32_t WORD = 2820u;
};
// Tile 162: a_segments = b_segments = {0,16}.
template<> struct TileMeta<162u> {
    static const uint32_t SEGMENT_MASK = 0x00010001u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<162u, 0u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<162u, 1u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<162u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<162u, 3u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<162u, 4u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<162u, 5u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<162u, 6u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<162u, 7u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<162u, 8u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<162u, 9u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<162u, 10u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<162u, 11u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<162u, 12u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<162u, 13u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<162u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<162u, 15u> {
    static const uint32_t WORD = 1860u;
};
// Tile 163: a_segments = b_segments = {1,17}.
template<> struct TileMeta<163u> {
    static const uint32_t SEGMENT_MASK = 0x00020002u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<163u, 0u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<163u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<163u, 2u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<163u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<163u, 4u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<163u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<163u, 6u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<163u, 7u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<163u, 8u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<163u, 9u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<163u, 10u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<163u, 11u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<163u, 12u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<163u, 13u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<163u, 14u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<163u, 15u> {
    static const uint32_t WORD = 1920u;
};
// Tile 164: a_segments = b_segments = {0,1,16,17}.
template<> struct TileMeta<164u> {
    static const uint32_t SEGMENT_MASK = 0x00030003u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<164u, 0u> {
    static const uint32_t WORD = 1020u;
};
template<> struct TileOffset<164u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<164u, 2u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<164u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<164u, 4u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<164u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<164u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<164u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 165: a_segments = b_segments = {2,18}.
template<> struct TileMeta<165u> {
    static const uint32_t SEGMENT_MASK = 0x00040004u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<165u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<165u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<165u, 2u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<165u, 3u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<165u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<165u, 5u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<165u, 6u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<165u, 7u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<165u, 8u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<165u, 9u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<165u, 10u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<165u, 11u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<165u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<165u, 13u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<165u, 14u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<165u, 15u> {
    static const uint32_t WORD = 1980u;
};
// Tile 166: a_segments = b_segments = {3,19}.
template<> struct TileMeta<166u> {
    static const uint32_t SEGMENT_MASK = 0x00080008u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<166u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<166u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<166u, 2u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<166u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<166u, 4u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<166u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<166u, 6u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<166u, 7u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<166u, 8u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<166u, 9u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<166u, 10u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<166u, 11u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<166u, 12u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<166u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<166u, 14u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<166u, 15u> {
    static const uint32_t WORD = 2040u;
};
// Tile 167: a_segments = b_segments = {2,3,18,19}.
template<> struct TileMeta<167u> {
    static const uint32_t SEGMENT_MASK = 0x000c000cu;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<167u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<167u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<167u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<167u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<167u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<167u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<167u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<167u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 168: a_segments = b_segments = {0,2,16,18}.
template<> struct TileMeta<168u> {
    static const uint32_t SEGMENT_MASK = 0x00050005u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<168u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<168u, 1u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<168u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<168u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<168u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<168u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<168u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<168u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 169: a_segments = b_segments = {1,3,17,19}.
template<> struct TileMeta<169u> {
    static const uint32_t SEGMENT_MASK = 0x000a000au;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<169u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<169u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<169u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<169u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<169u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<169u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<169u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<169u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 170: a_segments = b_segments = {0,1,2,3,16,17,18,19}.
template<> struct TileMeta<170u> {
    static const uint32_t SEGMENT_MASK = 0x000f000fu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<170u, 0u> {
    static const uint32_t WORD = 1140u;
};
template<> struct TileOffset<170u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<170u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<170u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 171: a_segments = b_segments = {4,20}.
template<> struct TileMeta<171u> {
    static const uint32_t SEGMENT_MASK = 0x00100010u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<171u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<171u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<171u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<171u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<171u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<171u, 5u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<171u, 6u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<171u, 7u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<171u, 8u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<171u, 9u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<171u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<171u, 11u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<171u, 12u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<171u, 13u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<171u, 14u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<171u, 15u> {
    static const uint32_t WORD = 2100u;
};
// Tile 172: a_segments = b_segments = {5,21}.
template<> struct TileMeta<172u> {
    static const uint32_t SEGMENT_MASK = 0x00200020u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<172u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<172u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<172u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<172u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<172u, 4u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<172u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<172u, 6u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<172u, 7u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<172u, 8u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<172u, 9u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<172u, 10u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<172u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<172u, 12u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<172u, 13u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<172u, 14u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<172u, 15u> {
    static const uint32_t WORD = 2160u;
};
// Tile 173: a_segments = b_segments = {4,5,20,21}.
template<> struct TileMeta<173u> {
    static const uint32_t SEGMENT_MASK = 0x00300030u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<173u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<173u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<173u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<173u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<173u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<173u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<173u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<173u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 174: a_segments = b_segments = {6,22}.
template<> struct TileMeta<174u> {
    static const uint32_t SEGMENT_MASK = 0x00400040u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<174u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<174u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<174u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<174u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<174u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<174u, 5u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<174u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<174u, 7u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<174u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<174u, 9u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<174u, 10u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<174u, 11u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<174u, 12u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<174u, 13u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<174u, 14u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<174u, 15u> {
    static const uint32_t WORD = 2220u;
};
// Tile 175: a_segments = b_segments = {7,23}.
template<> struct TileMeta<175u> {
    static const uint32_t SEGMENT_MASK = 0x00800080u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<175u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<175u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<175u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<175u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<175u, 4u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<175u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<175u, 6u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<175u, 7u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<175u, 8u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<175u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<175u, 10u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<175u, 11u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<175u, 12u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<175u, 13u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<175u, 14u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<175u, 15u> {
    static const uint32_t WORD = 2280u;
};
// Tile 176: a_segments = b_segments = {6,7,22,23}.
template<> struct TileMeta<176u> {
    static const uint32_t SEGMENT_MASK = 0x00c000c0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<176u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<176u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<176u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<176u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<176u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<176u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<176u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<176u, 7u> {
    static const uint32_t WORD = 2220u;
};
// Tile 177: a_segments = b_segments = {4,6,20,22}.
template<> struct TileMeta<177u> {
    static const uint32_t SEGMENT_MASK = 0x00500050u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<177u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<177u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<177u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<177u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<177u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<177u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<177u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<177u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 178: a_segments = b_segments = {5,7,21,23}.
template<> struct TileMeta<178u> {
    static const uint32_t SEGMENT_MASK = 0x00a000a0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<178u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<178u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<178u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<178u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<178u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<178u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<178u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<178u, 7u> {
    static const uint32_t WORD = 2160u;
};
// Tile 179: a_segments = b_segments = {4,5,6,7,20,21,22,23}.
template<> struct TileMeta<179u> {
    static const uint32_t SEGMENT_MASK = 0x00f000f0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<179u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<179u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<179u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<179u, 3u> {
    static const uint32_t WORD = 2100u;
};
// Tile 180: a_segments = b_segments = {0,4,16,20}.
template<> struct TileMeta<180u> {
    static const uint32_t SEGMENT_MASK = 0x00110011u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<180u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<180u, 1u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<180u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<180u, 3u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<180u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<180u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<180u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<180u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 181: a_segments = b_segments = {1,5,17,21}.
template<> struct TileMeta<181u> {
    static const uint32_t SEGMENT_MASK = 0x00220022u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<181u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<181u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<181u, 2u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<181u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<181u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<181u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<181u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<181u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 182: a_segments = b_segments = {0,1,4,5,16,17,20,21}.
template<> struct TileMeta<182u> {
    static const uint32_t SEGMENT_MASK = 0x00330033u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<182u, 0u> {
    static const uint32_t WORD = 1260u;
};
template<> struct TileOffset<182u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<182u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<182u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 183: a_segments = b_segments = {2,6,18,22}.
template<> struct TileMeta<183u> {
    static const uint32_t SEGMENT_MASK = 0x00440044u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<183u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<183u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<183u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<183u, 3u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<183u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<183u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<183u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<183u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 184: a_segments = b_segments = {3,7,19,23}.
template<> struct TileMeta<184u> {
    static const uint32_t SEGMENT_MASK = 0x00880088u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<184u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<184u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<184u, 2u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<184u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<184u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<184u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<184u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<184u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 185: a_segments = b_segments = {2,3,6,7,18,19,22,23}.
template<> struct TileMeta<185u> {
    static const uint32_t SEGMENT_MASK = 0x00cc00ccu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<185u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<185u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<185u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<185u, 3u> {
    static const uint32_t WORD = 1980u;
};
// Tile 186: a_segments = b_segments = {0,2,4,6,16,18,20,22}.
template<> struct TileMeta<186u> {
    static const uint32_t SEGMENT_MASK = 0x00550055u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<186u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<186u, 1u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<186u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<186u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 187: a_segments = b_segments = {1,3,5,7,17,19,21,23}.
template<> struct TileMeta<187u> {
    static const uint32_t SEGMENT_MASK = 0x00aa00aau;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<187u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<187u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<187u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<187u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 188: a_segments = b_segments = {0,1,2,3,4,5,6,7,16,17,18,19,20,21,22,23}.
template<> struct TileMeta<188u> {
    static const uint32_t SEGMENT_MASK = 0x00ff00ffu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<188u, 0u> {
    static const uint32_t WORD = 1380u;
};
template<> struct TileOffset<188u, 1u> {
    static const uint32_t WORD = 1860u;
};
// Tile 189: a_segments = b_segments = {8,24}.
template<> struct TileMeta<189u> {
    static const uint32_t SEGMENT_MASK = 0x01000100u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<189u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<189u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<189u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<189u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<189u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<189u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<189u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<189u, 7u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<189u, 8u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<189u, 9u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<189u, 10u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<189u, 11u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<189u, 12u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<189u, 13u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<189u, 14u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<189u, 15u> {
    static const uint32_t WORD = 2340u;
};
// Tile 190: a_segments = b_segments = {9,25}.
template<> struct TileMeta<190u> {
    static const uint32_t SEGMENT_MASK = 0x02000200u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<190u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<190u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<190u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<190u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<190u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<190u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<190u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<190u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<190u, 8u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<190u, 9u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<190u, 10u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<190u, 11u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<190u, 12u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<190u, 13u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<190u, 14u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<190u, 15u> {
    static const uint32_t WORD = 2400u;
};
// Tile 191: a_segments = b_segments = {8,9,24,25}.
template<> struct TileMeta<191u> {
    static const uint32_t SEGMENT_MASK = 0x03000300u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<191u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<191u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<191u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<191u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<191u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<191u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<191u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<191u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 192: a_segments = b_segments = {10,26}.
template<> struct TileMeta<192u> {
    static const uint32_t SEGMENT_MASK = 0x04000400u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<192u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<192u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<192u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<192u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<192u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<192u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<192u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<192u, 7u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<192u, 8u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<192u, 9u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<192u, 10u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<192u, 11u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<192u, 12u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<192u, 13u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<192u, 14u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<192u, 15u> {
    static const uint32_t WORD = 2460u;
};
// Tile 193: a_segments = b_segments = {11,27}.
template<> struct TileMeta<193u> {
    static const uint32_t SEGMENT_MASK = 0x08000800u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<193u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<193u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<193u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<193u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<193u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<193u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<193u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<193u, 7u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<193u, 8u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<193u, 9u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<193u, 10u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<193u, 11u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<193u, 12u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<193u, 13u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<193u, 14u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<193u, 15u> {
    static const uint32_t WORD = 2520u;
};
// Tile 194: a_segments = b_segments = {10,11,26,27}.
template<> struct TileMeta<194u> {
    static const uint32_t SEGMENT_MASK = 0x0c000c00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<194u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<194u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<194u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<194u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<194u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<194u, 5u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<194u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<194u, 7u> {
    static const uint32_t WORD = 2460u;
};
// Tile 195: a_segments = b_segments = {8,10,24,26}.
template<> struct TileMeta<195u> {
    static const uint32_t SEGMENT_MASK = 0x05000500u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<195u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<195u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<195u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<195u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<195u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<195u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<195u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<195u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 196: a_segments = b_segments = {9,11,25,27}.
template<> struct TileMeta<196u> {
    static const uint32_t SEGMENT_MASK = 0x0a000a00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<196u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<196u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<196u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<196u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<196u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<196u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<196u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<196u, 7u> {
    static const uint32_t WORD = 2400u;
};
// Tile 197: a_segments = b_segments = {8,9,10,11,24,25,26,27}.
template<> struct TileMeta<197u> {
    static const uint32_t SEGMENT_MASK = 0x0f000f00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<197u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<197u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<197u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<197u, 3u> {
    static const uint32_t WORD = 2340u;
};
// Tile 198: a_segments = b_segments = {12,28}.
template<> struct TileMeta<198u> {
    static const uint32_t SEGMENT_MASK = 0x10001000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<198u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<198u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<198u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<198u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<198u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<198u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<198u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<198u, 7u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<198u, 8u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<198u, 9u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<198u, 10u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<198u, 11u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<198u, 12u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<198u, 13u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<198u, 14u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<198u, 15u> {
    static const uint32_t WORD = 2580u;
};
// Tile 199: a_segments = b_segments = {13,29}.
template<> struct TileMeta<199u> {
    static const uint32_t SEGMENT_MASK = 0x20002000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<199u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<199u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<199u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<199u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<199u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<199u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<199u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<199u, 7u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<199u, 8u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<199u, 9u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<199u, 10u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<199u, 11u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<199u, 12u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<199u, 13u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<199u, 14u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<199u, 15u> {
    static const uint32_t WORD = 2640u;
};
// Tile 200: a_segments = b_segments = {12,13,28,29}.
template<> struct TileMeta<200u> {
    static const uint32_t SEGMENT_MASK = 0x30003000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<200u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<200u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<200u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<200u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<200u, 4u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<200u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<200u, 6u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<200u, 7u> {
    static const uint32_t WORD = 2580u;
};
// Tile 201: a_segments = b_segments = {14,30}.
template<> struct TileMeta<201u> {
    static const uint32_t SEGMENT_MASK = 0x40004000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<201u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<201u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<201u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<201u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<201u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<201u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<201u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<201u, 7u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<201u, 8u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<201u, 9u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<201u, 10u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<201u, 11u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<201u, 12u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<201u, 13u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<201u, 14u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<201u, 15u> {
    static const uint32_t WORD = 2700u;
};
// Tile 202: a_segments = b_segments = {15,31}.
template<> struct TileMeta<202u> {
    static const uint32_t SEGMENT_MASK = 0x80008000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<202u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<202u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<202u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<202u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<202u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<202u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<202u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<202u, 7u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<202u, 8u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<202u, 9u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<202u, 10u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<202u, 11u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<202u, 12u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<202u, 13u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<202u, 14u> {
    static const uint32_t WORD = 2700u;
};
template<> struct TileOffset<202u, 15u> {
    static const uint32_t WORD = 2760u;
};
// Tile 203: a_segments = b_segments = {14,15,30,31}.
template<> struct TileMeta<203u> {
    static const uint32_t SEGMENT_MASK = 0xc000c000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<203u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<203u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<203u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<203u, 3u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<203u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<203u, 5u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<203u, 6u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<203u, 7u> {
    static const uint32_t WORD = 2700u;
};
// Tile 204: a_segments = b_segments = {12,14,28,30}.
template<> struct TileMeta<204u> {
    static const uint32_t SEGMENT_MASK = 0x50005000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<204u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<204u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<204u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<204u, 3u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<204u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<204u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<204u, 6u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<204u, 7u> {
    static const uint32_t WORD = 2580u;
};
// Tile 205: a_segments = b_segments = {13,15,29,31}.
template<> struct TileMeta<205u> {
    static const uint32_t SEGMENT_MASK = 0xa000a000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<205u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<205u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<205u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<205u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<205u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<205u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<205u, 6u> {
    static const uint32_t WORD = 2580u;
};
template<> struct TileOffset<205u, 7u> {
    static const uint32_t WORD = 2640u;
};
// Tile 206: a_segments = b_segments = {12,13,14,15,28,29,30,31}.
template<> struct TileMeta<206u> {
    static const uint32_t SEGMENT_MASK = 0xf000f000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<206u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<206u, 1u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<206u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<206u, 3u> {
    static const uint32_t WORD = 2580u;
};
// Tile 207: a_segments = b_segments = {8,12,24,28}.
template<> struct TileMeta<207u> {
    static const uint32_t SEGMENT_MASK = 0x11001100u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<207u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<207u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<207u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<207u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<207u, 4u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<207u, 5u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<207u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<207u, 7u> {
    static const uint32_t WORD = 2340u;
};
// Tile 208: a_segments = b_segments = {9,13,25,29}.
template<> struct TileMeta<208u> {
    static const uint32_t SEGMENT_MASK = 0x22002200u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<208u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<208u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<208u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<208u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<208u, 4u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<208u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<208u, 6u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<208u, 7u> {
    static const uint32_t WORD = 2400u;
};
// Tile 209: a_segments = b_segments = {8,9,12,13,24,25,28,29}.
template<> struct TileMeta<209u> {
    static const uint32_t SEGMENT_MASK = 0x33003300u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<209u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<209u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<209u, 2u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<209u, 3u> {
    static const uint32_t WORD = 2340u;
};
// Tile 210: a_segments = b_segments = {10,14,26,30}.
template<> struct TileMeta<210u> {
    static const uint32_t SEGMENT_MASK = 0x44004400u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<210u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<210u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<210u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<210u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<210u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<210u, 5u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<210u, 6u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<210u, 7u> {
    static const uint32_t WORD = 2460u;
};
// Tile 211: a_segments = b_segments = {11,15,27,31}.
template<> struct TileMeta<211u> {
    static const uint32_t SEGMENT_MASK = 0x88008800u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<211u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<211u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<211u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<211u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<211u, 4u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<211u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<211u, 6u> {
    static const uint32_t WORD = 2460u;
};
template<> struct TileOffset<211u, 7u> {
    static const uint32_t WORD = 2520u;
};
// Tile 212: a_segments = b_segments = {10,11,14,15,26,27,30,31}.
template<> struct TileMeta<212u> {
    static const uint32_t SEGMENT_MASK = 0xcc00cc00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<212u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<212u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<212u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<212u, 3u> {
    static const uint32_t WORD = 2460u;
};
// Tile 213: a_segments = b_segments = {8,10,12,14,24,26,28,30}.
template<> struct TileMeta<213u> {
    static const uint32_t SEGMENT_MASK = 0x55005500u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<213u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<213u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<213u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<213u, 3u> {
    static const uint32_t WORD = 2340u;
};
// Tile 214: a_segments = b_segments = {9,11,13,15,25,27,29,31}.
template<> struct TileMeta<214u> {
    static const uint32_t SEGMENT_MASK = 0xaa00aa00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<214u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<214u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<214u, 2u> {
    static const uint32_t WORD = 2340u;
};
template<> struct TileOffset<214u, 3u> {
    static const uint32_t WORD = 2400u;
};
// Tile 215: a_segments = b_segments = {8,9,10,11,12,13,14,15,24,25,26,27,28,29,30,31}.
template<> struct TileMeta<215u> {
    static const uint32_t SEGMENT_MASK = 0xff00ff00u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<215u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<215u, 1u> {
    static const uint32_t WORD = 2340u;
};
// Tile 216: a_segments = b_segments = {0,8,16,24}.
template<> struct TileMeta<216u> {
    static const uint32_t SEGMENT_MASK = 0x01010101u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<216u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<216u, 1u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<216u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<216u, 3u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<216u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<216u, 5u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<216u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<216u, 7u> {
    static const uint32_t WORD = 1860u;
};
// Tile 217: a_segments = b_segments = {1,9,17,25}.
template<> struct TileMeta<217u> {
    static const uint32_t SEGMENT_MASK = 0x02020202u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<217u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<217u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<217u, 2u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<217u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<217u, 4u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<217u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<217u, 6u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<217u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 218: a_segments = b_segments = {0,1,8,9,16,17,24,25}.
template<> struct TileMeta<218u> {
    static const uint32_t SEGMENT_MASK = 0x03030303u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<218u, 0u> {
    static const uint32_t WORD = 1500u;
};
template<> struct TileOffset<218u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<218u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<218u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 219: a_segments = b_segments = {2,10,18,26}.
template<> struct TileMeta<219u> {
    static const uint32_t SEGMENT_MASK = 0x04040404u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<219u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<219u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<219u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<219u, 3u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<219u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<219u, 5u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<219u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<219u, 7u> {
    static const uint32_t WORD = 1980u;
};
// Tile 220: a_segments = b_segments = {3,11,19,27}.
template<> struct TileMeta<220u> {
    static const uint32_t SEGMENT_MASK = 0x08080808u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<220u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<220u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<220u, 2u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<220u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<220u, 4u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<220u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<220u, 6u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<220u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 221: a_segments = b_segments = {2,3,10,11,18,19,26,27}.
template<> struct TileMeta<221u> {
    static const uint32_t SEGMENT_MASK = 0x0c0c0c0cu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<221u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<221u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<221u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<221u, 3u> {
    static const uint32_t WORD = 1980u;
};
// Tile 222: a_segments = b_segments = {0,2,8,10,16,18,24,26}.
template<> struct TileMeta<222u> {
    static const uint32_t SEGMENT_MASK = 0x05050505u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<222u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<222u, 1u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<222u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<222u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 223: a_segments = b_segments = {1,3,9,11,17,19,25,27}.
template<> struct TileMeta<223u> {
    static const uint32_t SEGMENT_MASK = 0x0a0a0a0au;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<223u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<223u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<223u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<223u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 224: a_segments = b_segments = {0,1,2,3,8,9,10,11,16,17,18,19,24,25,26,27}.
template<> struct TileMeta<224u> {
    static const uint32_t SEGMENT_MASK = 0x0f0f0f0fu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<224u, 0u> {
    static const uint32_t WORD = 1620u;
};
template<> struct TileOffset<224u, 1u> {
    static const uint32_t WORD = 1860u;
};
// Tile 225: a_segments = b_segments = {4,12,20,28}.
template<> struct TileMeta<225u> {
    static const uint32_t SEGMENT_MASK = 0x10101010u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<225u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<225u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<225u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<225u, 3u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<225u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<225u, 5u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<225u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<225u, 7u> {
    static const uint32_t WORD = 2100u;
};
// Tile 226: a_segments = b_segments = {5,13,21,29}.
template<> struct TileMeta<226u> {
    static const uint32_t SEGMENT_MASK = 0x20202020u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<226u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<226u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<226u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<226u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<226u, 4u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<226u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<226u, 6u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<226u, 7u> {
    static const uint32_t WORD = 2160u;
};
// Tile 227: a_segments = b_segments = {4,5,12,13,20,21,28,29}.
template<> struct TileMeta<227u> {
    static const uint32_t SEGMENT_MASK = 0x30303030u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<227u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<227u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<227u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<227u, 3u> {
    static const uint32_t WORD = 2100u;
};
// Tile 228: a_segments = b_segments = {6,14,22,30}.
template<> struct TileMeta<228u> {
    static const uint32_t SEGMENT_MASK = 0x40404040u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<228u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<228u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<228u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<228u, 3u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<228u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<228u, 5u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<228u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<228u, 7u> {
    static const uint32_t WORD = 2220u;
};
// Tile 229: a_segments = b_segments = {7,15,23,31}.
template<> struct TileMeta<229u> {
    static const uint32_t SEGMENT_MASK = 0x80808080u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<229u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<229u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<229u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<229u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<229u, 4u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<229u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<229u, 6u> {
    static const uint32_t WORD = 2220u;
};
template<> struct TileOffset<229u, 7u> {
    static const uint32_t WORD = 2280u;
};
// Tile 230: a_segments = b_segments = {6,7,14,15,22,23,30,31}.
template<> struct TileMeta<230u> {
    static const uint32_t SEGMENT_MASK = 0xc0c0c0c0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<230u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<230u, 1u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<230u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<230u, 3u> {
    static const uint32_t WORD = 2220u;
};
// Tile 231: a_segments = b_segments = {4,6,12,14,20,22,28,30}.
template<> struct TileMeta<231u> {
    static const uint32_t SEGMENT_MASK = 0x50505050u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<231u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<231u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<231u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<231u, 3u> {
    static const uint32_t WORD = 2100u;
};
// Tile 232: a_segments = b_segments = {5,7,13,15,21,23,29,31}.
template<> struct TileMeta<232u> {
    static const uint32_t SEGMENT_MASK = 0xa0a0a0a0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<232u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<232u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<232u, 2u> {
    static const uint32_t WORD = 2100u;
};
template<> struct TileOffset<232u, 3u> {
    static const uint32_t WORD = 2160u;
};
// Tile 233: a_segments = b_segments = {4,5,6,7,12,13,14,15,20,21,22,23,28,29,30,31}.
template<> struct TileMeta<233u> {
    static const uint32_t SEGMENT_MASK = 0xf0f0f0f0u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<233u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<233u, 1u> {
    static const uint32_t WORD = 2100u;
};
// Tile 234: a_segments = b_segments = {0,4,8,12,16,20,24,28}.
template<> struct TileMeta<234u> {
    static const uint32_t SEGMENT_MASK = 0x11111111u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<234u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<234u, 1u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<234u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<234u, 3u> {
    static const uint32_t WORD = 1860u;
};
// Tile 235: a_segments = b_segments = {1,5,9,13,17,21,25,29}.
template<> struct TileMeta<235u> {
    static const uint32_t SEGMENT_MASK = 0x22222222u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<235u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<235u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<235u, 2u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<235u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 236: a_segments = b_segments = {0,1,4,5,8,9,12,13,16,17,20,21,24,25,28,29}.
template<> struct TileMeta<236u> {
    static const uint32_t SEGMENT_MASK = 0x33333333u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<236u, 0u> {
    static const uint32_t WORD = 1740u;
};
template<> struct TileOffset<236u, 1u> {
    static const uint32_t WORD = 1860u;
};
// Tile 237: a_segments = b_segments = {2,6,10,14,18,22,26,30}.
template<> struct TileMeta<237u> {
    static const uint32_t SEGMENT_MASK = 0x44444444u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<237u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<237u, 1u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<237u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<237u, 3u> {
    static const uint32_t WORD = 1980u;
};
// Tile 238: a_segments = b_segments = {3,7,11,15,19,23,27,31}.
template<> struct TileMeta<238u> {
    static const uint32_t SEGMENT_MASK = 0x88888888u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<238u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<238u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<238u, 2u> {
    static const uint32_t WORD = 1980u;
};
template<> struct TileOffset<238u, 3u> {
    static const uint32_t WORD = 2040u;
};
// Tile 239: a_segments = b_segments = {2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31}.
template<> struct TileMeta<239u> {
    static const uint32_t SEGMENT_MASK = 0xccccccccu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<239u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<239u, 1u> {
    static const uint32_t WORD = 1980u;
};
// Tile 240: a_segments = b_segments = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30}.
template<> struct TileMeta<240u> {
    static const uint32_t SEGMENT_MASK = 0x55555555u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<240u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<240u, 1u> {
    static const uint32_t WORD = 1860u;
};
// Tile 241: a_segments = b_segments = {1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31}.
template<> struct TileMeta<241u> {
    static const uint32_t SEGMENT_MASK = 0xaaaaaaaau;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<241u, 0u> {
    static const uint32_t WORD = 1860u;
};
template<> struct TileOffset<241u, 1u> {
    static const uint32_t WORD = 1920u;
};
// Tile 242: a_segments = b_segments = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}.
template<> struct TileMeta<242u> {
    static const uint32_t SEGMENT_MASK = 0xffffffffu;
    static const uint32_t OFFSET_COUNT = 1u;
};
template<> struct TileOffset<242u, 0u> {
    static const uint32_t WORD = 1860u;
};
} // namespace kara
#endif
