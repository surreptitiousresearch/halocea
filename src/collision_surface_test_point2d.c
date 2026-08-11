/* collision_surface_test_point2d @0x837E04F0 — test whether a 2D point lies inside a collision
 * surface's polygon, working in the surface's projection plane. Walks the surface's edge ring; for
 * each edge it orients the edge so the surface is on the consistent side, then computes the 2D cross
 * product of (point - tail) against the edge direction. If the point ever falls on the outside
 * (cross > 0) the test fails (0). If the walk returns to the first edge with the point inside every
 * edge, the point is inside (1).
 *
 * When the surface is the edge's right surface the ring is walked via the reverse links with the
 * start vertex as the tail; otherwise via the forward links with the end vertex as the tail. The
 * projection mapping selects which two position components form the 2D plane. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


uint8_t collision_surface_test_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point)
{
    const collision_edge *edges = (const collision_edge *)bsp->edges.address;
    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const int16_t *map    = global_projection3d_mappings[projection_axis][projection_sign];

    int first_edge = ((const collision_surface *)bsp->surfaces.address)[surface_index].first_edge_index;
    int edge_index = first_edge;

    while (1)
    {
        const collision_edge *edge = &edges[edge_index];
        int surface_on_right = (surface_index == edge->surface_indices[1]);

        const real_point3d *tail = &vertices[surface_on_right ? edge->vertex_indices[0] : edge->vertex_indices[1]].point;
        const real_point3d *head = &vertices[surface_on_right ? edge->vertex_indices[1] : edge->vertex_indices[0]].point;

        float cross = (point->n[1] - tail->n[map[1]]) * (point->n[0] - head->n[map[0]])
                    - (point->n[0] - tail->n[map[0]]) * (point->n[1] - head->n[map[1]]);
        if (cross > 0.0f)
            return 0;

        edge_index = surface_on_right ? edge->edge_indices[1] : edge->edge_indices[0];
        if (edge_index == first_edge)
            return 1;
    }
}
