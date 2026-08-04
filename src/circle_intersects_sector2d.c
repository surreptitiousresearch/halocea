#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t point_in_sector2d(const real_point2d *point, const real_point2d *base, const real_vector2d *direction, float distance, float cosine);

/* Tests whether a circle (center, radius) overlaps a 2D sector. `direction` doubles
 * as the sector apex point (n[0],n[1]); `normal` (last arg) is the sector's axis
 * direction used to project the center onto the sector axis. */
int circle_intersects_sector2d(
    const real_point2d *center,
    float radius,
    const real_point2d *base,
    const real_vector2d *direction,
    float distance,
    float sine,
    float cosine,
    const real_vector2d *normal)
{
    float projection;
    float back_offset;
    real_point2d shifted_center;

    /* project center onto sector axis (apex stored in `direction`) */
    projection = normal->n[0] * (center->n[0] - direction->n[0])
               + normal->n[1] * (center->n[1] - direction->n[1]);
    if (projection < -radius)
        return 0;
    if (projection > radius + distance)
        return 0;

    /* push the center outward along the axis by radius/sin(half-angle) so the
     * point test accounts for the circle's radius against the angular bounds */
    back_offset = -(radius / sine);
    shifted_center.n[0] = normal->n[0] * back_offset + direction->n[0];
    shifted_center.n[1] = normal->n[1] * back_offset + direction->n[1];

    if (!point_in_sector2d(center, &shifted_center, normal,
                           (radius / sine) + radius + distance, cosine))
        return 0;
    return 1;
}
