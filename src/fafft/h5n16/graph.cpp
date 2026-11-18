#include "graph.h"
h5n16Graph h5n16_graph;
#if defined(__AIESIM__) || defined(__X86SIM__)
int main() {
    h5n16_graph.init();
    h5n16_graph.run(3);
    h5n16_graph.end();
    return 0;
}
#endif
