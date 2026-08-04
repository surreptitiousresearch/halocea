/* circle_intersects_cone2d @ 0x836FF6D0 — test whether a circle (center, radius) overlaps a 2D cone
 * (a wedge) defined by an apex/base point, an axis direction (unit), a length (distance) along the
 * axis, and the cone half-angle given as its sine/cosine. Projects the center onto the axis; rejects
 * if the projection falls behind the apex (past -radius) or beyond the length plus radius; then does
 * the standard "point vs. infinite cone" squared-distance test widened by the radius.
 *
 * DEVIATION: the DB prototype carries a phantom trailing float* param ("a8") and marks the fp args as
 * double. On this PPC ABI a float argument reserves its GPR slot, so `float radius` reserves r4 and
 * shifts the pointer args down: the base point lands in r5 and the axis-direction vector in r6 (which
 * the decompiler mislabels "a8"). The math — disp = center - base, proj = dot(axis, disp) — confirms
 * r5 is the base point and r6 is the axis. Real signature is the 7-parameter form below (mirrors the
 * circle_intersects_rectangle2d / circle_intersects_triangle2d deviations). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

uint8_t circle_intersects_cone2d(const real_point2d *center, float radius, const real_point2d *base, const real_vector2d *direction, float distance, float sine, float cosine)
{
    float dx = center->n[0] - base->n[0];
    float dy = center->n[1] - base->n[1];
    float projection = direction->n[1] * dy + direction->n[0] * dx;

    if ( projection < -radius )
        return 0;
    if ( projection > radius + distance )
        return 0;

    if ( (radius * radius) + ((radius * sine * 2.0f + projection) * projection)
         < (dx * dx + dy * dy) * cosine * cosine )
        return 0;

    return 1;
}
