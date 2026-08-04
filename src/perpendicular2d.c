/* perpendicular2d @0x836FC4A0 — the left-hand 2D perpendicular of `a`: (-a.y, a.x). */

#include "headers/real_vector2d.h"

real_vector2d *perpendicular2d(const real_vector2d *a, real_vector2d *result)
{
    result->n[0] = -a->n[1];
    result->n[1] = a->n[0];
    return result;
}
