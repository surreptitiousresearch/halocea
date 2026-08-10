/* vector_intersects_pill3d @0x836FE760 — does the ray (point + t*vector, t in [0,1]) come within `width`
 * of the capsule ("pill") whose axis is the segment (base + s*height, s in [0,1])?
 *
 * Classic segment/segment closest-approach test. Let c = vector x height and d = base - point.
 *   - When the ray and the capsule axis are (nearly) parallel, |c|^2 collapses to ~0 and the closest-approach
 *     solve is degenerate. The parallel branch instead projects each segment's two endpoints onto the other
 *     line, clamps the resulting parameters to [0,1], and averages them to pick representative t and s.
 *   - Otherwise the unclamped closest-approach parameters are
 *         t = (d . (height x c)) / |c|^2   (on the ray),
 *         s = (d . (vector x c)) / |c|^2   (on the axis).
 *     If both land in [0,1] the closest points are interior to both segments, so a single squared-distance
 *     <= width^2 test decides it. If a parameter falls outside [0,1] it is clamped to its nearest endpoint
 *     and the corresponding spherical cap is tested with fast_vector_intersects_sphere (the line-vs-sphere
 *     primitive), which models the rounded end of the capsule.
 *
 * DEVIATIONS (disasm-grounded):
 *   - `width` is a float (5th arg in f1; the decompiler widened it to double through the FPR-shadow ABI).
 *   - The two `__asm fsel fX, fcond, f5, f6` are endpoint-clamp selects with f5 = 1.0 (__real_3f800000) and
 *     f6 = 0.0 (__real_00000000): result = (cond >= 0) ? 1.0 : 0.0. They are reached only when the parameter
 *     is already known to be outside [0,1], so the select clamps t>1 -> 1.0 and t<0 -> 0.0 (likewise s).
 *   - epsilon = __real_38d1b717 = 9.9999997e-5f.
 *   - The decompiler's _FP0 / _FP12 / _FP13 register-allocation-failure pseudo-registers are the named
 *     parameters axis_parameter (s) / ray_parameter (t) / the clamped endpoint value. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern double __fabs(double x);
extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);

uint8_t vector_intersects_pill3d(const real_point3d *point, const real_vector3d *vector,
                                         const real_point3d *base, const real_vector3d *height, float width)
{
    const float epsilon = 9.9999997e-5f;

    /* d = base - point */
    float delta_x = base->n[0] - point->n[0];
    float delta_y = base->n[1] - point->n[1];
    float delta_z = base->n[2] - point->n[2];

    /* c = vector x height */
    float cross_x = height->n[2] * vector->n[1] - height->n[1] * vector->n[2];
    float cross_y = height->n[0] * vector->n[2] - vector->n[0] * height->n[2];
    float cross_z = vector->n[0] * height->n[1] - height->n[0] * vector->n[1];
    float cross_magnitude_sq = cross_x * cross_x + cross_y * cross_y + cross_z * cross_z;

    float ray_parameter;   /* t, on the ray   */
    float axis_parameter;  /* s, on the axis  */

    if (__fabs(cross_magnitude_sq) < epsilon)
    {
        /* --- parallel case: ray direction (nearly) collinear with the capsule axis --- */
        float ray_dir_magnitude_sq = vector->n[0] * vector->n[0]
                                    + vector->n[1] * vector->n[1] + vector->n[2] * vector->n[2];
        if (ray_dir_magnitude_sq <= epsilon)
        {
            axis_parameter = 0.0f;
            ray_parameter = 0.0f;
        }
        else
        {
            float inv_ray_dir_sq = 1.0f / ray_dir_magnitude_sq;
            /* ray parameters for the points on the ray closest to the axis' two endpoints (base, base+height) */
            float t_at_base = (vector->n[0] * delta_x + vector->n[2] * delta_z + vector->n[1] * delta_y)
                              * inv_ray_dir_sq;
            float t_at_axis_end = inv_ray_dir_sq
                                  * (vector->n[0] * height->n[0] + vector->n[2] * height->n[2]
                                     + vector->n[1] * height->n[1])
                                  + t_at_base;
            axis_parameter = 0.0f;
            if (t_at_base >= 0.0f) { if (t_at_base > 1.0f) t_at_base = 1.0f; }
            else                     t_at_base = 0.0f;
            if (t_at_axis_end >= 0.0f)
            {
                if (t_at_axis_end > 1.0f) t_at_axis_end = 1.0f;
                ray_parameter = (t_at_axis_end + t_at_base) * 0.5f;
            }
            else
            {
                ray_parameter = (t_at_base + 0.0f) * 0.5f;
            }
        }

        float axis_magnitude_sq = height->n[2] * height->n[2]
                                + height->n[0] * height->n[0] + height->n[1] * height->n[1];
        if (axis_magnitude_sq > epsilon)
        {
            float inv_axis_sq = 1.0f / axis_magnitude_sq;
            /* axis parameters for the points on the axis closest to the ray's two endpoints (point, point+vector) */
            float s_at_point = -((height->n[0] * delta_x + height->n[2] * delta_z + height->n[1] * delta_y)
                                 * inv_axis_sq);
            float s_at_ray_end = inv_axis_sq
                                 * (vector->n[0] * height->n[0] + vector->n[2] * height->n[2]
                                    + vector->n[1] * height->n[1])
                                 + s_at_point;
            if (s_at_point >= axis_parameter) { if (s_at_point > 1.0f) s_at_point = 1.0f; }
            else                                s_at_point = axis_parameter;   /* = 0.0f */
            if (s_at_ray_end >= axis_parameter)
                axis_parameter = (s_at_ray_end <= 1.0f) ? s_at_ray_end : 1.0f;
            axis_parameter = (axis_parameter + s_at_point) * 0.5f;
        }
        goto distance_check;
    }

    /* --- general case: solve for the unclamped closest-approach parameters --- */
    float inv_cross_sq = 1.0f / cross_magnitude_sq;

    /* t = (d . (height x c)) / |c|^2 */
    ray_parameter = (height->n[2] * delta_y - height->n[1] * delta_z) * (cross_x * inv_cross_sq)
                  + (height->n[1] * delta_x - height->n[0] * delta_y) * (cross_z * inv_cross_sq)
                  + (height->n[0] * delta_z - height->n[2] * delta_x) * (cross_y * inv_cross_sq);

    /* s = (d . (vector x c)) / |c|^2 */
    axis_parameter = (vector->n[2] * delta_y - vector->n[1] * delta_z) * (cross_x * inv_cross_sq)
                   + (vector->n[1] * delta_x - vector->n[0] * delta_y) * (cross_z * inv_cross_sq)
                   + (vector->n[0] * delta_z - vector->n[2] * delta_x) * (cross_y * inv_cross_sq);

    char ray_param_outside = (ray_parameter < 0.0f || ray_parameter > 1.0f);
    char axis_param_outside = (axis_parameter < 0.0f || axis_parameter > 1.0f);

    real_point3d clamped_ray_point;
    real_point3d clamped_axis_point;

    if (ray_param_outside)
    {
        /* fsel clamp: t already out of range, so >1 -> 1.0, <0 -> 0.0 */
        float clamped_t = (ray_parameter >= 0.0f) ? 1.0f : 0.0f;
        clamped_ray_point.n[0] = vector->n[0] * clamped_t + point->n[0];
        clamped_ray_point.n[1] = vector->n[1] * clamped_t + point->n[1];
        clamped_ray_point.n[2] = vector->n[2] * clamped_t + point->n[2];
    }
    else if (!axis_param_outside)
    {
        goto distance_check;   /* both parameters interior: closest points are the true closest approach */
    }

    if (axis_param_outside)
    {
        float clamped_s = (axis_parameter >= 0.0f) ? 1.0f : 0.0f;
        clamped_axis_point.n[0] = height->n[0] * clamped_s + base->n[0];
        clamped_axis_point.n[1] = height->n[1] * clamped_s + base->n[1];
        clamped_axis_point.n[2] = height->n[2] * clamped_s + base->n[2];
    }

    /* test the relevant capsule cap(s): the clamped ray endpoint against the axis line, and/or the clamped
     * axis endpoint against the ray line */
    return (ray_param_outside && fast_vector_intersects_sphere(base, height, &clamped_ray_point, width))
        || (axis_param_outside && fast_vector_intersects_sphere(point, vector, &clamped_axis_point, width));

distance_check:
    {
        /* squared distance between the closest points Q(s) = base + s*height and P(t) = point + t*vector */
        float dx = (height->n[0] * axis_parameter + base->n[0]) - (vector->n[0] * ray_parameter + point->n[0]);
        float dy = (height->n[1] * axis_parameter + base->n[1]) - (vector->n[1] * ray_parameter + point->n[1]);
        float dz = (axis_parameter * height->n[2] + base->n[2]) - (vector->n[2] * ray_parameter + point->n[2]);
        if (dx * dx + dz * dz + dy * dy <= width * width)
            return 1;
        return 0;
    }
}
