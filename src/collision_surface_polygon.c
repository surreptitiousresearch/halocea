/* collision_surface_polygon @0x837E03E0 — gather the ordered vertex positions of a collision surface by walking
 * its edge ring, writing them into `points` and returning the vertex count.
 *
 * Deviation: the DB return type is __int16 (the vertex count); Hex-Rays reuses the `bsp` register as the loop
 * counter and therefore mistypes the return as `const collision_bsp *`. Edge orientation/idiom matches the rest
 * of the collision code: forward = (surface_index == edge.surface_indices[1]), near vertex = forward ? vert1 : vert0,
 * next edge = forward ? reverse_next : forward_next. */

#include <stdint.h>

#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_point3d.h"

int16_t collision_surface_polygon(const collision_bsp *bsp, int surface_index, real_point3d *points)
{
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;
    int first_edge = surfaces[surface_index].first_edge_index;

    int16_t count = 0;
    int edge_index = first_edge;
    do
    {
        const collision_edge *edge = &((const collision_edge *)bsp->edges.address)[edge_index];
        int forward = (surface_index == edge->surface_indices[1]);
        int vertex_index = edge->vertex_indices[forward];
        const collision_vertex *vertex = &((const collision_vertex *)bsp->vertices.address)[vertex_index];

        points[count].n[0] = vertex->point.n[0];
        points[count].n[1] = vertex->point.n[1];
        points[count].n[2] = vertex->point.n[2];
        count = (int16_t)(count + 1);

        edge_index = edge->edge_indices[forward];
    }
    while ( edge_index != first_edge );

    return count;
}
