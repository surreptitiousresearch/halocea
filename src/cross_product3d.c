#include "headers/real_vector3d.h"

/* result = a x b (3D cross product). */
real_vector3d * cross_product3d(const real_vector3d *a, const real_vector3d *b, real_vector3d *result)
{
    result->n[0] = b->n[2] * a->n[1] - a->n[2] * b->n[1];
    result->n[2] = a->n[0] * b->n[1] - b->n[0] * a->n[1];
    result->n[1] = a->n[2] * b->n[0] - b->n[2] * a->n[0];
    return result;
}
