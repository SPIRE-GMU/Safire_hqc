#ifndef KARA_PROJECT_H
#define KARA_PROJECT_H
#include <adf.h>
#include "kernels.h"
using namespace adf;
class KaraGraph : public adf::graph {
public:
    static constexpr int WORK_TILES = 81;
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
        work[26] = kernel::create(tile_mid<26>);
        work[27] = kernel::create(tile_mid<27>);
        work[28] = kernel::create(tile_mid<28>);
        work[29] = kernel::create(tile_mid<29>);
        work[30] = kernel::create(tile_mid<30>);
        work[31] = kernel::create(tile_mid<31>);
        work[32] = kernel::create(tile_mid<32>);
        work[33] = kernel::create(tile_mid<33>);
        work[34] = kernel::create(tile_mid<34>);
        work[35] = kernel::create(tile_mid<35>);
        work[36] = kernel::create(tile_mid<36>);
        work[37] = kernel::create(tile_mid<37>);
        work[38] = kernel::create(tile_mid<38>);
        work[39] = kernel::create(tile_mid<39>);
        work[40] = kernel::create(tile_mid<40>);
        work[41] = kernel::create(tile_mid<41>);
        work[42] = kernel::create(tile_mid<42>);
        work[43] = kernel::create(tile_mid<43>);
        work[44] = kernel::create(tile_mid<44>);
        work[45] = kernel::create(tile_mid<45>);
        work[46] = kernel::create(tile_mid<46>);
        work[47] = kernel::create(tile_mid<47>);
        work[48] = kernel::create(tile_mid<48>);
        work[49] = kernel::create(tile_mid<49>);
        work[50] = kernel::create(tile_mid<50>);
        work[51] = kernel::create(tile_mid<51>);
        work[52] = kernel::create(tile_mid<52>);
        work[53] = kernel::create(tile_mid<53>);
        work[54] = kernel::create(tile_mid<54>);
        work[55] = kernel::create(tile_mid<55>);
        work[56] = kernel::create(tile_mid<56>);
        work[57] = kernel::create(tile_mid<57>);
        work[58] = kernel::create(tile_mid<58>);
        work[59] = kernel::create(tile_mid<59>);
        work[60] = kernel::create(tile_mid<60>);
        work[61] = kernel::create(tile_mid<61>);
        work[62] = kernel::create(tile_mid<62>);
        work[63] = kernel::create(tile_mid<63>);
        work[64] = kernel::create(tile_mid<64>);
        work[65] = kernel::create(tile_mid<65>);
        work[66] = kernel::create(tile_mid<66>);
        work[67] = kernel::create(tile_mid<67>);
        work[68] = kernel::create(tile_mid<68>);
        work[69] = kernel::create(tile_mid<69>);
        work[70] = kernel::create(tile_mid<70>);
        work[71] = kernel::create(tile_mid<71>);
        work[72] = kernel::create(tile_mid<72>);
        work[73] = kernel::create(tile_mid<73>);
        work[74] = kernel::create(tile_mid<74>);
        work[75] = kernel::create(tile_mid<75>);
        work[76] = kernel::create(tile_mid<76>);
        work[77] = kernel::create(tile_mid<77>);
        work[78] = kernel::create(tile_mid<78>);
        work[79] = kernel::create(tile_mid<79>);
        work[80] = kernel::create(tile_last<80>);
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
