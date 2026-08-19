/* circle_intersects_sector2d @0x836FF7E8 */
#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t point_in_sector2d(const real_point2d *point, const real_point2d *base, const real_vector2d *direction, float distance, float cosine);

/* Tests whether a circle (center, radius) overlaps a 2D sector whose apex is `base`
 * and whose axis is `direction`. The apex is pulled back along the axis by
 * radius/sin(half-angle) so a plain point-in-sector test accounts for the radius.
 *
 * DEVIATION: the decompiler invented a trailing `normal` pointer parameter and moved the
 * apex/axis roles onto `direction`/`normal`. Disasm 0x836FF7E8-0x836FF820 reads exactly three
 * pointer args (r3/r5/r6) and four FPRs (f1-f4); with the float-slot-skip ABI that is the
 * 7-parameter list below — r5 is the apex (`base`) and r6 the axis (`direction`). */
int circle_intersects_sector2d(
    const real_point2d *center,
    float radius,
    const real_point2d *base,
    const real_vector2d *direction,
    float distance,
    float sine,
    float cosine)
{
    float projection;
    float back_offset;
    real_point2d shifted_apex;

    /* project center onto the sector axis (apex stored in `base`) */
    projection = direction->n[0] * (center->n[0] - base->n[0])
               + direction->n[1] * (center->n[1] - base->n[1]);
    if (projection < -radius)
        return 0;
    if (projection > radius + distance)
        return 0;

    /* pull the apex backward along the axis so the point test accounts for the
     * circle's radius against the angular bounds */
    back_offset = -(radius / sine);
    shifted_apex.n[0] = direction->n[0] * back_offset + base->n[0];
    shifted_apex.n[1] = direction->n[1] * back_offset + base->n[1];

    if (!point_in_sector2d(center, &shifted_apex, direction,
                           (radius / sine) + radius + distance, cosine))
        return 0;
    return 1;
}
