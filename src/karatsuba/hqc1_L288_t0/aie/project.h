#ifndef KARA_PROJECT_H
#define KARA_PROJECT_H
#include <adf.h>
#include "kernels.h"
using namespace adf;
class KaraGraph : public adf::graph {
public:
    kernel work;
    input_plio input;
    output_plio output;
    KaraGraph() {
        input = input_plio::create("KaraIn", plio_32_bits, "data/in.txt", 520);
        output = output_plio::create("KaraOut", plio_32_bits, "data/out.txt", 520);
        work = kernel::create(tile0);
        source(work) = "src/kernels/tile0.cc";
        runtime<ratio>(work) = 1;
        location<stack>(work) = location<kernel>(work);
        connect<stream>(input.out[0], work.in[0]);
        connect<stream>(work.out[0], output.in[0]);
    }
};
#endif
