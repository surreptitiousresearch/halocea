/* obstacles_disc_tangents @0x8382FFC0 — computes the two tangent-line directions from `point` to disc
 * `disc_index` (inflated by `radius`, plus a small 1/256 epsilon), using the standard sin/cos-of-half-angle
 * construction (`combined_radius/distance` as sin, clamped to 1 when `point` is inside/on the inflated
 * circle). Also returns the distance from `point` to the near tangent point along the direction-to-center
 * axis. If `point` coincides with the disc center (distance < epsilon), the direction-to-center is treated as
 * the zero vector, degenerating both tangents and `tangent_distance` to 0.
 *
 * DEVIATION — dead-float-shadow-GPR pattern, same as find_avoidance_point/find_tangent_point: `radius` (param
 * 4, float) consumes a dead shadow, shifting every later parameter's Hex-Rays binding one slot early. The DB's
 * own decompile writes two floats through its declared `left_direction` (really `right_direction`'s true
 * register) and through its declared `tangent_distance` (really `left_direction`'s true register, hence being
 * written as a 2-float pair despite its declared `float *` type), while its fabricated 8th parameter `a8`
 * holds the true single-float `tangent_distance` output. Confirmed against the real call site in
 * path_add_steps.c, whose disasm-verified argument registers (r7/r8/r9) match this corrected binding exactly. */

#include <stdint.h>
#include "headers/obstacles.h"
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern double __fabs(double x);
extern float __fsqrts(float x);

void obstacles_disc_tangents(const obstacles *obstacles, int16_t disc_index, const real_point2d *point, float radius,
        real_vector2d *right_direction, real_vector2d *left_direction, float *tangent_distance)
{
    const disc *target_disc = &obstacles->discs[disc_index];

    float to_center_y = target_disc->center.n[1] - point->n[1];
    float to_center_x = target_disc->center.n[0] - point->n[0];
    float distance = __fsqrts(to_center_x * to_center_x + to_center_y * to_center_y);

    if (__fabs(distance) < 0.000099999997f)
    {
        distance = 0.0f;
    }
    else
    {
        float inv_distance = 1.0f / distance;
        to_center_x = inv_distance * (target_disc->center.n[0] - point->n[0]);
        to_center_y = inv_distance * (target_disc->center.n[1] - point->n[1]);
    }

    float sin_half_angle = 1.0f;
    if (distance != 0.0f)
    {
        sin_half_angle = (target_disc->radius + radius + 0.00390625f) / distance;
        if (sin_half_angle > 1.0f)
            sin_half_angle = 1.0f;
    }

    float cos_half_angle = __fsqrts(-(sin_half_angle * sin_half_angle - 1.0f));

    right_direction->n[0] = -((-sin_half_angle * to_center_y) - (cos_half_angle * to_center_x));
    right_direction->n[1] = (-sin_half_angle * to_center_x) + (cos_half_angle * to_center_y);
    left_direction->n[0] = -((sin_half_angle * to_center_y) - (cos_half_angle * to_center_x));
    left_direction->n[1] = (sin_half_angle * to_center_x) + (cos_half_angle * to_center_y);
    *tangent_distance = cos_half_angle * distance;
}
