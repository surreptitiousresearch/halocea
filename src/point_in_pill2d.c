/* point_in_pill2d @0x836FD5E8 */
#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

/* point_in_pill2d — is `point` within `width` of the capsule (pill) whose axis runs from
 * `base` along `height`? Projects the point onto the axis, clamps the parameter to [0,1] so the
 * projection stays on the segment, then compares the squared perpendicular distance to width^2. */
uint8_t point_in_pill2d(const real_point2d *point, const real_point2d *base, const real_vector2d *height, float width)
{
    float t;
    float clamp;
    float dx;
    float dy;

    clamp = 0.0f;
    t = ((height->n[0] * (point->n[0] - base->n[0])) + (height->n[1] * (point->n[1] - base->n[1])))
      / ((height->n[0] * height->n[0]) + (height->n[1] * height->n[1]));
    if (t < 0.0f || (clamp = 1.0f, t > 1.0f))
        t = clamp;

    dy = (-t * height->n[1]) + (point->n[1] - base->n[1]);
    dx = (height->n[0] * -t) + (point->n[0] - base->n[0]);
    return ((dx * dx) + (dy * dy)) <= (width * width);
}
