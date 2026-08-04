/* find_avoidance_point @0x8381E620 — pick a point near `tangent_points[0]`/`tangent_points[1]` that steers
 * `radius` away from `center`. If the two tangent points and `center` aren't collinear, first tries the
 * circle-circle intersection point equidistant (2*radius) from `center` on the chord through the tangent
 * points; if that point is invalid (collinear) or still lies within `2*radius` of `center`, falls back to
 * offsetting `tangent_points[0]` by `radius` along the direction from `start_point` to `tangent_points[0]`
 * (or `global_left2d` if that direction is degenerate).
 *
 * DEVIATION — dead-float-shadow-GPR pattern: the DB's own decompile/prototype fabricate a 6th parameter
 * (`float *a6`, unnamed) that receives every real write in the body, while the declared 5th parameter
 * (`avoidance_point`, correctly named/typed `real_point2d *`) is never referenced. `radius` (param 4) is a
 * float and consumes a dead shadow at r6, which the decompiler mis-splits into two named parameters (5 and 6)
 * for what disasm_range (0x8381E620-0x8381E754) shows is really just ONE pointer parameter, bound to r7 —
 * i.e. `avoidance_point` IS the true output register; `a6` is discarded below. `radius` is also declared
 * `double` by the decompiler despite every arithmetic op on it being single-precision (`fmuls`/`fsubs`/
 * `fmadds`); reconstructed as `float` per the same precedent established for `normalize2d`. */

#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);
extern float __fsqrts(float x);

void find_avoidance_point(const real_point2d *tangent_points, const real_point2d *center,
        const real_point2d *start_point, float radius, real_point2d *avoidance_point)
{
    float dx0 = tangent_points[0].n[0] - center->n[0];
    float dy0 = tangent_points[0].n[1] - center->n[1];
    float dx1 = tangent_points[1].n[0] - center->n[0];
    float dy1 = tangent_points[1].n[1] - center->n[1];
    float cross = dy1 * dx0 - dy0 * dx1;

    unsigned __int8 have_intersection_point = 0;

    if (__fabs(cross) >= 0.000099999997f)
    {
        have_intersection_point = 1;
        float scale = (radius * radius) / cross;
        avoidance_point->n[1] = (tangent_points[0].n[0] - tangent_points[1].n[0]) * scale + center->n[1];
        avoidance_point->n[0] = center->n[0] - (dy0 - dy1) * scale;
    }

    if (have_intersection_point)
    {
        float ax = avoidance_point->n[0] - center->n[0];
        float ay = avoidance_point->n[1] - center->n[1];
        if (ax * ax + ay * ay <= (radius * radius) * 4.0f)
            return;
    }

    float to_start_x = tangent_points[0].n[0] - start_point->n[0];
    float to_start_y = tangent_points[0].n[1] - start_point->n[1];
    float distance = __fsqrts(to_start_x * to_start_x + to_start_y * to_start_y);

    float direction_x, direction_y;
    if (__fabs(distance) < 0.000099999997f || distance == 0.0f)
    {
        direction_x = global_left2d->n[0];
        direction_y = global_left2d->n[1];
    }
    else
    {
        direction_x = to_start_x / distance;
        direction_y = to_start_y / distance;
    }

    avoidance_point->n[0] = direction_x * radius + tangent_points[0].n[0];
    avoidance_point->n[1] = direction_y * radius + tangent_points[0].n[1];
}
