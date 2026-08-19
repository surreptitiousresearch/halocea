/* equal_point2d @0x8379A378 — component-wise equality of two point2d values. */

#include <stdint.h>
#include "headers/point2d.h"

uint8_t equal_point2d(const point2d *p1, const point2d *p2)
{
    if ( p1->n[0] != p2->n[0] )
        return 0;
    return p1->n[1] == p2->n[1];
}
