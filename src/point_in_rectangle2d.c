#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_rectangle2d.h"

/* True if `point` lies within (inclusive) the axis-aligned 2D rectangle. */
uint8_t point_in_rectangle2d(const real_point2d *point, const real_rectangle2d *bounds)
{
    if ( point->n[0] < bounds->n[0] )
        return 0;
    if ( point->n[0] > bounds->n[1] )
        return 0;
    if ( point->n[1] < bounds->n[2] )
        return 0;
    if ( point->n[1] > bounds->n[3] )
        return 0;
    return 1;
}
