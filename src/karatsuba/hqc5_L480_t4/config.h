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
static const uint32_t SPLIT = 4u;
static const uint32_t G = 81u;
static const uint32_t SUB_WORDS = 120u;
static const uint32_t PRODUCT_WORDS = 240u;
static const uint32_t F_WORDS = 3840u;
static const uint32_t SCRATCH_WORDS = 420u;
static const uint32_t LEAF_WORDS = 15u;
static const uint32_t CONTRIBUTION_WORDS = 0u;
static const uint32_t LOCAL_LEVELS = 3u;
static const uint32_t INPUT_SEGMENTS = 16u;
static const uint32_t WORK_TILE_BYTES = 3600u;
static const uint32_t FOLD_TILE_BYTES = 22568u;
static const uint32_t AIE_WORK_TILE_BYTES = 9744u;
static const uint32_t AIE_FOLD_TILE_BYTES = 26664u;
template<uint32_t TILE> struct TileMeta;
template<uint32_t TILE, uint32_t INDEX> struct TileOffset;
// Tile 0: a_segments = b_segments = {0}.
template<> struct TileMeta<0u> {
    static const uint32_t SEGMENT_MASK = 0x00000001u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<0u, 0u> {
    static const uint32_t WORD = 0u;
};
template<> struct TileOffset<0u, 1u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<0u, 2u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<0u, 3u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<0u, 4u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<0u, 5u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<0u, 6u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<0u, 7u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<0u, 8u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<0u, 9u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<0u, 10u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<0u, 11u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<0u, 12u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<0u, 13u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<0u, 14u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<0u, 15u> {
    static const uint32_t WORD = 1800u;
};
// Tile 1: a_segments = b_segments = {1}.
template<> struct TileMeta<1u> {
    static const uint32_t SEGMENT_MASK = 0x00000002u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<1u, 0u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<1u, 1u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<1u, 2u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<1u, 3u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<1u, 4u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<1u, 5u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<1u, 6u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<1u, 7u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<1u, 8u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<1u, 9u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<1u, 10u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<1u, 11u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<1u, 12u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<1u, 13u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<1u, 14u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<1u, 15u> {
    static const uint32_t WORD = 1920u;
};
// Tile 2: a_segments = b_segments = {0,1}.
template<> struct TileMeta<2u> {
    static const uint32_t SEGMENT_MASK = 0x00000003u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<2u, 0u> {
    static const uint32_t WORD = 120u;
};
template<> struct TileOffset<2u, 1u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<2u, 2u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<2u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<2u, 4u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<2u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<2u, 6u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<2u, 7u> {
    static const uint32_t WORD = 1800u;
};
// Tile 3: a_segments = b_segments = {2}.
template<> struct TileMeta<3u> {
    static const uint32_t SEGMENT_MASK = 0x00000004u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<3u, 0u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<3u, 1u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<3u, 2u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<3u, 3u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<3u, 4u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<3u, 5u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<3u, 6u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<3u, 7u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<3u, 8u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<3u, 9u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<3u, 10u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<3u, 11u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<3u, 12u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<3u, 13u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<3u, 14u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<3u, 15u> {
    static const uint32_t WORD = 2040u;
};
// Tile 4: a_segments = b_segments = {3}.
template<> struct TileMeta<4u> {
    static const uint32_t SEGMENT_MASK = 0x00000008u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<4u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<4u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<4u, 2u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<4u, 3u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<4u, 4u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<4u, 5u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<4u, 6u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<4u, 7u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<4u, 8u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<4u, 9u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<4u, 10u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<4u, 11u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<4u, 12u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<4u, 13u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<4u, 14u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<4u, 15u> {
    static const uint32_t WORD = 2160u;
};
// Tile 5: a_segments = b_segments = {2,3}.
template<> struct TileMeta<5u> {
    static const uint32_t SEGMENT_MASK = 0x0000000cu;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<5u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<5u, 1u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<5u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<5u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<5u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<5u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<5u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<5u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 6: a_segments = b_segments = {0,2}.
template<> struct TileMeta<6u> {
    static const uint32_t SEGMENT_MASK = 0x00000005u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<6u, 0u> {
    static const uint32_t WORD = 240u;
};
template<> struct TileOffset<6u, 1u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<6u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<6u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<6u, 4u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<6u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<6u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<6u, 7u> {
    static const uint32_t WORD = 1800u;
};
// Tile 7: a_segments = b_segments = {1,3}.
template<> struct TileMeta<7u> {
    static const uint32_t SEGMENT_MASK = 0x0000000au;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<7u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<7u, 1u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<7u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<7u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<7u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<7u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<7u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<7u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 8: a_segments = b_segments = {0,1,2,3}.
template<> struct TileMeta<8u> {
    static const uint32_t SEGMENT_MASK = 0x0000000fu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<8u, 0u> {
    static const uint32_t WORD = 360u;
};
template<> struct TileOffset<8u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<8u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<8u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 9: a_segments = b_segments = {4}.
template<> struct TileMeta<9u> {
    static const uint32_t SEGMENT_MASK = 0x00000010u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<9u, 0u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<9u, 1u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<9u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<9u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<9u, 4u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<9u, 5u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<9u, 6u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<9u, 7u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<9u, 8u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<9u, 9u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<9u, 10u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<9u, 11u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<9u, 12u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<9u, 13u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<9u, 14u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<9u, 15u> {
    static const uint32_t WORD = 2280u;
};
// Tile 10: a_segments = b_segments = {5}.
template<> struct TileMeta<10u> {
    static const uint32_t SEGMENT_MASK = 0x00000020u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<10u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<10u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<10u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<10u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<10u, 4u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<10u, 5u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<10u, 6u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<10u, 7u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<10u, 8u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<10u, 9u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<10u, 10u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<10u, 11u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<10u, 12u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<10u, 13u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<10u, 14u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<10u, 15u> {
    static const uint32_t WORD = 2400u;
};
// Tile 11: a_segments = b_segments = {4,5}.
template<> struct TileMeta<11u> {
    static const uint32_t SEGMENT_MASK = 0x00000030u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<11u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<11u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<11u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<11u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<11u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<11u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<11u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<11u, 7u> {
    static const uint32_t WORD = 2280u;
};
// Tile 12: a_segments = b_segments = {6}.
template<> struct TileMeta<12u> {
    static const uint32_t SEGMENT_MASK = 0x00000040u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<12u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<12u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<12u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<12u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<12u, 4u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<12u, 5u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<12u, 6u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<12u, 7u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<12u, 8u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<12u, 9u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<12u, 10u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<12u, 11u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<12u, 12u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<12u, 13u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<12u, 14u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<12u, 15u> {
    static const uint32_t WORD = 2520u;
};
// Tile 13: a_segments = b_segments = {7}.
template<> struct TileMeta<13u> {
    static const uint32_t SEGMENT_MASK = 0x00000080u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<13u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<13u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<13u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<13u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<13u, 4u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<13u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<13u, 6u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<13u, 7u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<13u, 8u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<13u, 9u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<13u, 10u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<13u, 11u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<13u, 12u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<13u, 13u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<13u, 14u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<13u, 15u> {
    static const uint32_t WORD = 2640u;
};
// Tile 14: a_segments = b_segments = {6,7}.
template<> struct TileMeta<14u> {
    static const uint32_t SEGMENT_MASK = 0x000000c0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<14u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<14u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<14u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<14u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<14u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<14u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<14u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<14u, 7u> {
    static const uint32_t WORD = 2520u;
};
// Tile 15: a_segments = b_segments = {4,6}.
template<> struct TileMeta<15u> {
    static const uint32_t SEGMENT_MASK = 0x00000050u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<15u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<15u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<15u, 2u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<15u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<15u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<15u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<15u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<15u, 7u> {
    static const uint32_t WORD = 2280u;
};
// Tile 16: a_segments = b_segments = {5,7}.
template<> struct TileMeta<16u> {
    static const uint32_t SEGMENT_MASK = 0x000000a0u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<16u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<16u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<16u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<16u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<16u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<16u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<16u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<16u, 7u> {
    static const uint32_t WORD = 2400u;
};
// Tile 17: a_segments = b_segments = {4,5,6,7}.
template<> struct TileMeta<17u> {
    static const uint32_t SEGMENT_MASK = 0x000000f0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<17u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<17u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<17u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<17u, 3u> {
    static const uint32_t WORD = 2280u;
};
// Tile 18: a_segments = b_segments = {0,4}.
template<> struct TileMeta<18u> {
    static const uint32_t SEGMENT_MASK = 0x00000011u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<18u, 0u> {
    static const uint32_t WORD = 480u;
};
template<> struct TileOffset<18u, 1u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<18u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<18u, 3u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<18u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<18u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<18u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<18u, 7u> {
    static const uint32_t WORD = 1800u;
};
// Tile 19: a_segments = b_segments = {1,5}.
template<> struct TileMeta<19u> {
    static const uint32_t SEGMENT_MASK = 0x00000022u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<19u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<19u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<19u, 2u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<19u, 3u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<19u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<19u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<19u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<19u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 20: a_segments = b_segments = {0,1,4,5}.
template<> struct TileMeta<20u> {
    static const uint32_t SEGMENT_MASK = 0x00000033u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<20u, 0u> {
    static const uint32_t WORD = 600u;
};
template<> struct TileOffset<20u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<20u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<20u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 21: a_segments = b_segments = {2,6}.
template<> struct TileMeta<21u> {
    static const uint32_t SEGMENT_MASK = 0x00000044u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<21u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<21u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<21u, 2u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<21u, 3u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<21u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<21u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<21u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<21u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 22: a_segments = b_segments = {3,7}.
template<> struct TileMeta<22u> {
    static const uint32_t SEGMENT_MASK = 0x00000088u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<22u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<22u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<22u, 2u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<22u, 3u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<22u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<22u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<22u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<22u, 7u> {
    static const uint32_t WORD = 2160u;
};
// Tile 23: a_segments = b_segments = {2,3,6,7}.
template<> struct TileMeta<23u> {
    static const uint32_t SEGMENT_MASK = 0x000000ccu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<23u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<23u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<23u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<23u, 3u> {
    static const uint32_t WORD = 2040u;
};
// Tile 24: a_segments = b_segments = {0,2,4,6}.
template<> struct TileMeta<24u> {
    static const uint32_t SEGMENT_MASK = 0x00000055u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<24u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<24u, 1u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<24u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<24u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 25: a_segments = b_segments = {1,3,5,7}.
template<> struct TileMeta<25u> {
    static const uint32_t SEGMENT_MASK = 0x000000aau;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<25u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<25u, 1u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<25u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<25u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 26: a_segments = b_segments = {0,1,2,3,4,5,6,7}.
template<> struct TileMeta<26u> {
    static const uint32_t SEGMENT_MASK = 0x000000ffu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<26u, 0u> {
    static const uint32_t WORD = 840u;
};
template<> struct TileOffset<26u, 1u> {
    static const uint32_t WORD = 1800u;
};
// Tile 27: a_segments = b_segments = {8}.
template<> struct TileMeta<27u> {
    static const uint32_t SEGMENT_MASK = 0x00000100u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<27u, 0u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<27u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<27u, 2u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<27u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<27u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<27u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<27u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<27u, 7u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<27u, 8u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<27u, 9u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<27u, 10u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<27u, 11u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<27u, 12u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<27u, 13u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<27u, 14u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<27u, 15u> {
    static const uint32_t WORD = 2760u;
};
// Tile 28: a_segments = b_segments = {9}.
template<> struct TileMeta<28u> {
    static const uint32_t SEGMENT_MASK = 0x00000200u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<28u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<28u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<28u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<28u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<28u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<28u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<28u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<28u, 7u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<28u, 8u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<28u, 9u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<28u, 10u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<28u, 11u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<28u, 12u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<28u, 13u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<28u, 14u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<28u, 15u> {
    static const uint32_t WORD = 2880u;
};
// Tile 29: a_segments = b_segments = {8,9}.
template<> struct TileMeta<29u> {
    static const uint32_t SEGMENT_MASK = 0x00000300u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<29u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<29u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<29u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<29u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<29u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<29u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<29u, 6u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<29u, 7u> {
    static const uint32_t WORD = 2760u;
};
// Tile 30: a_segments = b_segments = {10}.
template<> struct TileMeta<30u> {
    static const uint32_t SEGMENT_MASK = 0x00000400u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<30u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<30u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<30u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<30u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<30u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<30u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<30u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<30u, 7u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<30u, 8u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<30u, 9u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<30u, 10u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<30u, 11u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<30u, 12u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<30u, 13u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<30u, 14u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<30u, 15u> {
    static const uint32_t WORD = 3000u;
};
// Tile 31: a_segments = b_segments = {11}.
template<> struct TileMeta<31u> {
    static const uint32_t SEGMENT_MASK = 0x00000800u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<31u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<31u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<31u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<31u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<31u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<31u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<31u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<31u, 7u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<31u, 8u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<31u, 9u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<31u, 10u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<31u, 11u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<31u, 12u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<31u, 13u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<31u, 14u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<31u, 15u> {
    static const uint32_t WORD = 3120u;
};
// Tile 32: a_segments = b_segments = {10,11}.
template<> struct TileMeta<32u> {
    static const uint32_t SEGMENT_MASK = 0x00000c00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<32u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<32u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<32u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<32u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<32u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<32u, 5u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<32u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<32u, 7u> {
    static const uint32_t WORD = 3000u;
};
// Tile 33: a_segments = b_segments = {8,10}.
template<> struct TileMeta<33u> {
    static const uint32_t SEGMENT_MASK = 0x00000500u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<33u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<33u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<33u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<33u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<33u, 4u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<33u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<33u, 6u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<33u, 7u> {
    static const uint32_t WORD = 2760u;
};
// Tile 34: a_segments = b_segments = {9,11}.
template<> struct TileMeta<34u> {
    static const uint32_t SEGMENT_MASK = 0x00000a00u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<34u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<34u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<34u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<34u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<34u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<34u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<34u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<34u, 7u> {
    static const uint32_t WORD = 2880u;
};
// Tile 35: a_segments = b_segments = {8,9,10,11}.
template<> struct TileMeta<35u> {
    static const uint32_t SEGMENT_MASK = 0x00000f00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<35u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<35u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<35u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<35u, 3u> {
    static const uint32_t WORD = 2760u;
};
// Tile 36: a_segments = b_segments = {12}.
template<> struct TileMeta<36u> {
    static const uint32_t SEGMENT_MASK = 0x00001000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<36u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<36u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<36u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<36u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<36u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<36u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<36u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<36u, 7u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<36u, 8u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<36u, 9u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<36u, 10u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<36u, 11u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<36u, 12u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<36u, 13u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<36u, 14u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<36u, 15u> {
    static const uint32_t WORD = 3240u;
};
// Tile 37: a_segments = b_segments = {13}.
template<> struct TileMeta<37u> {
    static const uint32_t SEGMENT_MASK = 0x00002000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<37u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<37u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<37u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<37u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<37u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<37u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<37u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<37u, 7u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<37u, 8u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<37u, 9u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<37u, 10u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<37u, 11u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<37u, 12u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<37u, 13u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<37u, 14u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<37u, 15u> {
    static const uint32_t WORD = 3360u;
};
// Tile 38: a_segments = b_segments = {12,13}.
template<> struct TileMeta<38u> {
    static const uint32_t SEGMENT_MASK = 0x00003000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<38u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<38u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<38u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<38u, 3u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<38u, 4u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<38u, 5u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<38u, 6u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<38u, 7u> {
    static const uint32_t WORD = 3240u;
};
// Tile 39: a_segments = b_segments = {14}.
template<> struct TileMeta<39u> {
    static const uint32_t SEGMENT_MASK = 0x00004000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<39u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<39u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<39u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<39u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<39u, 4u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<39u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<39u, 6u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<39u, 7u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<39u, 8u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<39u, 9u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<39u, 10u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<39u, 11u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<39u, 12u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<39u, 13u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<39u, 14u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<39u, 15u> {
    static const uint32_t WORD = 3480u;
};
// Tile 40: a_segments = b_segments = {15}.
template<> struct TileMeta<40u> {
    static const uint32_t SEGMENT_MASK = 0x00008000u;
    static const uint32_t OFFSET_COUNT = 16u;
};
template<> struct TileOffset<40u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<40u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<40u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<40u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<40u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<40u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<40u, 6u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<40u, 7u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<40u, 8u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<40u, 9u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<40u, 10u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<40u, 11u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<40u, 12u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<40u, 13u> {
    static const uint32_t WORD = 3360u;
};
template<> struct TileOffset<40u, 14u> {
    static const uint32_t WORD = 3480u;
};
template<> struct TileOffset<40u, 15u> {
    static const uint32_t WORD = 3600u;
};
// Tile 41: a_segments = b_segments = {14,15}.
template<> struct TileMeta<41u> {
    static const uint32_t SEGMENT_MASK = 0x0000c000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<41u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<41u, 1u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<41u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<41u, 3u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<41u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<41u, 5u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<41u, 6u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<41u, 7u> {
    static const uint32_t WORD = 3480u;
};
// Tile 42: a_segments = b_segments = {12,14}.
template<> struct TileMeta<42u> {
    static const uint32_t SEGMENT_MASK = 0x00005000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<42u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<42u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<42u, 2u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<42u, 3u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<42u, 4u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<42u, 5u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<42u, 6u> {
    static const uint32_t WORD = 3120u;
};
template<> struct TileOffset<42u, 7u> {
    static const uint32_t WORD = 3240u;
};
// Tile 43: a_segments = b_segments = {13,15}.
template<> struct TileMeta<43u> {
    static const uint32_t SEGMENT_MASK = 0x0000a000u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<43u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<43u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<43u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<43u, 3u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<43u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<43u, 5u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<43u, 6u> {
    static const uint32_t WORD = 3240u;
};
template<> struct TileOffset<43u, 7u> {
    static const uint32_t WORD = 3360u;
};
// Tile 44: a_segments = b_segments = {12,13,14,15}.
template<> struct TileMeta<44u> {
    static const uint32_t SEGMENT_MASK = 0x0000f000u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<44u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<44u, 1u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<44u, 2u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<44u, 3u> {
    static const uint32_t WORD = 3240u;
};
// Tile 45: a_segments = b_segments = {8,12}.
template<> struct TileMeta<45u> {
    static const uint32_t SEGMENT_MASK = 0x00001100u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<45u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<45u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<45u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<45u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<45u, 4u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<45u, 5u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<45u, 6u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<45u, 7u> {
    static const uint32_t WORD = 2760u;
};
// Tile 46: a_segments = b_segments = {9,13}.
template<> struct TileMeta<46u> {
    static const uint32_t SEGMENT_MASK = 0x00002200u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<46u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<46u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<46u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<46u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<46u, 4u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<46u, 5u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<46u, 6u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<46u, 7u> {
    static const uint32_t WORD = 2880u;
};
// Tile 47: a_segments = b_segments = {8,9,12,13}.
template<> struct TileMeta<47u> {
    static const uint32_t SEGMENT_MASK = 0x00003300u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<47u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<47u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<47u, 2u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<47u, 3u> {
    static const uint32_t WORD = 2760u;
};
// Tile 48: a_segments = b_segments = {10,14}.
template<> struct TileMeta<48u> {
    static const uint32_t SEGMENT_MASK = 0x00004400u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<48u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<48u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<48u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<48u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<48u, 4u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<48u, 5u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<48u, 6u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<48u, 7u> {
    static const uint32_t WORD = 3000u;
};
// Tile 49: a_segments = b_segments = {11,15}.
template<> struct TileMeta<49u> {
    static const uint32_t SEGMENT_MASK = 0x00008800u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<49u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<49u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<49u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<49u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<49u, 4u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<49u, 5u> {
    static const uint32_t WORD = 2880u;
};
template<> struct TileOffset<49u, 6u> {
    static const uint32_t WORD = 3000u;
};
template<> struct TileOffset<49u, 7u> {
    static const uint32_t WORD = 3120u;
};
// Tile 50: a_segments = b_segments = {10,11,14,15}.
template<> struct TileMeta<50u> {
    static const uint32_t SEGMENT_MASK = 0x0000cc00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<50u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<50u, 1u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<50u, 2u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<50u, 3u> {
    static const uint32_t WORD = 3000u;
};
// Tile 51: a_segments = b_segments = {8,10,12,14}.
template<> struct TileMeta<51u> {
    static const uint32_t SEGMENT_MASK = 0x00005500u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<51u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<51u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<51u, 2u> {
    static const uint32_t WORD = 2640u;
};
template<> struct TileOffset<51u, 3u> {
    static const uint32_t WORD = 2760u;
};
// Tile 52: a_segments = b_segments = {9,11,13,15}.
template<> struct TileMeta<52u> {
    static const uint32_t SEGMENT_MASK = 0x0000aa00u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<52u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<52u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<52u, 2u> {
    static const uint32_t WORD = 2760u;
};
template<> struct TileOffset<52u, 3u> {
    static const uint32_t WORD = 2880u;
};
// Tile 53: a_segments = b_segments = {8,9,10,11,12,13,14,15}.
template<> struct TileMeta<53u> {
    static const uint32_t SEGMENT_MASK = 0x0000ff00u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<53u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<53u, 1u> {
    static const uint32_t WORD = 2760u;
};
// Tile 54: a_segments = b_segments = {0,8}.
template<> struct TileMeta<54u> {
    static const uint32_t SEGMENT_MASK = 0x00000101u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<54u, 0u> {
    static const uint32_t WORD = 960u;
};
template<> struct TileOffset<54u, 1u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<54u, 2u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<54u, 3u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<54u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<54u, 5u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<54u, 6u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<54u, 7u> {
    static const uint32_t WORD = 1800u;
};
// Tile 55: a_segments = b_segments = {1,9}.
template<> struct TileMeta<55u> {
    static const uint32_t SEGMENT_MASK = 0x00000202u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<55u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<55u, 1u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<55u, 2u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<55u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<55u, 4u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<55u, 5u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<55u, 6u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<55u, 7u> {
    static const uint32_t WORD = 1920u;
};
// Tile 56: a_segments = b_segments = {0,1,8,9}.
template<> struct TileMeta<56u> {
    static const uint32_t SEGMENT_MASK = 0x00000303u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<56u, 0u> {
    static const uint32_t WORD = 1080u;
};
template<> struct TileOffset<56u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<56u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<56u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 57: a_segments = b_segments = {2,10}.
template<> struct TileMeta<57u> {
    static const uint32_t SEGMENT_MASK = 0x00000404u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<57u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<57u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<57u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<57u, 3u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<57u, 4u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<57u, 5u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<57u, 6u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<57u, 7u> {
    static const uint32_t WORD = 2040u;
};
// Tile 58: a_segments = b_segments = {3,11}.
template<> struct TileMeta<58u> {
    static const uint32_t SEGMENT_MASK = 0x00000808u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<58u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<58u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<58u, 2u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<58u, 3u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<58u, 4u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<58u, 5u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<58u, 6u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<58u, 7u> {
    static const uint32_t WORD = 2160u;
};
// Tile 59: a_segments = b_segments = {2,3,10,11}.
template<> struct TileMeta<59u> {
    static const uint32_t SEGMENT_MASK = 0x00000c0cu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<59u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<59u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<59u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<59u, 3u> {
    static const uint32_t WORD = 2040u;
};
// Tile 60: a_segments = b_segments = {0,2,8,10}.
template<> struct TileMeta<60u> {
    static const uint32_t SEGMENT_MASK = 0x00000505u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<60u, 0u> {
    static const uint32_t WORD = 1200u;
};
template<> struct TileOffset<60u, 1u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<60u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<60u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 61: a_segments = b_segments = {1,3,9,11}.
template<> struct TileMeta<61u> {
    static const uint32_t SEGMENT_MASK = 0x00000a0au;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<61u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<61u, 1u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<61u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<61u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 62: a_segments = b_segments = {0,1,2,3,8,9,10,11}.
template<> struct TileMeta<62u> {
    static const uint32_t SEGMENT_MASK = 0x00000f0fu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<62u, 0u> {
    static const uint32_t WORD = 1320u;
};
template<> struct TileOffset<62u, 1u> {
    static const uint32_t WORD = 1800u;
};
// Tile 63: a_segments = b_segments = {4,12}.
template<> struct TileMeta<63u> {
    static const uint32_t SEGMENT_MASK = 0x00001010u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<63u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<63u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<63u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<63u, 3u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<63u, 4u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<63u, 5u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<63u, 6u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<63u, 7u> {
    static const uint32_t WORD = 2280u;
};
// Tile 64: a_segments = b_segments = {5,13}.
template<> struct TileMeta<64u> {
    static const uint32_t SEGMENT_MASK = 0x00002020u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<64u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<64u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<64u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<64u, 3u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<64u, 4u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<64u, 5u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<64u, 6u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<64u, 7u> {
    static const uint32_t WORD = 2400u;
};
// Tile 65: a_segments = b_segments = {4,5,12,13}.
template<> struct TileMeta<65u> {
    static const uint32_t SEGMENT_MASK = 0x00003030u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<65u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<65u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<65u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<65u, 3u> {
    static const uint32_t WORD = 2280u;
};
// Tile 66: a_segments = b_segments = {6,14}.
template<> struct TileMeta<66u> {
    static const uint32_t SEGMENT_MASK = 0x00004040u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<66u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<66u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<66u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<66u, 3u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<66u, 4u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<66u, 5u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<66u, 6u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<66u, 7u> {
    static const uint32_t WORD = 2520u;
};
// Tile 67: a_segments = b_segments = {7,15}.
template<> struct TileMeta<67u> {
    static const uint32_t SEGMENT_MASK = 0x00008080u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<67u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<67u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<67u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<67u, 3u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<67u, 4u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<67u, 5u> {
    static const uint32_t WORD = 2400u;
};
template<> struct TileOffset<67u, 6u> {
    static const uint32_t WORD = 2520u;
};
template<> struct TileOffset<67u, 7u> {
    static const uint32_t WORD = 2640u;
};
// Tile 68: a_segments = b_segments = {6,7,14,15}.
template<> struct TileMeta<68u> {
    static const uint32_t SEGMENT_MASK = 0x0000c0c0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<68u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<68u, 1u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<68u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<68u, 3u> {
    static const uint32_t WORD = 2520u;
};
// Tile 69: a_segments = b_segments = {4,6,12,14}.
template<> struct TileMeta<69u> {
    static const uint32_t SEGMENT_MASK = 0x00005050u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<69u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<69u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<69u, 2u> {
    static const uint32_t WORD = 2160u;
};
template<> struct TileOffset<69u, 3u> {
    static const uint32_t WORD = 2280u;
};
// Tile 70: a_segments = b_segments = {5,7,13,15}.
template<> struct TileMeta<70u> {
    static const uint32_t SEGMENT_MASK = 0x0000a0a0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<70u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<70u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<70u, 2u> {
    static const uint32_t WORD = 2280u;
};
template<> struct TileOffset<70u, 3u> {
    static const uint32_t WORD = 2400u;
};
// Tile 71: a_segments = b_segments = {4,5,6,7,12,13,14,15}.
template<> struct TileMeta<71u> {
    static const uint32_t SEGMENT_MASK = 0x0000f0f0u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<71u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<71u, 1u> {
    static const uint32_t WORD = 2280u;
};
// Tile 72: a_segments = b_segments = {0,4,8,12}.
template<> struct TileMeta<72u> {
    static const uint32_t SEGMENT_MASK = 0x00001111u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<72u, 0u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<72u, 1u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<72u, 2u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<72u, 3u> {
    static const uint32_t WORD = 1800u;
};
// Tile 73: a_segments = b_segments = {1,5,9,13}.
template<> struct TileMeta<73u> {
    static const uint32_t SEGMENT_MASK = 0x00002222u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<73u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<73u, 1u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<73u, 2u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<73u, 3u> {
    static const uint32_t WORD = 1920u;
};
// Tile 74: a_segments = b_segments = {0,1,4,5,8,9,12,13}.
template<> struct TileMeta<74u> {
    static const uint32_t SEGMENT_MASK = 0x00003333u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<74u, 0u> {
    static const uint32_t WORD = 1560u;
};
template<> struct TileOffset<74u, 1u> {
    static const uint32_t WORD = 1800u;
};
// Tile 75: a_segments = b_segments = {2,6,10,14}.
template<> struct TileMeta<75u> {
    static const uint32_t SEGMENT_MASK = 0x00004444u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<75u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<75u, 1u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<75u, 2u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<75u, 3u> {
    static const uint32_t WORD = 2040u;
};
// Tile 76: a_segments = b_segments = {3,7,11,15}.
template<> struct TileMeta<76u> {
    static const uint32_t SEGMENT_MASK = 0x00008888u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<76u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<76u, 1u> {
    static const uint32_t WORD = 1920u;
};
template<> struct TileOffset<76u, 2u> {
    static const uint32_t WORD = 2040u;
};
template<> struct TileOffset<76u, 3u> {
    static const uint32_t WORD = 2160u;
};
// Tile 77: a_segments = b_segments = {2,3,6,7,10,11,14,15}.
template<> struct TileMeta<77u> {
    static const uint32_t SEGMENT_MASK = 0x0000ccccu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<77u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<77u, 1u> {
    static const uint32_t WORD = 2040u;
};
// Tile 78: a_segments = b_segments = {0,2,4,6,8,10,12,14}.
template<> struct TileMeta<78u> {
    static const uint32_t SEGMENT_MASK = 0x00005555u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<78u, 0u> {
    static const uint32_t WORD = 1680u;
};
template<> struct TileOffset<78u, 1u> {
    static const uint32_t WORD = 1800u;
};
// Tile 79: a_segments = b_segments = {1,3,5,7,9,11,13,15}.
template<> struct TileMeta<79u> {
    static const uint32_t SEGMENT_MASK = 0x0000aaaau;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<79u, 0u> {
    static const uint32_t WORD = 1800u;
};
template<> struct TileOffset<79u, 1u> {
    static const uint32_t WORD = 1920u;
};
// Tile 80: a_segments = b_segments = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}.
template<> struct TileMeta<80u> {
    static const uint32_t SEGMENT_MASK = 0x0000ffffu;
    static const uint32_t OFFSET_COUNT = 1u;
};
template<> struct TileOffset<80u, 0u> {
    static const uint32_t WORD = 1800u;
};
} // namespace kara
#endif
