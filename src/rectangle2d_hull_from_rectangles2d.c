/* rectangle2d_hull_from_rectangles2d @0x8379A1D4 — computes the bounding rectangle (n[0..3] = top, bottom,
 * left, right) enclosing two rectangles. */

#include "headers/rectangle2d.h"

rectangle2d * rectangle2d_hull_from_rectangles2d(const rectangle2d *r1, const rectangle2d *r2, rectangle2d *hull)
{
    hull->n[1] = (r1->n[1] > r2->n[1]) ? r2->n[1] : r1->n[1];
    hull->n[3] = (r1->n[3] <= r2->n[3]) ? r2->n[3] : r1->n[3];
    hull->n[0] = (r1->n[0] > r2->n[0]) ? r2->n[0] : r1->n[0];
    hull->n[2] = (r1->n[2] <= r2->n[2]) ? r2->n[2] : r1->n[2];
    return hull;
}
