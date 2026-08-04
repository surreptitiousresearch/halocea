/* circle_intersects_rectangle2d @ 0x836FF200 — test whether a circle (center, radius) overlaps an
 * axis-aligned 2D rectangle. Computes the per-axis distance from the center to the nearest point on
 * the rectangle (0 inside the span, otherwise the gap past the near/far edge), then compares the
 * squared distance to the squared radius.
 *
 * DEVIATION: the DB prototype carries a phantom 4th param ("a4") and marks radius as double. On this
 * PPC ABI a float argument still reserves its GPR slot, so `float radius` reserves r4 and the real
 * rectangle pointer lands in r5 — which the decompiler mislabels as the extra "a4". The rectangle is
 * accessed as x0,x1,y0,y1 (real_rectangle2d), confirming a4 IS the bounds. Real signature is the
 * 3-parameter form below (matching the sibling circle_intersects_triangle2d deviation). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_rectangle2d.h"

uint8_t circle_intersects_rectangle2d(const real_point2d *center, float radius, const real_rectangle2d *bounds)
{
    float dx, dy;

    if ( center->n[0] > bounds->x1 )
        dx = center->n[0] - bounds->x1;
    else if ( center->n[0] >= bounds->x0 )
        dx = 0.0f;
    else
        dx = bounds->x0 - center->n[0];

    if ( center->n[1] > bounds->y1 )
        dy = center->n[1] - bounds->y1;
    else if ( center->n[1] >= bounds->y0 )
        dy = 0.0f;
    else
        dy = bounds->y0 - center->n[1];

    return (dy * dy + dx * dx) <= radius * radius;
}
