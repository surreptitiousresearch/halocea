/* point_to_line_distance_squared3d @0x836FD678 — squared distance from `point` to the line segment
 * base -> base+height. Projects point onto the segment (t clamped to [0,1]) and returns the squared
 * distance from point to that closest point. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height)
{
    float dx = point->n[0] - base->n[0];
    float dy = point->n[1] - base->n[1];
    float dz = point->n[2] - base->n[2];

    float t = (height->n[0] * dx + height->n[1] * dy + height->n[2] * dz)
            / (height->n[0] * height->n[0] + height->n[1] * height->n[1] + height->n[2] * height->n[2]);

    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

    float delta_x = dx - height->n[0] * t;
    float delta_y = dy - height->n[1] * t;
    float delta_z = dz - height->n[2] * t;

    return delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
}
