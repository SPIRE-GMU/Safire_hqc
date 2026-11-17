#ifndef KARA_KERNELS_H
#define KARA_KERNELS_H
#include <adf.h>
void tile0(input_stream_int32* __restrict operands,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile1(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile2(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile3(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile4(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile5(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile6(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile7(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile8(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile9(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile10(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile11(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile12(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile13(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile14(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile15(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile16(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile17(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile18(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile19(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile20(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile21(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile22(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile23(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile24(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile25(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict forward,
           output_stream_int32* __restrict fout);
void tile26(input_stream_int32* __restrict operands,
           input_stream_int32* __restrict fin,
           output_stream_int32* __restrict fout);
void fold(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
#endif
