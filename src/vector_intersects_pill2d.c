/* vector_intersects_pill2d @0x836FE488 — test whether the 2D segment [point, point+vector] comes within `width`
 * of the 2D "pill" (capsule) whose spine is the segment [base, base+height]; i.e. whether the two segments'
 * closest approach is <= width. When the two segments are (near) parallel (their 2D cross product is ~0) it
 * projects each segment's span onto the other, clamps to [0,1], takes the midpoints of the overlapping ranges,
 * and compares the distance between the resulting representative points to width. Otherwise it solves the
 * line-line intersection for the two parameters; if both lie in [0,1] the segments genuinely cross (return 1),
 * else it snaps each out-of-range parameter to its nearer endpoint and tests that clamped point against the other
 * segment's pill via point_in_pill2d.
 *
 * DEVIATION: reg-alloc-failure decompile. The two `__asm { fsel }` clamps (disasm 0x836FE578/0x836FE598, with
 * f0=1.0 and f13=0.0) snap the out-of-range parameter to the nearer endpoint — `(t >= 0) ? 1.0f : 0.0f` — not a
 * proportional clamp. Per-step (float) casts preserved for single-precision rounding; the reciprocal
 * length-squared terms are computed once each. epsilon = 0.000099999997 = 1e-4. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t point_in_pill2d(const real_point2d *point, const real_point2d *base, const real_vector2d *height, float width);

uint8_t vector_intersects_pill2d(const real_point2d *point, const real_vector2d *vector, const real_point2d *base, const real_vector2d *height, float width)
{
    float cross = ((vector->n[0] * height->n[1]) - (height->n[0] * vector->n[1]));

    if ( __fabs(cross) < 0.000099999997f )
    {
        /* near-parallel: compare closest representative points along each segment */
        float t_vector;
        if ( ((vector->n[1] * vector->n[1]) + (vector->n[0] * vector->n[0])) <= 0.000099999997f )
        {
            t_vector = 0.0f;
        }
        else
        {
            float inv_vector_len_sq =
                    (1.0f / ((vector->n[1] * vector->n[1]) + (vector->n[0] * vector->n[0])));
            float t_base = (((vector->n[1] * (base->n[1] - point->n[1]))
                            + (vector->n[0] * (base->n[0] - point->n[0]))) * inv_vector_len_sq);
            float t_base_end = ((inv_vector_len_sq
                            * ((vector->n[1] * height->n[1]) + (height->n[0] * vector->n[0])))
                    + (((vector->n[1] * (base->n[1] - point->n[1]))
                                    + (vector->n[0] * (base->n[0] - point->n[0]))) * inv_vector_len_sq));

            if ( t_base < 0.0f )
                t_base = 0.0f;
            else if ( t_base > 1.0f )
                t_base = 1.0f;

            if ( t_base_end >= 0.0f )
            {
                if ( t_base_end > 1.0f )
                    t_base_end = 1.0f;
                t_vector = ((t_base_end + t_base) * 0.5f);
            }
            else
            {
                t_vector = ((0.0f + t_base) * 0.5f);
            }
        }

        float t_height;
        if ( ((height->n[1] * height->n[1]) + (height->n[0] * height->n[0])) <= 0.000099999997f )
        {
            t_height = 0.0f;
        }
        else
        {
            float inv_height_len_sq =
                    (1.0f / ((height->n[1] * height->n[1]) + (height->n[0] * height->n[0])));
            float t_point = -(((height->n[0] * (base->n[0] - point->n[0]))
                            + (height->n[1] * (base->n[1] - point->n[1]))) * inv_height_len_sq);
            float t_point_end = ((inv_height_len_sq
                            * ((vector->n[1] * height->n[1]) + (height->n[0] * vector->n[0])))
                    + t_point);

            if ( t_point < 0.0f )
                t_point = 0.0f;
            else if ( t_point > 1.0f )
                t_point = 1.0f;

            if ( t_point_end > 1.0f )
            {
                t_height = ((1.0f + t_point) * 0.5f);
            }
            else
            {
                float t_low = 0.0f;
                if ( t_point_end >= 0.0f )
                    t_low = t_point_end;
                t_height = ((t_low + t_point) * 0.5f);
            }
        }

        float dx = (((height->n[0] * t_height) + base->n[0])
                - ((vector->n[0] * t_vector) + point->n[0]));
        float dy = (((height->n[1] * t_height) + base->n[1])
                - ((vector->n[1] * t_vector) + point->n[1]));
        if ( ((dx * dx) + (dy * dy)) <= (width * width) )
            return 1;
        return 0;
    }

    /* non-parallel: solve the line-line intersection parameters */
    float inv_cross = (1.0f / cross);
    float t_on_vector = (((height->n[1] * (base->n[0] - point->n[0]))
                    - (height->n[0] * (base->n[1] - point->n[1]))) * inv_cross);
    float t_on_height = (((vector->n[1] * (base->n[0] - point->n[0]))
                    - (vector->n[0] * (base->n[1] - point->n[1]))) * inv_cross);

    uint8_t vector_param_out_of_range = t_on_vector < 0.0f || t_on_vector > 1.0f;
    uint8_t height_param_out_of_range = t_on_height < 0.0f || t_on_height > 1.0f;

    real_point2d clamped_on_vector;
    real_point2d clamped_on_height;

    if ( vector_param_out_of_range )
    {
        float snapped = (t_on_vector >= 0.0f) ? 1.0f : 0.0f;   /* fsel: snap to nearer endpoint */
        clamped_on_vector.n[0] = (vector->n[0] * snapped) + point->n[0];
        clamped_on_vector.n[1] = (vector->n[1] * snapped) + point->n[1];
    }
    else if ( !height_param_out_of_range )
    {
        return 1;   /* both parameters inside [0,1] — the segments cross */
    }

    if ( height_param_out_of_range )
    {
        float snapped = (t_on_height >= 0.0f) ? 1.0f : 0.0f;
        clamped_on_height.n[0] = (height->n[0] * snapped) + base->n[0];
        clamped_on_height.n[1] = (height->n[1] * snapped) + base->n[1];
    }

    return (vector_param_out_of_range && point_in_pill2d(&clamped_on_vector, base, height, width))
        || (height_param_out_of_range && point_in_pill2d(&clamped_on_height, point, vector, width));
}
