/* collision_surface_test_point @ 0x837E0AB0 — confirm a projected 2D point lies inside a collision
 * surface's edge loop (and that the surface is not a destroyed breakable surface). Walks the surface's
 * directed edge ring; for each edge it computes the signed area of the point against the edge in the
 * surface's projection plane. The point is inside (returns 1) only if it is on the inner side of
 * every edge; a single outer-side edge (cross > 0) fails (returns 0). A breakable surface whose flag
 * is set (destroyed) also fails.
 *
 * Surface (12 bytes): +4 first edge index, +8 flags (bit3 = breakable), +9 breakable surface index.
 * Edge (24 bytes): [0] start vertex, [1] end vertex, [2] reverse-next edge, [3] forward-next edge,
 * [5] forward surface index. Vertex = 4 floats (16 bytes). Projection axes come from
 * global_projection3d_mappings[axis][sign]. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_surface_flags.h"
#include "headers/real_point2d.h"

extern const int16_t global_projection3d_mappings[3][2][2];

/* Return is 8-bit: both callers (collision_leaf_test_vector @0x837E15A0,
 * bsp3d_test_pill_recursive @0x837E29F0) normalize the result with clrlwi rN,r3,24. */
uint8_t collision_surface_test_point(const collision_bsp *bsp, int16_t breakable_surface_count,
                                     const uint8_t *breakable_surface_flags, int surface_index,
                                     int16_t projection_axis, uint8_t projection_sign,
                                     const real_point2d *point)
{
    const collision_surface *surface = &((const collision_surface *)bsp->surfaces.address)[surface_index];

    /* skip surfaces that are breakable and currently destroyed */
    if ( (surface->flags & (1u << _collision_surface_breakable_bit)) == 0
      || surface->breakable_surface_index >= breakable_surface_count
      || ((1 << (surface->breakable_surface_index & 0x1F))
          & *(int *)&breakable_surface_flags[(surface->breakable_surface_index >> 3) & 0x1FFFFFFC]) != 0 )
    {
        int first_edge = surface->first_edge_index;
        char *edges = (char *)bsp->edges.address;
        float *vertices = (float *)bsp->vertices.address;
        int map_index = 2 * projection_axis + projection_sign;
        int map_x = global_projection3d_mappings[0][map_index][0];
        int map_y = global_projection3d_mappings[0][map_index][1];
        float px = point->n[0];
        float py = point->n[1];
        int edge_index = first_edge;

        while ( 1 )
        {
            int *edge = (int *)&edges[24 * edge_index];
            int forward = (surface_index == edge[5]);            /* this surface is the edge's forward side */
            int from_vertex = forward ? edge[0] : edge[1];
            float *to_vertex = &vertices[4 * (forward ? edge[1] : edge[0])];
            float *from = &vertices[4 * from_vertex];

            float cross = (from[map_y] - to_vertex[map_y]) * (px - to_vertex[map_x])
                        - (from[map_x] - to_vertex[map_x]) * (py - to_vertex[map_y]);
            if ( cross > 0.0f )
                break;                                           /* point is outside this edge */

            edge_index = forward ? edge[3] : edge[2];            /* next edge around the loop */
            if ( edge_index == first_edge )
                return 1;                                        /* closed the loop: inside */
        }
    }
    return 0;
}
