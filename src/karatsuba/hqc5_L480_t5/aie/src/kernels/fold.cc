#include <adf.h>
#include <aie_api/aie.hpp>
#include <stdint.h>
#define KARA_PIPELINE chess_prepare_for_pipelining
#define kara kara_fold_namespace
#include "../../../tile.h"
#undef kara
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

void fold(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
    aie::set_saturation(aie::saturation_mode::none);
    aie::set_rounding(aie::rounding_mode::floor);
    using namespace kara_fold_namespace;
    alignas(32) static uint32_t product[F_WORDS];
    alignas(32) static uint32_t result[FRAME_WORDS];
    Input input{in};
    Output output{out};
    fold_tile(input, output, product, result);
}
