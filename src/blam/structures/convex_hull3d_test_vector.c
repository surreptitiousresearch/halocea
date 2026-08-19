/* convex_hull3d_test_vector @0x837FF640 — Cyrus-Beck style clip of the ray (point + t*vector) against a
 * convex hull's extant bounding half-space planes: for each surface, denom = dot(vector, plane.normal);
 * if it's (nearly) zero the ray is parallel to that plane and the test degenerates to "is point already
 * inside" (dot(point, plane.normal) - plane.d <= epsilon), otherwise t = -((dot(point, plane.normal)
 * - plane.d) / denom) folds into the "latest entry" bound (t_in, maximized over planes where
 * denom > 0) or "earliest exit" bound (t_out, minimized where denom <= 0). Fails as soon as t_in exceeds
 * t_out. On success, optionally writes the final t_in/t_out bounds out and returns 1; otherwise returns 0.
 * point_count/points/vertex_count/vertices/edge_count/edges are unused, matching the sibling
 * convex_hull3d_test_point.c.
 *
 * DEVIATION: the decompiler fabricates 22 extra trailing int parameters (a13..a27, a29, a31, a33); the
 * DB's 12-parameter prototype is ground truth (matches convex_hull3d_test_point.c's already-documented
 * fabricated-trailing-params pattern) — the decompiler's a28/a30/a32/a34 are point/vector/t_in_reference/
 * t_out_reference respectively. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/vertex3d.h"
#include "headers/edge3d.h"
#include "headers/surface3d.h"
#include "headers/blam_data_globals.h"

extern float fabsf(float value);

uint8_t convex_hull3d_test_vector(int16_t point_count, const real_point3d *points, int16_t vertex_count, const vertex3d *vertices, int16_t edge_count, const edge3d *edges, int16_t surface_count, const surface3d *surfaces, const real_point3d *point, const real_vector3d *vector, float *t_in_reference, float *t_out_reference)
{
    float t_in = -3.4028235e38f;
    float t_out = 3.4028235e38f;
    uint8_t result = 1;

    for ( int16_t i = 0; i < surface_count; i++ )
    {
        const surface3d *surface = &surfaces[i];
        if ( !surface->extant )
            continue;

        float denom = vector->n[0] * surface->plane.normal.n[0] + vector->n[1] * surface->plane.normal.n[1]
                    + vector->n[2] * surface->plane.normal.n[2];
        float offset = point->n[0] * surface->plane.normal.n[0] + point->n[1] * surface->plane.normal.n[1]
                     + point->n[2] * surface->plane.normal.n[2] - surface->plane.d;

        if ( fabsf(denom) < 0.0001f )
        {
            if ( offset > global_convex_hull3d_epsilon )
            {
                result = 0;
                break;
            }
        }
        else
        {
            float t = -(offset / denom);
            if ( denom <= 0.0f )
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
