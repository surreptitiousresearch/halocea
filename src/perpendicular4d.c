/* perpendicular4d @0x836FC568 — produce a 4D vector perpendicular to the input.
 *
 * Maps (x,y,z,w) -> (z, w, -x, -y). Returns the result pointer. */

#include "headers/real_vector4d.h"

real_vector4d * perpendicular4d(const real_vector4d *a, real_vector4d *result)
{
    result->n[0] =  a->n[2];
    result->n[1] =  a->n[3];
    result->n[2] = -a->n[0];
    result->n[3] = -a->n[1];
    return result;
}
