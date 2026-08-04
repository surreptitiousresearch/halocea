/* set_rectangle2d @0x83799E48 — initialize a 2D rectangle from (x0,y0,x1,y1) and return it. The fields are
 * stored in the database's y0/x0/y1/x1 order (n[0]=y0, n[1]=x0, n[2]=y1, n[3]=x1). */

#include <stdint.h>
#include "headers/rectangle2d.h"

/* Attested void: r3 (rectangle) is never reassigned and all 4 callers ignore the result. */
void set_rectangle2d(rectangle2d *rectangle, int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    rectangle->x0 = x0;
    rectangle->y0 = y0;
    rectangle->x1 = x1;
    rectangle->y1 = y1;
}
