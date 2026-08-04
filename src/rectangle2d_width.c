/* rectangle2d_width @0x83799E90 — width of a rectangle = x1 - x0 (n[3] - n[1]). */

#include <stdint.h>
#include "headers/rectangle2d.h"

int16_t rectangle2d_width(const rectangle2d *rectangle)
{
    return (int16_t)(rectangle->n[3] - rectangle->n[1]);
}
