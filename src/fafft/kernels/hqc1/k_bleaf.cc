#include "../../t1_common.h"
namespace {
#include "bitconversion.c"
#include "encode.c"
}

void k_bleaf(input_stream_int32* __restrict in0, input_stream_int32* __restrict in1, output_stream_int32* __restrict out) {
#ifdef T1_HOST_CHECK
    t1_assert_disjoint(in0, sizeof(*in0), in1, sizeof(*in1));
    t1_assert_disjoint(in0, sizeof(*in0), out, sizeof(*out));
    t1_assert_disjoint(in1, sizeof(*in1), out, sizeof(*out));
#endif
    static X86SIM_THREAD_LOCAL uint32_t local[256];
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in0, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 0] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in1, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 4] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in0, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 1] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in1, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 5] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in0, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 2] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in1, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 6] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in0, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 3] = decoded[j];
        }
    }
    {
        uint32_t encoded[32];
        uint32_t decoded[32];
        t1_read<32>(in1, encoded);
        t1_assert_disjoint(decoded, sizeof(decoded), encoded, sizeof(encoded));
        decode_trunc_fft(decoded, encoded);
        for (int j = 0; j < 32; j += 1)
            chess_prepare_for_pipelining
            chess_loop_range(32, 32)
        {
            local[8*j + 7] = decoded[j];
        }
    }
    ibc_1_256(local, 32);
    for (int i = 255; i > -1; i += -1)
        chess_prepare_for_pipelining
        chess_loop_range(256, 256)
    {
        writeincr(out, static_cast<int32>(local[i]));
    }
}
