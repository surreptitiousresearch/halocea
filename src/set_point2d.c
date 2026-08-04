/* set_point2d @0x83799E60 — initialize a 2D integer point and return it. */

#include <stdint.h>
#include "headers/point2d.h"

/* Attested void: r3 (point) is never reassigned and the lone caller ignores the result. */
void set_point2d(point2d *point, int16_t x, int16_t y)
{
    point->x = x;
    point->y = y;
}
