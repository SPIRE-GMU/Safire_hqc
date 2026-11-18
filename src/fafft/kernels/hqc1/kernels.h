#ifndef FAFFT_KERNELS_HQC1_H
#define FAFFT_KERNELS_HQC1_H
#include "../common/kernels.h"
void k_fleaf_first(input_stream_int32* __restrict in, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int SKIP_BEFORE, int SKIP_AFTER> void k_fleaf(input_stream_int32* __restrict in, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_fleaf_last(input_stream_int32* __restrict in, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_xf_10(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_9(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_8(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_7(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_fl_1(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_fl_2(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_fl_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_1(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_2(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_xi_7(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xi_8(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xi_9(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_xi_10(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_bleaf(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out);
void k_bout(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
#endif
