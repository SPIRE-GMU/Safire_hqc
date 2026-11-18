#include "../../t1_common.h"
namespace {
#include "bitconversion.c"
#include "encode.c"
}

// Template arguments: of every group of input words the kernel skips SKIP_BEFORE words, keeps
// eight and skips SKIP_AFTER words.
template<int SKIP_BEFORE, int SKIP_AFTER>
void k_fleaf(input_stream_int32* __restrict in, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in, sizeof(*in), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t local[128];
    for (int j = 15; j > -1; j += -1)
        chess_prepare_for_pipelining
        chess_loop_range(16, 16)
    {
        for (int skip = 0; skip < SKIP_BEFORE; skip += 1)
            chess_prepare_for_pipelining
            chess_loop_range(SKIP_BEFORE, SKIP_BEFORE)
        {
            (void)readincr(in);
        }
        for (int r = 7; r > -1; r += -1)
            chess_prepare_for_pipelining
            chess_loop_range(8, 8)
        {
            local[8*j + r] = static_cast<uint32_t>(readincr(in));
        }
        for (int skip = 0; skip < SKIP_AFTER; skip += 1)
            chess_prepare_for_pipelining
            chess_loop_range(SKIP_AFTER, SKIP_AFTER)
        {
            (void)readincr(in);
        }
    }
    bc_1_256(local, 16);
    {
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
