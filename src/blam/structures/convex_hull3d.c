/* convex_hull3d @0x837FF500 — build a 3D convex hull incrementally: begin with an initial hull, then expand
 * by adding each remaining point one at a time. Returns whether the hull was built successfully. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/vertex3d.h"
#include "headers/edge3d.h"
#include "headers/surface3d.h"

extern uint8_t convex_hull3d_begin(int16_t point_count, const real_point3d *points, int16_t vertex_count, vertex3d *vertices, int16_t edge_count, edge3d *edges, int16_t surface_count, surface3d *surfaces);
extern uint8_t convex_hull3d_expand(int16_t point_count, const real_point3d *points, int16_t vertex_count, vertex3d *vertices, int16_t edge_count, edge3d *edges, int16_t surface_count, surface3d *surfaces, int16_t point_index);

uint8_t convex_hull3d(int16_t point_count, const real_point3d *points, int16_t vertex_count, vertex3d *vertices, int16_t edge_count, edge3d *edges, int16_t surface_count, surface3d *surfaces)
{
    if ( !convex_hull3d_begin(point_count, points, vertex_count, vertices, edge_count, edges, surface_count, surfaces) )
        return 0;

    for ( int16_t point_index = 0; point_index < point_count; ++point_index )
    {
        if ( !convex_hull3d_expand(point_count, points, vertex_count, vertices, edge_count, edges, surface_count,
                surfaces, point_index) )
            return 0;
    }

    return 1;
}
