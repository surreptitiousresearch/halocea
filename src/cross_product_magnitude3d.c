#include "headers/real_vector3d.h"

/* Returns |a x b|, the magnitude of the 3D cross product. */
float cross_product_magnitude3d(const real_vector3d *a, const real_vector3d *b)
{
    float z = a->n[0] * b->n[1] - b->n[0] * a->n[1];
    float x = b->n[2] * a->n[1] - b->n[1] * a->n[2];
    float y = b->n[0] * a->n[2] - a->n[0] * b->n[2];
    return __builtin_sqrtf(y * y + (x * x + z * z));
}
