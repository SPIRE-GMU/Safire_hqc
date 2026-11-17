// Configuration constants; all metadata is compile-time.
#ifndef KARA_CONFIG_H
#define KARA_CONFIG_H
#include <stdint.h>
#ifndef KARA_PIPELINE
#define KARA_PIPELINE
#endif
namespace kara {
static const uint32_t N_BITS = 35851u;
static const uint32_t FRAME_WORDS = 1122u;
static const uint32_t L_BITS = 288u;
static const uint32_t LEVELS = 7u;
static const uint32_t PAD_BITS = 36864u;
static const uint32_t SPLIT = 3u;
static const uint32_t G = 27u;
static const uint32_t SUB_WORDS = 144u;
static const uint32_t PRODUCT_WORDS = 288u;
static const uint32_t F_WORDS = 2304u;
static const uint32_t SCRATCH_WORDS = 540u;
static const uint32_t LEAF_WORDS = 9u;
static const uint32_t CONTRIBUTION_WORDS = 0u;
static const uint32_t LOCAL_LEVELS = 4u;
static const uint32_t INPUT_SEGMENTS = 8u;
static const uint32_t WORK_TILE_BYTES = 4464u;
static const uint32_t FOLD_TILE_BYTES = 13704u;
static const uint32_t AIE_WORK_TILE_BYTES = 9840u;
static const uint32_t AIE_FOLD_TILE_BYTES = 17800u;
template<uint32_t TILE> struct TileMeta;
template<uint32_t TILE, uint32_t INDEX> struct TileOffset;
// Tile 0: a_segments = b_segments = {0}.
template<> struct TileMeta<0u> {
    static const uint32_t SEGMENT_MASK = 0x00000001u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<0u, 0u> {
    static const uint32_t WORD = 0u;
};
template<> struct TileOffset<0u, 1u> {
    static const uint32_t WORD = 144u;
};
template<> struct TileOffset<0u, 2u> {
    static const uint32_t WORD = 288u;
};
template<> struct TileOffset<0u, 3u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<0u, 4u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<0u, 5u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<0u, 6u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<0u, 7u> {
    static const uint32_t WORD = 1008u;
};
// Tile 1: a_segments = b_segments = {1}.
template<> struct TileMeta<1u> {
    static const uint32_t SEGMENT_MASK = 0x00000002u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<1u, 0u> {
    static const uint32_t WORD = 144u;
};
template<> struct TileOffset<1u, 1u> {
    static const uint32_t WORD = 288u;
};
template<> struct TileOffset<1u, 2u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<1u, 3u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<1u, 4u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<1u, 5u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<1u, 6u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<1u, 7u> {
    static const uint32_t WORD = 1152u;
};
// Tile 2: a_segments = b_segments = {0,1}.
template<> struct TileMeta<2u> {
    static const uint32_t SEGMENT_MASK = 0x00000003u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<2u, 0u> {
    static const uint32_t WORD = 144u;
};
template<> struct TileOffset<2u, 1u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<2u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<2u, 3u> {
    static const uint32_t WORD = 1008u;
};
// Tile 3: a_segments = b_segments = {2}.
template<> struct TileMeta<3u> {
    static const uint32_t SEGMENT_MASK = 0x00000004u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<3u, 0u> {
    static const uint32_t WORD = 288u;
};
template<> struct TileOffset<3u, 1u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<3u, 2u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<3u, 3u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<3u, 4u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<3u, 5u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<3u, 6u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<3u, 7u> {
    static const uint32_t WORD = 1296u;
};
// Tile 4: a_segments = b_segments = {3}.
template<> struct TileMeta<4u> {
    static const uint32_t SEGMENT_MASK = 0x00000008u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<4u, 0u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<4u, 1u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<4u, 2u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<4u, 3u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<4u, 4u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<4u, 5u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<4u, 6u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<4u, 7u> {
    static const uint32_t WORD = 1440u;
};
// Tile 5: a_segments = b_segments = {2,3}.
template<> struct TileMeta<5u> {
    static const uint32_t SEGMENT_MASK = 0x0000000cu;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<5u, 0u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<5u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<5u, 2u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<5u, 3u> {
    static const uint32_t WORD = 1296u;
};
// Tile 6: a_segments = b_segments = {0,2}.
template<> struct TileMeta<6u> {
    static const uint32_t SEGMENT_MASK = 0x00000005u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<6u, 0u> {
    static const uint32_t WORD = 288u;
};
template<> struct TileOffset<6u, 1u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<6u, 2u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<6u, 3u> {
    static const uint32_t WORD = 1008u;
};
// Tile 7: a_segments = b_segments = {1,3}.
template<> struct TileMeta<7u> {
    static const uint32_t SEGMENT_MASK = 0x0000000au;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<7u, 0u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<7u, 1u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<7u, 2u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<7u, 3u> {
    static const uint32_t WORD = 1152u;
};
// Tile 8: a_segments = b_segments = {0,1,2,3}.
template<> struct TileMeta<8u> {
    static const uint32_t SEGMENT_MASK = 0x0000000fu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<8u, 0u> {
    static const uint32_t WORD = 432u;
};
template<> struct TileOffset<8u, 1u> {
    static const uint32_t WORD = 1008u;
};
// Tile 9: a_segments = b_segments = {4}.
template<> struct TileMeta<9u> {
    static const uint32_t SEGMENT_MASK = 0x00000010u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<9u, 0u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<9u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<9u, 2u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<9u, 3u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<9u, 4u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<9u, 5u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<9u, 6u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<9u, 7u> {
    static const uint32_t WORD = 1584u;
};
// Tile 10: a_segments = b_segments = {5}.
template<> struct TileMeta<10u> {
    static const uint32_t SEGMENT_MASK = 0x00000020u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<10u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<10u, 1u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<10u, 2u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<10u, 3u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<10u, 4u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<10u, 5u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<10u, 6u> {
    static const uint32_t WORD = 1584u;
};
template<> struct TileOffset<10u, 7u> {
    static const uint32_t WORD = 1728u;
};
// Tile 11: a_segments = b_segments = {4,5}.
template<> struct TileMeta<11u> {
    static const uint32_t SEGMENT_MASK = 0x00000030u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<11u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<11u, 1u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<11u, 2u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<11u, 3u> {
    static const uint32_t WORD = 1584u;
};
// Tile 12: a_segments = b_segments = {6}.
template<> struct TileMeta<12u> {
    static const uint32_t SEGMENT_MASK = 0x00000040u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<12u, 0u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<12u, 1u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<12u, 2u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<12u, 3u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<12u, 4u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<12u, 5u> {
    static const uint32_t WORD = 1584u;
};
template<> struct TileOffset<12u, 6u> {
    static const uint32_t WORD = 1728u;
};
template<> struct TileOffset<12u, 7u> {
    static const uint32_t WORD = 1872u;
};
// Tile 13: a_segments = b_segments = {7}.
template<> struct TileMeta<13u> {
    static const uint32_t SEGMENT_MASK = 0x00000080u;
    static const uint32_t OFFSET_COUNT = 8u;
};
template<> struct TileOffset<13u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<13u, 1u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<13u, 2u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<13u, 3u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<13u, 4u> {
    static const uint32_t WORD = 1584u;
};
template<> struct TileOffset<13u, 5u> {
    static const uint32_t WORD = 1728u;
};
template<> struct TileOffset<13u, 6u> {
    static const uint32_t WORD = 1872u;
};
template<> struct TileOffset<13u, 7u> {
    static const uint32_t WORD = 2016u;
};
// Tile 14: a_segments = b_segments = {6,7}.
template<> struct TileMeta<14u> {
    static const uint32_t SEGMENT_MASK = 0x000000c0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<14u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<14u, 1u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<14u, 2u> {
    static const uint32_t WORD = 1584u;
};
template<> struct TileOffset<14u, 3u> {
    static const uint32_t WORD = 1872u;
};
// Tile 15: a_segments = b_segments = {4,6}.
template<> struct TileMeta<15u> {
    static const uint32_t SEGMENT_MASK = 0x00000050u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<15u, 0u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<15u, 1u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<15u, 2u> {
    static const uint32_t WORD = 1440u;
};
template<> struct TileOffset<15u, 3u> {
    static const uint32_t WORD = 1584u;
};
// Tile 16: a_segments = b_segments = {5,7}.
template<> struct TileMeta<16u> {
    static const uint32_t SEGMENT_MASK = 0x000000a0u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<16u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<16u, 1u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<16u, 2u> {
    static const uint32_t WORD = 1584u;
};
template<> struct TileOffset<16u, 3u> {
    static const uint32_t WORD = 1728u;
};
// Tile 17: a_segments = b_segments = {4,5,6,7}.
template<> struct TileMeta<17u> {
    static const uint32_t SEGMENT_MASK = 0x000000f0u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<17u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<17u, 1u> {
    static const uint32_t WORD = 1584u;
};
// Tile 18: a_segments = b_segments = {0,4}.
template<> struct TileMeta<18u> {
    static const uint32_t SEGMENT_MASK = 0x00000011u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<18u, 0u> {
    static const uint32_t WORD = 576u;
};
template<> struct TileOffset<18u, 1u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<18u, 2u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<18u, 3u> {
    static const uint32_t WORD = 1008u;
};
// Tile 19: a_segments = b_segments = {1,5}.
template<> struct TileMeta<19u> {
    static const uint32_t SEGMENT_MASK = 0x00000022u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<19u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<19u, 1u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<19u, 2u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<19u, 3u> {
    static const uint32_t WORD = 1152u;
};
// Tile 20: a_segments = b_segments = {0,1,4,5}.
template<> struct TileMeta<20u> {
    static const uint32_t SEGMENT_MASK = 0x00000033u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<20u, 0u> {
    static const uint32_t WORD = 720u;
};
template<> struct TileOffset<20u, 1u> {
    static const uint32_t WORD = 1008u;
};
// Tile 21: a_segments = b_segments = {2,6}.
template<> struct TileMeta<21u> {
    static const uint32_t SEGMENT_MASK = 0x00000044u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<21u, 0u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<21u, 1u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<21u, 2u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<21u, 3u> {
    static const uint32_t WORD = 1296u;
};
// Tile 22: a_segments = b_segments = {3,7}.
template<> struct TileMeta<22u> {
    static const uint32_t SEGMENT_MASK = 0x00000088u;
    static const uint32_t OFFSET_COUNT = 4u;
};
template<> struct TileOffset<22u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<22u, 1u> {
    static const uint32_t WORD = 1152u;
};
template<> struct TileOffset<22u, 2u> {
    static const uint32_t WORD = 1296u;
};
template<> struct TileOffset<22u, 3u> {
    static const uint32_t WORD = 1440u;
};
// Tile 23: a_segments = b_segments = {2,3,6,7}.
template<> struct TileMeta<23u> {
    static const uint32_t SEGMENT_MASK = 0x000000ccu;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<23u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<23u, 1u> {
    static const uint32_t WORD = 1296u;
};
// Tile 24: a_segments = b_segments = {0,2,4,6}.
template<> struct TileMeta<24u> {
    static const uint32_t SEGMENT_MASK = 0x00000055u;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<24u, 0u> {
    static const uint32_t WORD = 864u;
};
template<> struct TileOffset<24u, 1u> {
    static const uint32_t WORD = 1008u;
};
// Tile 25: a_segments = b_segments = {1,3,5,7}.
template<> struct TileMeta<25u> {
    static const uint32_t SEGMENT_MASK = 0x000000aau;
    static const uint32_t OFFSET_COUNT = 2u;
};
template<> struct TileOffset<25u, 0u> {
    static const uint32_t WORD = 1008u;
};
template<> struct TileOffset<25u, 1u> {
    static const uint32_t WORD = 1152u;
};
// Tile 26: a_segments = b_segments = {0,1,2,3,4,5,6,7}.
template<> struct TileMeta<26u> {
    static const uint32_t SEGMENT_MASK = 0x000000ffu;
    static const uint32_t OFFSET_COUNT = 1u;
};
template<> struct TileOffset<26u, 0u> {
    static const uint32_t WORD = 1008u;
};
} // namespace kara
#endif
