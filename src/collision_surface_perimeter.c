/* collision_surface_perimeter @0x837E0128 — sum of the edge lengths of one collision-BSP surface. Walks
 * the surface's winged-edge ring exactly like collision_surface_edge_count (the branchless cntlzw forms
 * in the compiled code are equality selects on `surface_index == edge->surface_indices[1]`, picking the
 * edge's vertex pair in winding order and the next-edge link).
 *
 * DEVIATION (decompiler bug class 9): the `return *((float *)&accumulator + 1)` register pun in the
 * decompiler output is just the float accumulator returned in fp1; rendered as a plain float return. */

#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_point3d.h"

float collision_surface_perimeter(const collision_bsp *bsp, int surface_index)
{
    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;
    const collision_edge *edges = (const collision_edge *)bsp->edges.address;

    int first_edge_index = surfaces[surface_index].first_edge_index;
    int edge_index = first_edge_index;
    float perimeter = 0.0f;

    do
    {
        const collision_edge *edge = &edges[edge_index];
        int forward = (surface_index == edge->surface_indices[1]);   /* 1 when this surface is the edge's right surface */
        const real_point3d *vertex0 = &vertices[edge->vertex_indices[forward]].point;
        const real_point3d *vertex1 = &vertices[edge->vertex_indices[1 - forward]].point;
        edge_index = edge->edge_indices[forward];

        float dx = vertex1->n[0] - vertex0->n[0];
        float dy = vertex1->n[1] - vertex0->n[1];
        float dz = vertex1->n[2] - vertex0->n[2];
        perimeter += __builtin_sqrtf(dy * dy + dx * dx + dz * dz);
    }
    while ( edge_index != first_edge_index );

    return perimeter;
}
