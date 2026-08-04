/* net_graph_initialize @ 0x837642B0 — reset the bandwidth graph */

#include <stdint.h>
#include "headers/net_graph.h"

extern void update_for_resolution_change(net_graph *graph, uint8_t force);

extern void initialize_graph(net_graph *const graph, net_graph_data_type type, net_graph_data_direction direction);
int net_graph_initialize(void) /* attested int: explicit li r3,1; sole caller ignores (width-agnostic) */
{
    /* DEVIATION: donor initialize_graph@0x83764278 inlined here (0 xrefs); graph folded to &gGraph, type/direction folded to the compile-time-constant display mode this function initializes to */
    initialize_graph(&gGraph, net_graph_data_type_bytes, net_graph_data_direction_received);
    return 1;
}
