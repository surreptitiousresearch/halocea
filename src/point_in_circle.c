/* point_in_circle @0x8382F848 */
#include <stdint.h>
#include "headers/real_point2d.h"

uint8_t point_in_circle(const real_point2d *point, const real_point2d *center, float radius)
{
    float dx = center->n[0] - point->n[0];
    float dy = center->n[1] - point->n[1];
    return dx * dx + dy * dy <= radius * radius;
}
