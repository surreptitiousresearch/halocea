/* collision_surface_edge_count @0x837E00C0 — DEVIATION: PPC calling convention reuses r3 for both the
 * incoming `bsp` parameter and the return value; the decompiler tracked the loop counter through that same
 * register and rendered it as `(const collision_bsp *)bsp`, an invented reinterpret-cast. disasm_range
 * (0x837E00C0,0x837E0124) confirms r3 is a plain incrementing counter, sign-extended each iteration and
 * returned as-is — matching the DB's own `__int16` return type. Walks the surface's winged-edge ring
 * (the branchless `cntlzw`-based equality test in disasm is the compiler's rendering of
 * `surface_index == edge->surface_indices[1]`, selecting which of the edge's two next-edge links to
 * follow) until it returns to the starting edge. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"

int16_t collision_surface_edge_count(const collision_bsp *bsp, int surface_index)
{
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;
    const collision_edge *edges = (const collision_edge *)bsp->edges.address;
    int first_edge_index = surfaces[surface_index].first_edge_index;
    int edge_index = first_edge_index;
    int16_t count = 0;

    do
    {
        count++;
        edge_index = edges[edge_index].edge_indices[surface_index == edges[edge_index].surface_indices[1]];
    } while ( edge_index != first_edge_index );

    return count;
}
