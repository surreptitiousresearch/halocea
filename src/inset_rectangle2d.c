#include <stdint.h>
#include "headers/rectangle2d.h"

void inset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy)
{
    rectangle->y0 = rectangle->y0 + dy;
    rectangle->x0 = rectangle->x0 + dx;
    rectangle->y1 = rectangle->y1 - dy;
    rectangle->x1 = rectangle->x1 - dx;
    return;
}
