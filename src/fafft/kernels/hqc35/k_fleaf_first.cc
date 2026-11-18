#include "../../t1_common.h"
namespace {
#include "bitconversion.c"
#include "encode.c"
}

void k_fleaf_first(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t local[128];
    for (int j = 15; j > -1; j += -1)
        chess_prepare_for_pipelining
        chess_loop_range(16, 16)
    {
        for (int skip = 0; skip < 120; skip += 1)
            chess_prepare_for_pipelining
            chess_loop_range(120, 120)
        {
            (void)readincr(in);
        }
        for (int r = 7; r > -1; r += -1)
            chess_prepare_for_pipelining
            chess_loop_range(8, 8)
        {
            local[8*j + r] = static_cast<uint32_t>(readincr(in));
        }
    }
    bc_1_256(local, 16);
    {
        // Global encoded group 0.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 1.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 2.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 3.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 4.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 5.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 6.
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
        t1_write<32>(out, encoded);
    }
    {
        // Global encoded group 7.
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
        t1_write<32>(out, encoded);
    }
}
