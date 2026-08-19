/* collision_surface_test_line2d @0x837E0878 — clip a 2D line (point + direction) against one collision
 * surface's boundary edge loop, recording where the line enters and exits the surface interior. Walks the
 * surface's edges in loop order; for each edge it computes the signed area terms of the line vs the edge and,
 * from the sign of the denominator and whether this surface is the edge's right surface, classifies the edge
 * crossing as an enter or exit, keeping the latest enter (largest t) and earliest exit (smallest t). Parallel
 * edges (zero denominator) that bound the interior collapse the interval to empty. Returns nonzero when the
 * line misses the surface (enter_t ends up past exit_t).
 *
 * projection_axis / projection_sign are part of the surface-test signature but unused by the 2D boundary walk
 * (the caller has already projected point/vector into the surface plane). Edge fields are indexed as the
 * collision_edge dwords: [0]=start_vertex, [1]=end_vertex, [surface_is_right+2]=next edge in this surface's
 * loop, [!surface_is_right+4]=the opposite surface across the edge. */

#include <stdint.h>

#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/collision_surface_test_line2d_result.h"

uint8_t collision_surface_test_line2d(
    const collision_bsp *bsp,
    int surface_index,
    int16_t projection_axis,
    uint8_t projection_sign,
    const real_point2d *point,
    const real_vector2d *vector,
    collision_surface_test_line2d_result *result)
{
    int first_edge = ((collision_surface *)bsp->surfaces.address)[surface_index].first_edge_index;
    result->enter_t = -3.4028235e38f;
    result->enter_edge_index = -1;
    result->exit_t = 3.4028235e38f;
    result->enter_surface_index = -1;
    int edge_index = first_edge;
    result->exit_edge_index = -1;
    result->exit_surface_index = -1;

    do
    {
        collision_vertex *vertices = (collision_vertex *)bsp->vertices.address;
        collision_edge *edge = &((collision_edge *)bsp->edges.address)[edge_index];
        float *start_point = (float *)&vertices[edge->vertex_indices[0]];
        float *end_point = (float *)&vertices[edge->vertex_indices[1]];

        float delta_x = end_point[0] - start_point[0];
        float delta_y = end_point[1] - start_point[1];
        float denominator = (vector->n[0] * delta_y) - (vector->n[1] * delta_x);
        float numerator = (delta_x * (point->n[1] - start_point[1]))
                        - ((point->n[0] - start_point[0]) * delta_y);

        /* DEVIATION: decompiler indexed `edge` as a raw int array; retyped to named
         * collision_edge members (surface_indices/edge_indices) — same offsets. */
        int surface_is_right = surface_index == edge->surface_indices[1];

        if ( denominator == 0.0f )
        {
            /* parallel edge: only bounds the interval when the point lies on the interior side */
            if ( (numerator < 0.0f) != surface_is_right )
            {
                result->enter_t = 3.4028235e38f;
                result->enter_edge_index = edge_index;
                result->exit_t = -3.4028235e38f;
                result->exit_edge_index = edge_index;
                result->enter_surface_index = edge->surface_indices[!surface_is_right];
                result->exit_surface_index = edge->surface_indices[!surface_is_right];
            }
        }
        else
        {
            float t = numerator / denominator;
            if ( (denominator < 0.0f) == surface_is_right )
            {
                /* exit crossing: keep the earliest */
                if ( result->exit_t > t )
                {
                    result->exit_edge_index = edge_index;
                    result->exit_t = t;
                    result->exit_surface_index = edge->surface_indices[!surface_is_right];
                }
            }
            else
            {
                /* enter crossing: keep the latest */
                if ( result->enter_t < t )
                {
                    result->enter_edge_index = edge_index;
                    result->enter_t = t;
                    result->enter_surface_index = edge->surface_indices[!surface_is_right];
                }
            }
        }

        edge_index = edge->edge_indices[surface_is_right];
    }
    while ( edge_index != first_edge );

    return result->enter_t > result->exit_t;
}
