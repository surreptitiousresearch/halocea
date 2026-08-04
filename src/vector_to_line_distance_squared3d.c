/* vector_to_line_distance_squared3d @0x836FD710 — squared distance between two 3D line segments:
 * segment 1 = point -> point+vector, segment 2 = base -> base+height. Returns the minimum squared
 * distance between any pair of points, one on each segment.
 *
 * Three cases (mirrors the disasm):
 *   1. General (segments not parallel): solve the two infinite lines' closest-approach parameters
 *      t1 (on segment 1) and t2 (on segment 2) from the cross product. If both lie in [0,1] the
 *      closest points are interior — return the squared distance between them. Otherwise at least one
 *      closest point is an endpoint: snap the out-of-range segment to its nearest endpoint (0 or 1),
 *      measure that endpoint against the other whole segment, and take the smaller of the two.
 *   2. Parallel / degenerate (|cross|^2 < 1e-4): project each segment onto the other, clamp, and take
 *      the midpoint parameter of the overlap on each; then fall through to the interior distance calc.
 *
 * The two endpoint selectors are `fsel(t, 1.0, 0.0)` = (t >= 0) ? 1.0 : 0.0 — picks endpoint t=1 when
 * the parameter overshot past 1, endpoint t=0 when it went below 0. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);
extern float fabsf(float x);

float vector_to_line_distance_squared3d(const real_point3d *point, const real_vector3d *vector,
                                        const real_point3d *base, const real_vector3d *height)
{
    float bx = base->n[0] - point->n[0];
    float by = base->n[1] - point->n[1];
    float bz = base->n[2] - point->n[2];

    /* cross = height x vector (component grouping preserved from disasm) */
    float cross_a = height->n[0] * vector->n[2] - vector->n[0] * height->n[2];
    float cross_b = height->n[2] * vector->n[1] - height->n[1] * vector->n[2];
    float cross_c = vector->n[0] * height->n[1] - height->n[0] * vector->n[1];
    float cross_length_squared = cross_b * cross_b + (cross_c * cross_c + cross_a * cross_a);

    float t1; /* parameter along segment 1 (point + t1*vector) */
    float t2; /* parameter along segment 2 (base  + t2*height) */

    if ( fabsf(cross_length_squared) < 0.0001f )
    {
        /* Parallel / degenerate: project one segment onto the other and take overlap midpoints. */
        t1 = 0.0f;
        t2 = 0.0f;

        float seg1_length_squared = vector->n[0] * vector->n[0]
                                  + (vector->n[1] * vector->n[1] + vector->n[2] * vector->n[2]);
        if ( seg1_length_squared <= 0.0001f )
        {
            t1 = 0.0f;
        }
        else
        {
            float inv1 = 1.0f / seg1_length_squared;
            /* where `base` and `base+height` project onto segment 1 */
            float project_base = (vector->n[0] * bx + (vector->n[1] * by + vector->n[2] * bz)) * inv1;
            float project_base_plus_height =
                inv1 * (vector->n[0] * height->n[0]
                        + (vector->n[2] * height->n[2] + vector->n[1] * height->n[1]))
                + project_base;

            if ( project_base >= 0.0f )
            {
                if ( project_base > 1.0f )
                    project_base = 1.0f;
            }
            else
            {
                project_base = 0.0f;
            }

            if ( project_base_plus_height >= 0.0f )
            {
                if ( project_base_plus_height > 1.0f )
                    project_base_plus_height = 1.0f;
                t1 = (project_base_plus_height + project_base) * 0.5f;
            }
            else
            {
                t1 = (0.0f + project_base) * 0.5f;
            }
        }

        float seg2_length_squared = height->n[0] * height->n[0]
                                  + (height->n[1] * height->n[1] + height->n[2] * height->n[2]);
        if ( seg2_length_squared > 0.0001f )
        {
            float inv2 = 1.0f / seg2_length_squared;
            /* where `point` and `point+vector` project onto segment 2 */
            float project_point = -((height->n[0] * bx + (height->n[2] * bz + height->n[1] * by)) * inv2);
            float project_point_plus_vector =
                inv2 * (vector->n[0] * height->n[0]
                        + (vector->n[2] * height->n[2] + vector->n[1] * height->n[1]))
                + project_point;

            if ( project_point >= 0.0f )
            {
                if ( project_point > 1.0f )
                    project_point = 1.0f;
            }
            else
            {
                project_point = 0.0f;
            }

            float project_point_plus_vector_clamped = 1.0f;
            if ( project_point_plus_vector >= 0.0f )
            {
                if ( project_point_plus_vector <= 1.0f )
                    project_point_plus_vector_clamped = project_point_plus_vector;
            }
            else
            {
                project_point_plus_vector_clamped = 0.0f;
            }
            t2 = (project_point_plus_vector_clamped + project_point) * 0.5f;
        }
    }
    else
    {
        float inv = 1.0f / cross_length_squared;
        float cross_a_inv = cross_a * inv;

        t2 = (vector->n[2] * by - vector->n[1] * bz) * (cross_b * inv)
           + ((vector->n[1] * bx - vector->n[0] * by) * (cross_c * inv)
              + (vector->n[0] * bz - vector->n[2] * bx) * cross_a_inv);

        t1 = (height->n[2] * by - height->n[1] * bz) * (cross_b * inv)
           + ((height->n[1] * bx - height->n[0] * by) * (cross_c * inv)
              + (height->n[0] * bz - height->n[2] * bx) * cross_a_inv);

        int seg1_out_of_range = (t1 < 0.0f || t1 > 1.0f);
        int seg2_out_of_range = (t2 < 0.0f || t2 > 1.0f);

        if ( seg1_out_of_range || seg2_out_of_range )
        {
            float distance1 = 3.4028235e38f; /* FLT_MAX */
            float distance2 = 3.4028235e38f;

            if ( seg1_out_of_range )
            {
                float select = (t1 >= 0.0f) ? 1.0f : 0.0f;
                real_point3d endpoint;
                endpoint.n[0] = vector->n[0] * select + point->n[0];
                endpoint.n[1] = vector->n[1] * select + point->n[1];
                endpoint.n[2] = vector->n[2] * select + point->n[2];
                distance1 = point_to_line_distance_squared3d(&endpoint, base, height);
            }
            if ( seg2_out_of_range )
            {
                float select = (t2 >= 0.0f) ? 1.0f : 0.0f;
                real_point3d endpoint;
                endpoint.n[0] = height->n[0] * select + base->n[0];
                endpoint.n[1] = height->n[1] * select + base->n[1];
                endpoint.n[2] = select * height->n[2] + base->n[2];
                distance2 = point_to_line_distance_squared3d(&endpoint, point, vector);
            }
            return (distance1 <= distance2) ? distance1 : distance2;
        }
    }

    /* Interior closest points on both segments: return their squared separation. */
    float ex = (height->n[0] * t2 + base->n[0]) - (vector->n[0] * t1 + point->n[0]);
    float ey = (height->n[1] * t2 + base->n[1]) - (vector->n[1] * t1 + point->n[1]);
    float ez = (t2 * height->n[2] + base->n[2]) - (vector->n[2] * t1 + point->n[2]);
    return ex * ex + (ez * ez + ey * ey);
}
