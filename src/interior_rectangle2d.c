/* interior_rectangle2d @0x8379A2B8 — true if `inner` fits entirely within `outer` (rectangle2d.n =
 * [top, left, bottom, right]). */

#include <stdint.h>
#include "headers/rectangle2d.h"

uint8_t interior_rectangle2d(const rectangle2d *outer, const rectangle2d *inner)
{
    if ( inner->n[1] < outer->n[1] )
        return 0;
    if ( inner->n[3] > outer->n[3] )
        return 0;
    if ( inner->n[0] < outer->n[0] )
        return 0;
    if ( inner->n[2] > outer->n[2] )
        return 0;
    return 1;
}
