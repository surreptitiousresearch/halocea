/* points_interpolate @0x836FC9A0 — component-wise linear interpolation of two points: result = (1-t)*a + t*b. */

#include "headers/real_point3d.h"

void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result)
{
    result->n[0] = (a->n[0] * (1.0f - t)) + (b->n[0] * t);
    result->n[1] = (a->n[1] * (1.0f - t)) + (b->n[1] * t);
    result->n[2] = (a->n[2] * (1.0f - t)) + (b->n[2] * t);
}
