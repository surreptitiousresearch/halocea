/* convex_hull2d_test_vector @0x837FDD38 — Cyrus-Beck style clip of the ray (point + t*vector) against a
 * convex polygon's edges (points[0..count), wrapping): for each edge, the ray's crossing parameter t is
 * folded into the "latest entry" bound (t_in, initially -inf, maximized over edges the ray enters) or
 * "earliest exit" bound (t_out, initially +inf, minimized over edges it exits), depending on which side of
 * the edge the ray direction crosses. An edge (nearly) parallel to the ray fails the test outright unless
 * the ray's origin is already on its inside. Fails as soon as t_in exceeds t_out. On success, optionally
 * writes the final t_in/t_out bounds out and returns 1; otherwise returns 0. A non-positive `count` always
 * succeeds with the initial -inf/+inf bounds. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern float fabsf(float value);

int convex_hull2d_test_vector(int16_t count, const real_point2d *points, const real_point2d *point,
    const real_vector2d *vector, float *t_in_reference, float *t_out_reference)
{
    float t_in = -3.4028235e38f;
    float t_out = 3.4028235e38f;
    int result = 1;

    for ( int16_t i = 0; i < count; i++ )
    {
        int16_t next = (i + 1 < count) ? (i + 1) : 0;
        const real_point2d *a = &points[i];
        const real_point2d *b = &points[next];

        float edge_x = b->n[0] - a->n[0];
        float edge_y = b->n[1] - a->n[1];
        float cross = vector->n[0] * edge_y - vector->n[1] * edge_x;
        float offset = edge_x * (point->n[1] - a->n[1]) - (point->n[0] - a->n[0]) * edge_y;

        if ( fabsf(cross) < 0.0001f )
        {
            if ( offset >= 0.0001f )
            {
                result = 0;
                break;
            }
        }
        else
        {
            float t = offset / cross;
            if ( cross <= 0.0f )
            {
                if ( t_out > t )
                    t_out = t;
            }
            else if ( t_in < t )
            {
                t_in = t;
            }

            if ( t_in > t_out )
            {
                result = 0;
                break;
            }
        }
    }

    if ( result )
    {
        if ( t_in_reference )
            *t_in_reference = t_in;
        if ( t_out_reference )
            *t_out_reference = t_out;
    }

    return result;
}
