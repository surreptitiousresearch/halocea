/* add_vectors3d @ 0x836C69E8 — component-wise sum of two 3D vectors into result;
 * returns result. */

#include "headers/real_vector3d.h"

real_vector3d * add_vectors3d(const real_vector3d *a, const real_vector3d *b, real_vector3d *result)
{
    result->n[0] = a->n[0] + b->n[0];
    result->n[1] = a->n[1] + b->n[1];
    result->n[2] = a->n[2] + b->n[2];
    return result;
}
