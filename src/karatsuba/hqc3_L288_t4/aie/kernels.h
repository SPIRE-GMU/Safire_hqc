#ifndef KARA_KERNELS_H
#define KARA_KERNELS_H
#include <adf.h>
// Work tiles: ID is the position in the chain (src/kernels/tile.cc).
template<unsigned ID>
void tile_first(input_stream_int32* __restrict operands,
                output_stream_int32* __restrict forward,
                output_stream_int32* __restrict fout);
template<unsigned ID>
void tile_mid(input_stream_int32* __restrict operands,
              input_stream_int32* __restrict fin,
              output_stream_int32* __restrict forward,
              output_stream_int32* __restrict fout);
template<unsigned ID>
void tile_last(input_stream_int32* __restrict operands,
               input_stream_int32* __restrict fin,
               output_stream_int32* __restrict fout);
void fold(input_stream_int32* __restrict in, output_stream_int32* __restrict out);
#endif
