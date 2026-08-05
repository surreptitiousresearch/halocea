/* convex_polygon3d_clip_to_plane @0x837FE2F0 — Sutherland-Hodgman clip of a convex polygon against a
 * single plane, keeping the side where dot(plane.n, point) - plane.d >= 0.
 *
 * Returns the clipped vertex count; 0 if the polygon is discarded entirely (wholly on the negative side,
 * or coplanar-within-epsilon and `keep_coplanar` is false); the original `count` unclipped if the polygon
 * is wholly on the positive side (or coplanar with `keep_coplanar` true); -1 if the clip would have
 * produced more than `maximum_count` vertices (in which case `points` is copied to `result` verbatim, same
 * as the "keep whole" case, leaving the caller to detect the -1 and react).
 *
 * DEVIATION: the DB's own decompiler output has a 9th parameter `a9` that is never assigned anywhere in
 * the body, while the DB's real (and disasm-confirmed) 8th parameter `keep_coplanar` is declared but never
 * read — traced via disasm (`mr r20, r10` at entry, r10 being the last GPR argument once `epsilon`'s float
 * shadows r9), both names refer to the SAME register; `a9`'s only use in the original decompilation is
 * exactly where `keep_coplanar` belongs.
 * DEVIATION: the final `(output_count >= 3) ? output_count : 0` and the whole-polygon-kept/discarded
 * dispatch below reproduce a carry-flag range-check idiom (`__CFADD__`) and a chain of goto-linked
 * conditions in the decompiled output; both simplify to plain comparisons/if-chains with no behavior
 * change (confirmed against the disasm's addc/subfe sequence, which computes exactly `x >= 3`). */

#include <stdint.h>
#include <string.h>
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/ppc_intrinsics.h"

/* After appending a new vertex to `result`, drop it again if it duplicates (within epsilon) the polygon's
 * first vertex or its own immediate predecessor — collapses degenerate near-zero-length edges produced by
 * a plane crossing landing exactly on an existing vertex. */
static int16_t drop_last_point_if_duplicate(real_point3d *result, int16_t output_count, float epsilon)
{
    if (output_count == 1)
        return output_count;

    const real_point3d *last = &result[output_count - 1];
    const real_point3d *prev = &result[output_count - 2];

    uint8_t duplicates_first = __fabs(last->n[0] - result[0].n[0]) < epsilon
                                     && __fabs(last->n[1] - result[0].n[1]) < epsilon
                                     && __fabs(last->n[2] - result[0].n[2]) < epsilon;
    uint8_t duplicates_prev  = __fabs(last->n[0] - prev->n[0]) < epsilon
                                     && __fabs(last->n[1] - prev->n[1]) < epsilon
                                     && __fabs(last->n[2] - prev->n[2]) < epsilon;

    return (duplicates_first || duplicates_prev) ? output_count - 1 : output_count;
}

int16_t convex_polygon3d_clip_to_plane(int16_t count, const real_point3d *points, const real_plane3d *plane, int16_t maximum_count, real_point3d *result, uint8_t *clipped, float epsilon, uint8_t keep_coplanar)
{
    real_point3d local_points[180]; /* self-overlap staging buffer when points == result */

    int16_t          output_count = 0;
    uint8_t  any_positive = 0; /* any vertex clearly beyond +epsilon of the plane */
    uint8_t  any_negative = 0; /* any vertex clearly beyond -epsilon of the plane */

    if (clipped)
        *clipped = 0;

    if (points == result)
    {
        memcpy(local_points, points, 12 * count);
        points = local_points;
    }

    const real_point3d *previous_point = &points[count - 1];
    float previous_distance = plane->n.n[0] * previous_point->n[0]
                             + plane->n.n[1] * previous_point->n[1]
                             + plane->n.n[2] * previous_point->n[2] - plane->d;
    uint8_t previous_side = previous_distance >= 0.0f;

    for (int16_t i = 0; i < count; ++i)
    {
        const real_point3d *current_point = &points[i];
        float signed_distance = plane->n.n[0] * current_point->n[0]
                               + plane->n.n[1] * current_point->n[1]
                               + plane->n.n[2] * current_point->n[2] - plane->d;
        uint8_t current_side = signed_distance >= 0.0f;

        if (signed_distance <= epsilon)
        {
            if (signed_distance < -epsilon)
                any_negative = 1;
        }
        else
        {
            any_positive = 1;
        }

        if (current_side != previous_side)
        {
            if (output_count == maximum_count)
            {
                output_count = -1;
                goto overflow;
            }
            if (clipped)
                *clipped = 1;

            float dx = previous_point->n[0] - current_point->n[0];
            float dy = previous_point->n[1] - current_point->n[1];
            float dz = previous_point->n[2] - current_point->n[2];
            float denominator = plane->n.n[0] * dx + plane->n.n[1] * dy + plane->n.n[2] * dz;
            float t = -signed_distance / denominator;

            if (t >= 0.0f)
            {
                if (t > 1.0f)
                    t = 1.0f;
            }
            else
            {
                t = 0.0f;
            }

            result[output_count].n[0] = t * dx + current_point->n[0];
            result[output_count].n[1] = t * dy + current_point->n[1];
            result[output_count].n[2] = t * dz + current_point->n[2];
            ++output_count;

            output_count = drop_last_point_if_duplicate(result, output_count, epsilon);
        }

        if (current_side)
        {
            if (output_count >= maximum_count)
            {
                output_count = -1;
                goto overflow;
            }

            result[output_count] = *current_point;
            ++output_count;

            output_count = drop_last_point_if_duplicate(result, output_count, epsilon);
        }

        previous_point = current_point;
        previous_side  = current_side;
    }

overflow:
    if (output_count == -1)
    {
        memcpy(result, points, count * 12);
        return output_count;
    }

    int16_t clipped_count = (output_count >= 3) ? output_count : 0;

    if (any_positive || (!any_negative && keep_coplanar))
    {
        if (!any_negative)
        {
            memcpy(result, points, count * 12);
            return count;
        }
        return clipped_count;
    }

    return 0;
}
