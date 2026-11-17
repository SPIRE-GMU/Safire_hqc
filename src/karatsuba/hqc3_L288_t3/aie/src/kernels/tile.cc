// Work tiles of the Karatsuba chain. This file is the source of every work tile:
// the template argument ID is the position of the tile in the chain and selects
// its segment and offset tables in config.h and tile.h at compile time.
// The first tile has no F input and the last tile forwards no operands, so there
// are three kernels that differ only in their stream ports.
#include <adf.h>
#include <aie_api/aie.hpp>
#include <stdint.h>
#define KARA_PIPELINE chess_prepare_for_pipelining
#include "../../../config.h"
#include "leaf_aie.h"
#include "../../../tile.h"
namespace {
struct Input {
    input_stream_int32* __restrict ptr;
    uint32_t read() { return static_cast<uint32_t>(readincr(ptr)); }
};
struct Output {
    output_stream_int32* __restrict ptr;
    void write(uint32_t word) { writeincr(ptr, static_cast<int32>(word)); }
};
struct Unused {};
} // namespace

template<unsigned ID> KARA_NOINLINE
void tile_first(input_stream_int32* __restrict operands,
                output_stream_int32* __restrict forward,
                output_stream_int32* __restrict fout) {
    static_assert(ID == 0u, "the first work tile is tile 0");
    aie::set_saturation(aie::saturation_mode::none);
    aie::set_rounding(aie::rounding_mode::floor);
    using namespace kara;
    alignas(32) static uint32_t a[SUB_WORDS];
    alignas(32) static uint32_t b[SUB_WORDS];
    alignas(32) static uint32_t product[PRODUCT_WORDS];
    alignas(32) static uint32_t scratch[SCRATCH_WORDS];
    Input input{operands};
    Unused prior;
    Output onward{forward};
    Output next{fout};
    Tile<ID>::receive(input, onward, a, b);
    Tile<ID>::compute(a, b, product, scratch);
    Tile<ID>::emit(prior, next, product, nullptr);
}

template<unsigned ID> KARA_NOINLINE
void tile_mid(input_stream_int32* __restrict operands,
              input_stream_int32* __restrict fin,
              output_stream_int32* __restrict forward,
              output_stream_int32* __restrict fout) {
    static_assert(ID > 0u && ID + 1u < kara::G, "a middle work tile is neither the first nor the last");
    aie::set_saturation(aie::saturation_mode::none);
    aie::set_rounding(aie::rounding_mode::floor);
    using namespace kara;
    alignas(32) static uint32_t a[SUB_WORDS];
    alignas(32) static uint32_t b[SUB_WORDS];
    alignas(32) static uint32_t product[PRODUCT_WORDS];
    alignas(32) static uint32_t scratch[SCRATCH_WORDS];
    Input input{operands};
    Input prior{fin};
    Output onward{forward};
    Output next{fout};
    Tile<ID>::receive(input, onward, a, b);
    Tile<ID>::compute(a, b, product, scratch);
    Tile<ID>::emit(prior, next, product, nullptr);
}

template<unsigned ID> KARA_NOINLINE
void tile_last(input_stream_int32* __restrict operands,
               input_stream_int32* __restrict fin,
               output_stream_int32* __restrict fout) {
    static_assert(ID + 1u == kara::G, "the last work tile is tile G - 1");
    aie::set_saturation(aie::saturation_mode::none);
    aie::set_rounding(aie::rounding_mode::floor);
    using namespace kara;
    alignas(32) static uint32_t a[SUB_WORDS];
    alignas(32) static uint32_t b[SUB_WORDS];
    alignas(32) static uint32_t product[PRODUCT_WORDS];
    alignas(32) static uint32_t scratch[SCRATCH_WORDS];
    Input input{operands};
    Input prior{fin};
    Unused onward;
    Output next{fout};
    Tile<ID>::receive(input, onward, a, b);
    Tile<ID>::compute(a, b, product, scratch);
    Tile<ID>::emit(prior, next, product, nullptr);
}
