/* convex_hull2d_verify @0x837FD8D8 — sanity-check a computed 2D convex hull (given as a list of indices
 * into a point array): walks the hull's vertices in order, verifying each turn (via the cross product of
 * consecutive edges) is a left turn (i.e. the hull is convex and wound consistently); bails false as soon
 * as a non-left turn is found. If every turn checks out, sums the interior angle at each vertex and
 * verifies the total is a full turn (2*pi, within a small tolerance) — catching a self-intersecting or
 * degenerate hull that still passed the per-vertex convexity check. An empty hull (hull_count <= 0)
 * trivially satisfies the angle-sum check (0 - 2*pi is never within tolerance... actually reproduces the
 * decompiled behavior verbatim: it compares 0.0 against 2*pi and returns whatever that yields). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/math_constants.h"

extern float __fabs(float x);
extern float angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b);

BOOL convex_hull2d_verify(int16_t point_count, const real_point2d *points, int16_t hull_count, const int16_t *hull_indices)
{
    float angle_sum = 0.0f;

    if ( hull_count <= 0 )
        return __fabs(angle_sum - TWO_PI) < 0.001f;

    for ( __int16 i = 0; i < hull_count; )
    {
        __int16 previous_index = (i - 1 < 0) ? (hull_count - 1) : (i - 1);
        __int16 next_i = (i + 1 >= hull_count) ? 0 : (i + 1);

        const real_point2d *previous_point = &points[hull_indices[previous_index]];
        const real_point2d *current_point = &points[hull_indices[i]];
        const real_point2d *next_point = &points[hull_indices[next_i]];

        real_vector2d incoming_edge;
        incoming_edge.n[0] = current_point->n[0] - previous_point->n[0];
        incoming_edge.n[1] = current_point->n[1] - previous_point->n[1];

        real_vector2d outgoing_edge;
        outgoing_edge.n[0] = next_point->n[0] - current_point->n[0];
        outgoing_edge.n[1] = next_point->n[1] - current_point->n[1];

        float cross = incoming_edge.n[0] * outgoing_edge.n[1] - outgoing_edge.n[0] * incoming_edge.n[1];
        if ( cross < 0.0f )
            return 0;

        angle_sum += angle_between_vectors2d(&incoming_edge, &outgoing_edge);
        i = i + 1;

        if ( i >= hull_count )
            return __fabs(angle_sum - TWO_PI) < 0.001f;
    }

    return 0;
}
