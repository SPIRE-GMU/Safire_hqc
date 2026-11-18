#include "graph.h"
h3n16Graph h3n16_graph;
#if defined(__AIESIM__) || defined(__X86SIM__)
int main() {
    h3n16_graph.init();
    h3n16_graph.run(3);
    h3n16_graph.end();
    return 0;
}
#endif
