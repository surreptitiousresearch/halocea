/* negate_vector3d @ 0x836A77A8 — Blam engine.
 * Writes the component-wise negation of a into result and returns result. */

#include "headers/real_vector3d.h"

real_vector3d * negate_vector3d(const real_vector3d *a, real_vector3d *result)
{
    result->n[0] = -a->n[0];
    result->n[1] = -a->n[1];
    result->n[2] = -a->n[2];
    return result;
}
