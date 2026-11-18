#ifndef FAFFT_KERNELS_HQC35_H
#define FAFFT_KERNELS_HQC35_H
#include "../common/kernels.h"
void k_fleaf_first(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int SKIP_BEFORE, int SKIP_AFTER> void k_fleaf(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
void k_fleaf_last(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
void k_xf_11(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_10(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_9(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xf_8(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_fl_1(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_fl_2(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_fl_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_1(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_2(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_il_3(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
template<int BLOCK> void k_xi_8(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xi_9(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
template<int BLOCK> void k_xi_10(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_xi_11(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1);
void k_bleaf(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
void k_bs16(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
void k_bout_hqc3(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
void k_bout_hqc5(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
#endif
