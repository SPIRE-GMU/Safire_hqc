#ifndef FAFFT_KERNELS_COMMON_H
#define FAFFT_KERNELS_COMMON_H
#include <adf.h>
template<int FRAME_WORDS, int POLY_WORDS> void k_fin(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int S, int O, int BLOCKS> void k_fp(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int S, int O, int BLOCKS> void k_fp_bits(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int S, int BLOCKS> void k_fp_half(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCKS> void k_pw(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out);
template<int WORDS> void k_join(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out);
template<int S, int BLOCKS> void k_bp_half(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int S, int O, int BLOCKS> void k_bp(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int S, int O, int BLOCKS> void k_bp_bits(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
#endif
