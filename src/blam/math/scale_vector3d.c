/* scale_vector3d @ 0x8368C880 — scale a 3D vector by a scalar into result;
 * returns result. (r3=vector, f1=scale, r5=result; r4 unused.) */

#include "headers/real_vector3d.h"

real_vector3d * scale_vector3d(const real_vector3d *vector, float scale, real_vector3d *result)
{
    result->n[0] = vector->n[0] * scale;
    result->n[1] = vector->n[1] * scale;
    result->n[2] = vector->n[2] * scale;
    return result;
}
