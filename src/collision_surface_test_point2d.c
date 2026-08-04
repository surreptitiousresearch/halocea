/* collision_surface_test_point2d @0x837E0504 — test whether a 2D point lies inside a collision
 * surface's polygon, working in the surface's projection plane. Walks the surface's edge ring; for
 * each edge it orients the edge so the surface is on the consistent side, then computes the 2D cross
 * product of (point - tail) against the edge direction. If the point ever falls on the outside
 * (cross > 0) the test fails (0). If the walk returns to the first edge with the point inside every
 * edge, the point is inside (1).
 *
 * Edges are 24-byte records: start_vertex@0, end_vertex@4, forward_edge@8, reverse_edge@12,
 * right_surface@20 (dword index 5). When the surface is the edge's right surface the ring is walked
 * via the reverse links with the start vertex as the tail; otherwise via the forward links with the
 * end vertex as the tail. Vertices are 16-byte (4-float) records; the projection mapping selects
 * which two components form the 2D plane. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"

extern const __int16 global_projection3d_mappings[3][2][2];

uint8_t collision_surface_test_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point)
{
    const collision_edge *edges = (const collision_edge *)bsp->edges.address;
    const float *vertices = (const float *)bsp->vertices.address;
    const __int16 *map    = global_projection3d_mappings[projection_axis][projection_sign];

    int first_edge = ((const collision_surface *)bsp->surfaces.address)[surface_index].first_edge_index;
    int edge_index = first_edge;

    while (1)
    {
        const collision_edge *edge = &edges[edge_index];
        int surface_on_right = (surface_index == edge->surface_indices[1]);

        const float *tail = &vertices[4 * (surface_on_right ? edge->vertex_indices[0] : edge->vertex_indices[1])];
        const float *head = &vertices[4 * (surface_on_right ? edge->vertex_indices[1] : edge->vertex_indices[0])];

        float cross = (point->n[1] - tail[map[1]]) * (point->n[0] - head[map[0]])
                    - (point->n[0] - tail[map[0]]) * (point->n[1] - head[map[1]]);
        if (cross > 0.0f)
            return 0;

        edge_index = surface_on_right ? edge->edge_indices[1] : edge->edge_indices[0];
        if (edge_index == first_edge)
            return 1;
    }
}
