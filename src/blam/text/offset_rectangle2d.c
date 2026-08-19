/* offset_rectangle2d @0x83799F08 — translate a rectangle by (dx, dy) in place and return it. */

#include <stdint.h>
#include "headers/rectangle2d.h"

rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy)
{
    rectangle->x0 += dx;
    rectangle->x1 += dx;
    rectangle->y0 += dy;
    rectangle->y1 += dy;
    return rectangle;
}
