/* perpendicular3d @ 0x836FC4C0 — produce a (non-normalized) vector perpendicular to `a`. To stay
 * numerically robust it zeroes the component of largest magnitude and swaps/negates the other two,
 * guaranteeing a non-degenerate result regardless of which axis `a` is most aligned with. */

#include "headers/real_vector3d.h"

extern float __fabs(float x);

real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result)
{
    float abs_x = __fabs(a->n[0]);
    float abs_y = __fabs(a->n[1]);
    float abs_z = __fabs(a->n[2]);

    if ( abs_x > abs_y || abs_x > abs_z )
    {
        if ( abs_y > abs_z )
        {
            result->n[0] = a->n[1];
            result->n[1] = -a->n[0];
            result->n[2] = 0.0f;
        }
        else
        {
            result->n[0] = -a->n[2];
            result->n[1] = 0.0f;
            result->n[2] = a->n[0];
        }
    }
    else
    {
        result->n[0] = 0.0f;
        result->n[1] = a->n[2];
        result->n[2] = -a->n[1];
    }
    return result;
}
