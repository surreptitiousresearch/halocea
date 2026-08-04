/* bounding_rectangles_intersect @0x837C4B20 — classify a 3D AABB (test_rectangle) against another (rectangle):
 * 0 if they are disjoint on any axis, 2 if test_rectangle lies entirely inside rectangle, otherwise 1 (the two
 * overlap but test_rectangle pokes outside). Rectangle layout is [x0,x1,y0,y1,z0,z1]. */

#include <stdint.h>
#include "headers/real_rectangle3d.h"

int16_t bounding_rectangles_intersect(const real_rectangle3d *test_rectangle, const real_rectangle3d *rectangle)
{
    if (rectangle->n[1] < test_rectangle->n[0])
        return 0;
    if (rectangle->n[0] > test_rectangle->n[1])
        return 0;
    if (rectangle->n[3] < test_rectangle->n[2])
        return 0;
    if (rectangle->n[2] > test_rectangle->n[3])
        return 0;
    if (rectangle->n[5] < test_rectangle->n[4])
        return 0;
    if (rectangle->n[4] > test_rectangle->n[5])
        return 0;

    if (test_rectangle->n[0] < rectangle->n[0] || test_rectangle->n[1] > rectangle->n[1]
        || test_rectangle->n[2] < rectangle->n[2] || test_rectangle->n[3] > rectangle->n[3]
        || test_rectangle->n[4] < rectangle->n[4] || test_rectangle->n[5] > rectangle->n[5])
        return 1;

    return 2;
}
