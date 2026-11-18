#include "../../t1_common.h"
namespace {
#include "bitconversion.c"
#include "encode.c"
}

void k_fleaf_last(input_stream_int32* __restrict in, output_stream_int32* __restrict out0, output_stream_int32* __restrict out1) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out0, sizeof(*out0));
    t1_assert_disjoint(in, sizeof(*in), out1, sizeof(*out1));
    t1_assert_disjoint(out0, sizeof(*out0), out1, sizeof(*out1));
#endif
    static X86SIM_THREAD_LOCAL uint32_t local[128];
    for (int j = 15; j > -1; j += -1)
        chess_prepare_for_pipelining
        chess_loop_range(16, 16)
    {
        for (int r = 7; r > -1; r += -1)
            chess_prepare_for_pipelining
            chess_loop_range(8, 8)
        {
            local[8*j + r] = static_cast<uint32_t>(readincr(in));
        }
        for (int skip = 0; skip < 56; skip += 1)
            chess_prepare_for_pipelining
            chess_loop_range(56, 56)
        {
            (void)readincr(in);
        }
    }
    bc_1_256(local, 16);
    {
        // Global encoded group 56.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 0];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out0, encoded);
    }
    {
        // Global encoded group 60.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 4];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out1, encoded);
    }
    {
        // Global encoded group 57.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 1];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out0, encoded);
    }
    {
        // Global encoded group 61.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 5];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out1, encoded);
    }
    {
        // Global encoded group 58.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 2];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out0, encoded);
    }
    {
        // Global encoded group 62.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 6];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out1, encoded);
    }
    {
        // Global encoded group 59.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 3];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out0, encoded);
    }
    {
        // Global encoded group 63.
        uint32_t gathered[16];
        uint32_t encoded[32];
        for (int j = 0; j < 16; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(16, 16)
        {
            gathered[j] = local[8*j + 7];
        }
        t1_assert_disjoint(encoded, sizeof(encoded), gathered, sizeof(gathered));
        encode_trunc_fft(encoded, gathered);
        t1_write<32>(out1, encoded);
    }
}
