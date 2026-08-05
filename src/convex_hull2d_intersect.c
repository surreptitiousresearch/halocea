/* convex_hull2d_intersect @0x837FF8? — intersect two convex 2D polygons (p and q) by successively clipping q
 * against each edge-line of p (Sutherland-Hodgman). For each edge of p a clip line is built from the edge's two
 * vertices and q is clipped to its inside half-plane; the surviving polygon feeds the next edge. The clip output
 * ping-pongs between two scratch buffers, except the final edge writes straight into the caller's result buffer.
 * A degenerate edge (no valid line) copies q through unchanged. Iteration stops early if the working polygon is
 * emptied. Returns the vertex count of the intersection, or -1 if a clip overflowed maximum_count.
 *
 * DEVIATION: the decompiler models the scratch frame as real_plane2d[9] and carves the two output buffers from
 * it at a 0x1000 stride (selected by the edge index's low bit). Reproduced as an explicit clip plane plus a
 * two-slot ping-pong buffer array; each slot holds up to maximum_count (<= 512) vertices, matching the stride. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_plane2d.h"

extern void *memcpy(void *destination, const void *source, unsigned int count);
extern real_plane2d *plane2d_from_points(real_plane2d *plane, const real_point2d *point0, const real_point2d *point1);
extern int16_t convex_polygon2d_clip_to_plane(int16_t count, const real_point2d *points, const real_plane2d *plane, int16_t maximum_count, real_point2d *result, uint32_t *clip_flags, uint8_t *clipped, float epsilon);

int16_t convex_hull2d_intersect(int16_t p_count, const real_point2d *p, int q_count, const real_point2d *q, int16_t maximum_count, real_point2d *result, float epsilon)
{
    int current_count = q_count;
    if (p_count <= 0)
        return current_count;

    real_plane2d clip_line;
    real_point2d ping_pong[2][512];

    int active_count = (int16_t)q_count;
    int edge_index = 0;
    while (active_count > 0)
    {
        int previous_vertex = (int16_t)edge_index != 0 ? edge_index - 1 : p_count - 1;
        real_point2d *output = (edge_index == p_count - 1) ? result : ping_pong[edge_index & 1];

        if (plane2d_from_points(&clip_line, &p[edge_index], &p[previous_vertex]))
        {
            int clipped_count = convex_polygon2d_clip_to_plane(current_count, q, &clip_line, maximum_count,
                                                               output, nullptr, nullptr, epsilon);
            active_count = (int16_t)clipped_count;
            current_count = clipped_count;
            if ((int16_t)clipped_count == -1)
                return -1;
        }
        else
        {
            memcpy(output, q, 8 * active_count);
        }

        q = output;
        edge_index = (int16_t)(edge_index + 1);
        if (edge_index >= p_count)
            return current_count;
    }
    return current_count;
}
