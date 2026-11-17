#include "project.h"
KaraGraph kara_graph;
#if defined(__AIESIM__) || defined(__X86SIM__)
int main() {
    kara_graph.init();
    kara_graph.run(2);
    kara_graph.wait();
    kara_graph.end();
    return 0;
}
#endif
