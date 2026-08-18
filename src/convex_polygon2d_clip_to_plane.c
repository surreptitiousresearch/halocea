/* convex_polygon2d_clip_to_plane @0x837FDE60 — Sutherland-Hodgman clip of a convex 2D polygon against a
 * single 2D plane (line). Walks the input edges (previous -> current vertex); whenever an edge crosses the
 * plane it emits the interpolated intersection point, and whenever the current vertex is in front of (or on)
 * the plane it keeps that vertex. Vertices that coincide (within epsilon) with the first or the immediately
 * preceding output vertex are dropped, so degenerate edges collapse.
 *
 * Returns the output vertex count, with these special results:
 *   - -1 if the output overflowed `maximum_count` (the original input is copied back into `result`);
 *   - the whole input is kept (and `result` filled with it, returning `count`) when every vertex is in front;
 *   - 0 when the polygon is entirely behind the plane, or the surviving polygon has fewer than 3 vertices.
 * `*clipped` is set when any edge was actually split; `*clip_flags` carries a per-output-vertex bit (an input
 * vertex propagates its incoming flag, an emitted intersection vertex is flagged).
 *
 * DEVIATION: `epsilon` is the float 8th argument (the decompiler widened it to double via the FPR-shadow ABI).
 * The `__CFADD__(output_count - 3, -2147483645) ? 0 : output_count` idiom is `output_count < 3 ? 0 :
 * output_count`. Output index `output_count` is compared as __int16 throughout (it carries the -1 sentinel). */

#include <stdint.h>
#include <string.h>
#include "headers/real_point2d.h"
#include "headers/real_plane2d.h"

extern double __fabs(double x);

int16_t convex_polygon2d_clip_to_plane(int16_t count, const real_point2d *points, const real_plane2d *plane,
                                       int16_t maximum_count, real_point2d *result, uint32_t *clip_flags,
                                       uint8_t *clipped, float epsilon)
{
    int output_count = 0;
    char any_in_front = 0;          /* a vertex strictly in front of the plane (distance >  epsilon) */
    char any_behind = 0;            /* a vertex strictly behind the plane     (distance < -epsilon) */
    unsigned int output_clip_flags = 0;

    if (clipped)
        *clipped = 0;

    /* clip in place safely by snapshotting the input when it aliases the output */
    real_point2d scratch[14];
    if (points == result)
    {
        memcpy(scratch, points, 8 * count);
        points = scratch;
    }

    int vertex_count = count;
    const real_point2d *previous_vertex = &points[vertex_count - 1];
    bool previous_in_front = ((previous_vertex->n[1] * plane->n.n[1] + previous_vertex->n[0] * plane->n.n[0])
                                     - plane->d) >= 0.0f;

    if (count > 0)
    {
        int i = 0;
        do
        {
            const real_point2d *current_vertex = &points[i];
            float current_distance = ((current_vertex->n[1] * plane->n.n[1]
                                              + plane->n.n[0] * current_vertex->n[0]) - plane->d);
            bool current_in_front = current_distance >= 0.0f;

            if (current_distance <= epsilon)
            {
                if (current_distance < -epsilon)
                    any_behind = 1;
            }
            else
            {
                any_in_front = 1;
            }

            /* edge crosses the plane: emit the interpolated intersection point */
            if ((current_distance >= 0.0f) != previous_in_front)
            {
                int emitted_bit_index = output_count; /* DEVIATION: was char; slw @0x837FE044 shifts by the full word, no extsb exists */
                if ((int16_t)output_count == maximum_count)
                    goto overflow;
                if (clipped)
                    *clipped = 1;

                float edge_delta_y = previous_vertex->n[1] - current_vertex->n[1];
                float denominator = plane->n.n[1] * (previous_vertex->n[1] - current_vertex->n[1])
                                  + plane->n.n[0] * (previous_vertex->n[0] - current_vertex->n[0]);
                float t;
                if (denominator == 0.0f)
                {
                    t = 0.0f;
                }
                else
                {
                    t = ((current_vertex->n[1] * plane->n.n[1] + current_vertex->n[0] * plane->n.n[0])
                                - plane->d)
                        * (-1.0f / denominator);
                    if (t >= 0.0f) { if (t > 1.0f) t = 1.0f; }
                    else             t = 0.0f;
                }

                real_point2d *emitted = &result[(int16_t)output_count];
                emitted->n[0] = t * (previous_vertex->n[0] - current_vertex->n[0]) + current_vertex->n[0];
                output_count = (int16_t)(output_count + 1);
                output_clip_flags |= 1 << emitted_bit_index;
                emitted->n[1] = t * edge_delta_y + current_vertex->n[1];

                /* drop the vertex if it coincides with the first or the previous output vertex */
                if (output_count != 1)
                {
                    real_point2d *tail = &result[output_count];
                    if ((__fabs(tail[-1].n[0] - result->n[0]) < epsilon
                         && __fabs(tail[-1].n[1] - result->n[1]) < epsilon)
                        || (__fabs(tail[-1].n[0] - result[output_count - 2].n[0]) < epsilon
                            && __fabs(tail[-1].n[1] - tail[-2].n[1]) < epsilon))
                        output_count = (int16_t)(output_count - 1);
                }
            }

            /* keep the current vertex when it is in front of (or on) the plane */
            if (current_in_front)
            {
                if ((int16_t)output_count == maximum_count)
                {
                overflow:
                    output_count = -1;
                    break;
                }
                real_point2d *kept = &result[(int16_t)output_count];
                kept->n[0] = current_vertex->n[0];
                kept->n[1] = current_vertex->n[1];
                if (!clip_flags || ((1 << i) & *clip_flags) == 0)
                    output_clip_flags &= ~(1 << output_count);
                else
                    output_clip_flags |= 1 << output_count;
                output_count = (int16_t)(output_count + 1);

                if ((int16_t)output_count != 1)
                {
                    real_point2d *tail = &result[(int16_t)output_count];
                    if ((__fabs(tail[-1].n[0] - result->n[0]) < epsilon
                         && __fabs(tail[-1].n[1] - result->n[1]) < epsilon)
                        || (__fabs(tail[-1].n[0] - result[(int16_t)output_count - 2].n[0]) < epsilon
                            && __fabs(tail[-1].n[1] - tail[-2].n[1]) < epsilon))
                        output_count = (int16_t)(output_count - 1);
                }
            }

            previous_vertex = &points[i];
            previous_in_front = current_in_front;
            i = (int16_t)(i + 1);
        } while (i < count);
    }

    if ((int16_t)output_count == -1)
    {
        memcpy(result, points, vertex_count * 8);
    }
    else
    {
        output_count = ((int16_t)output_count < 3) ? 0 : output_count;   /* a valid polygon needs >= 3 verts */
        if (any_in_front)
        {
            if (!any_behind)
            {
                /* entirely in front: nothing was clipped, return the original polygon verbatim */
                memcpy(result, points, vertex_count * 8);
                output_count = count;
            }
        }
        else
        {
            output_count = 0;   /* entirely behind the plane */
        }
    }

    if (clip_flags)
        *clip_flags = output_clip_flags;
    return output_count;
}
