/* collision_surface_perimeter @0x837E0128 — sum of the edge lengths of one collision-BSP surface. Walks
 * the surface's winged-edge ring exactly like collision_surface_edge_count (24-byte edges; the branchless
 * cntlzw forms in the compiled code are equality selects on `surface_index == edges[6*e + 5]`, picking the
 * edge's vertex pair in winding order and the next-edge link); vertices are 16-byte records whose first
 * three floats are the position.
 *
 * DEVIATION (decompiler bug class 9): the `return *((float *)&accumulator + 1)` register pun in the
 * decompiler output is just the float accumulator returned in fp1; rendered as a plain float return. */

#include "headers/collision_bsp.h"

float collision_surface_perimeter(const collision_bsp *bsp, int surface_index)
{
    const float *vertices = (const float *)bsp->vertices.address;
    const int *surfaces = (const int *)bsp->surfaces.address;
    const int *edges = (const int *)bsp->edges.address;

    int first_edge_index = surfaces[3 * surface_index + 1];
    int edge_index = first_edge_index;
    float perimeter = 0.0f;

    do
    {
        const int *edge = &edges[6 * edge_index];
        int forward = (surface_index == edge[5]);   /* 1 when this surface is the edge's right surface */
        const float *vertex0 = &vertices[4 * edge[forward]];
        const float *vertex1 = &vertices[4 * edge[1 - forward]];
        edge_index = edge[2 + forward];

        float dx = vertex1[0] - vertex0[0];
        float dy = vertex1[1] - vertex0[1];
        float dz = vertex1[2] - vertex0[2];
        perimeter += __builtin_sqrtf(dy * dy + dx * dx + dz * dz);
    }
    while ( edge_index != first_edge_index );

    return perimeter;
}
