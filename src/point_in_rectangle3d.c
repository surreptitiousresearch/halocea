#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_rectangle3d.h"

/* True if `point` lies within (inclusive) the axis-aligned 3D box. */
uint8_t point_in_rectangle3d(const real_point3d *point, const real_rectangle3d *bounds)
{
    if ( point->n[0] < bounds->n[0] )
        return 0;
    if ( point->n[0] > bounds->n[1] )
        return 0;
    if ( point->n[1] < bounds->n[2] )
        return 0;
    if ( point->n[1] > bounds->n[3] )
        return 0;
    if ( point->n[2] < bounds->n[4] )
        return 0;
    if ( point->n[2] > bounds->n[5] )
        return 0;
    return 1;
}
