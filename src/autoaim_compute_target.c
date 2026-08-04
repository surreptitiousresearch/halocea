/* autoaim_compute_target @0x836C5EA0 — compute the direction/distance/angle from `position` to an
 * autoaim-adjusted point on `object_index`, gated on having clear line of sight. Returns 0 (leaving the
 * outputs partially or fully unwritten) if there's no line of sight or the target coincides with
 * `position`. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern void object_compute_autoaim_target(int object_index, const real_point3d *position, const real_vector3d *direction, real_point3d *target);
extern uint8_t aim_assist_clear_line_of_sight(const real_point3d *p0, const real_point3d *p1, int ignore_object_index, int target_object_index);
extern float normalize3d(real_vector3d *v);
extern double acos(double x);

uint8_t autoaim_compute_target(int object_index, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, real_point3d *target_position, real_vector3d *target_direction, float *target_distance, float *target_angle)
{
    object_compute_autoaim_target(object_index, position, direction, target_position);

    if (!aim_assist_clear_line_of_sight(position, target_position, ignore_object_index, object_index))
        return 0;

    target_direction->n[0] = target_position->n[0] - position->n[0];
    target_direction->n[1] = target_position->n[1] - position->n[1];
    target_direction->n[2] = target_position->n[2] - position->n[2];

    *target_distance = normalize3d(target_direction);
    if (*target_distance == 0.0f)
        return 0;

    float cosine = direction->n[0] * target_direction->n[0]
                 + direction->n[1] * target_direction->n[1]
                 + direction->n[2] * target_direction->n[2];
    if (cosine < -1.0f)
        cosine = -1.0f;
    else if (cosine > 1.0f)
        cosine = 1.0f;

    *target_angle = (float)acos(cosine);
    return 1;
}
