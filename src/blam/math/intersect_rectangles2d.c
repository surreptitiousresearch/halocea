/* intersect_rectangles2d @0x8379A0D8 — compute the intersection of two 2D rectangles: x0/y0 = max of the
 * two lower bounds, x1/y1 = min of the two upper bounds. If the result is empty (x0 >= x1 or y0 >= y1),
 * zeroes `intersection` and returns 0; otherwise writes the intersection and returns 1. */

#include "headers/rectangle2d.h"

int intersect_rectangles2d(const rectangle2d *r1, const rectangle2d *r2, rectangle2d *intersection)
{
    rectangle2d result;

    result.x0 = r1->x0 <= r2->x0 ? r2->x0 : r1->x0;
    result.x1 = r1->x1 > r2->x1 ? r2->x1 : r1->x1;
    result.y0 = r1->y0 <= r2->y0 ? r2->y0 : r1->y0;
    result.y1 = r1->y1 > r2->y1 ? r2->y1 : r1->y1;

    if ( result.x0 >= result.x1 || result.y0 >= result.y1 )
    {
        intersection->y0 = 0;
        intersection->x0 = 0;
        intersection->y1 = 0;
        intersection->x1 = 0;
        return 0;
    }

    *intersection = result;
    return 1;
}
