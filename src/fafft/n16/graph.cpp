#include "graph.h"
n16Graph n16_graph;
#if defined(__AIESIM__) || defined(__X86SIM__)
int main() {
    n16_graph.init();
    n16_graph.run(3);
    n16_graph.end();
    return 0;
}
#endif
