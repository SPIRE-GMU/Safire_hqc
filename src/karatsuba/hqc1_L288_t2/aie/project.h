#ifndef KARA_PROJECT_H
#define KARA_PROJECT_H
#include <adf.h>
#include "kernels.h"
using namespace adf;
class KaraGraph : public adf::graph {
public:
    kernel work[9], folding;
    input_plio input;
    output_plio output;
    KaraGraph() {
        input = input_plio::create("KaraIn", plio_32_bits, "data/in.txt", 520);
        output = output_plio::create("KaraOut", plio_32_bits, "data/out.txt", 520);
        work[0] = kernel::create(tile0);
        source(work[0]) = "src/kernels/tile0.cc";
        runtime<ratio>(work[0]) = 1;
        location<stack>(work[0]) = location<kernel>(work[0]);
        work[1] = kernel::create(tile1);
        source(work[1]) = "src/kernels/tile1.cc";
        runtime<ratio>(work[1]) = 1;
        location<stack>(work[1]) = location<kernel>(work[1]);
        work[2] = kernel::create(tile2);
        source(work[2]) = "src/kernels/tile2.cc";
        runtime<ratio>(work[2]) = 1;
        location<stack>(work[2]) = location<kernel>(work[2]);
        work[3] = kernel::create(tile3);
        source(work[3]) = "src/kernels/tile3.cc";
        runtime<ratio>(work[3]) = 1;
        location<stack>(work[3]) = location<kernel>(work[3]);
        work[4] = kernel::create(tile4);
        source(work[4]) = "src/kernels/tile4.cc";
        runtime<ratio>(work[4]) = 1;
        location<stack>(work[4]) = location<kernel>(work[4]);
        work[5] = kernel::create(tile5);
        source(work[5]) = "src/kernels/tile5.cc";
        runtime<ratio>(work[5]) = 1;
        location<stack>(work[5]) = location<kernel>(work[5]);
        work[6] = kernel::create(tile6);
        source(work[6]) = "src/kernels/tile6.cc";
        runtime<ratio>(work[6]) = 1;
        location<stack>(work[6]) = location<kernel>(work[6]);
        work[7] = kernel::create(tile7);
        source(work[7]) = "src/kernels/tile7.cc";
        runtime<ratio>(work[7]) = 1;
        location<stack>(work[7]) = location<kernel>(work[7]);
        work[8] = kernel::create(tile8);
        source(work[8]) = "src/kernels/tile8.cc";
        runtime<ratio>(work[8]) = 1;
        location<stack>(work[8]) = location<kernel>(work[8]);
        folding = kernel::create(fold);
        source(folding) = "src/kernels/fold.cc";
        runtime<ratio>(folding) = 1;
        location<stack>(folding) = location<kernel>(folding);
        connect<stream>(input.out[0], work[0].in[0]);
        connect<stream>(work[0].out[0], work[1].in[0]);
        connect<stream>(work[0].out[1], work[1].in[1]);
        connect<stream>(work[1].out[0], work[2].in[0]);
        connect<stream>(work[1].out[1], work[2].in[1]);
        connect<stream>(work[2].out[0], work[3].in[0]);
        connect<stream>(work[2].out[1], work[3].in[1]);
        connect<stream>(work[3].out[0], work[4].in[0]);
        connect<stream>(work[3].out[1], work[4].in[1]);
        connect<stream>(work[4].out[0], work[5].in[0]);
        connect<stream>(work[4].out[1], work[5].in[1]);
        connect<stream>(work[5].out[0], work[6].in[0]);
        connect<stream>(work[5].out[1], work[6].in[1]);
        connect<stream>(work[6].out[0], work[7].in[0]);
        connect<stream>(work[6].out[1], work[7].in[1]);
        connect<stream>(work[7].out[0], work[8].in[0]);
        connect<stream>(work[7].out[1], work[8].in[1]);
        connect<stream>(work[8].out[0], folding.in[0]);
        connect<stream>(folding.out[0], output.in[0]);
    }
};
#endif
