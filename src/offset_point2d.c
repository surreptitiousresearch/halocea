#include <stdint.h>
#include "headers/point2d.h"

void offset_point2d(point2d *point, int16_t dx, int16_t dy)
{
    __int16 y = point->n[1] + dy;
    point->n[0] += dx;
    point->n[1] = y;
    return;
}
