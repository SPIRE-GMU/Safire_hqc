#ifndef KARA_PROJECT_H
#define KARA_PROJECT_H
#include <adf.h>
#include "kernels.h"
using namespace adf;
class KaraGraph : public adf::graph {
public:
    static constexpr int WORK_TILES = 27;
    kernel work[WORK_TILES], folding;
    input_plio input;
    output_plio output;
    KaraGraph() {
        input = input_plio::create("KaraIn", plio_32_bits, "data/in.txt", 520);
        output = output_plio::create("KaraOut", plio_32_bits, "data/out.txt", 520);
        // Every work tile is an instance of the same kernel source. The template
        // argument is the position of the tile in the chain.
        work[0] = kernel::create(tile_first<0>);
        work[1] = kernel::create(tile_mid<1>);
        work[2] = kernel::create(tile_mid<2>);
        work[3] = kernel::create(tile_mid<3>);
        work[4] = kernel::create(tile_mid<4>);
        work[5] = kernel::create(tile_mid<5>);
        work[6] = kernel::create(tile_mid<6>);
        work[7] = kernel::create(tile_mid<7>);
        work[8] = kernel::create(tile_mid<8>);
        work[9] = kernel::create(tile_mid<9>);
        work[10] = kernel::create(tile_mid<10>);
        work[11] = kernel::create(tile_mid<11>);
        work[12] = kernel::create(tile_mid<12>);
        work[13] = kernel::create(tile_mid<13>);
        work[14] = kernel::create(tile_mid<14>);
        work[15] = kernel::create(tile_mid<15>);
        work[16] = kernel::create(tile_mid<16>);
        work[17] = kernel::create(tile_mid<17>);
        work[18] = kernel::create(tile_mid<18>);
        work[19] = kernel::create(tile_mid<19>);
        work[20] = kernel::create(tile_mid<20>);
        work[21] = kernel::create(tile_mid<21>);
        work[22] = kernel::create(tile_mid<22>);
        work[23] = kernel::create(tile_mid<23>);
        work[24] = kernel::create(tile_mid<24>);
        work[25] = kernel::create(tile_mid<25>);
        work[26] = kernel::create(tile_last<26>);
        for (int i = 0; i < WORK_TILES; ++i) {
            source(work[i]) = "src/kernels/tile.cc";
            runtime<ratio>(work[i]) = 1;
            location<stack>(work[i]) = location<kernel>(work[i]);
        }
        folding = kernel::create(fold);
        source(folding) = "src/kernels/fold.cc";
        runtime<ratio>(folding) = 1;
        location<stack>(folding) = location<kernel>(folding);
        // Port 0 carries the operands down the chain, port 1 the accumulated product F.
        // The last work tile forwards no operands, so its only output is F.
        connect<stream>(input.out[0], work[0].in[0]);
        for (int i = 0; i + 1 < WORK_TILES; ++i) {
            connect<stream>(work[i].out[0], work[i + 1].in[0]);
            connect<stream>(work[i].out[1], work[i + 1].in[1]);
        }
        connect<stream>(work[WORK_TILES - 1].out[0], folding.in[0]);
        connect<stream>(folding.out[0], output.in[0]);
    }
};
#endif
