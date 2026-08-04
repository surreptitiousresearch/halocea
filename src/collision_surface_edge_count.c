/* collision_surface_edge_count @0x837E00C0 — DEVIATION: PPC calling convention reuses r3 for both the
 * incoming `bsp` parameter and the return value; the decompiler tracked the loop counter through that same
 * register and rendered it as `(const collision_bsp *)bsp`, an invented reinterpret-cast. disasm_range
 * (0x837E00C0,0x837E0124) confirms r3 is a plain incrementing counter, sign-extended each iteration and
 * returned as-is — matching the DB's own `__int16` return type. Walks the surface's edge ring (each edge
 * stores its two neighboring-surface edge links at offsets +2/+5 of a 6-int stride; the branchless
 * `cntlzw`-based equality test in disasm is the compiler's rendering of `surface_index == edges[...+5]`,
 * matching the original decompile's own indexing exactly) until it returns to the starting edge. */

#include <stdint.h>
#include "headers/collision_bsp.h"

int16_t collision_surface_edge_count(const collision_bsp *bsp, int surface_index)
{
    int *surfaces = (int *)bsp->surfaces.address;
    int *edges = (int *)bsp->edges.address;
    int first_edge_index = surfaces[3 * surface_index + 1];
    int edge_index = first_edge_index;
    __int16 count = 0;

    do
    {
        count++;
        edge_index = edges[6 * edge_index + 2 + (surface_index == edges[6 * edge_index + 5])];
    } while ( edge_index != first_edge_index );

    return count;
}
