/* vector_from_points3d @0x8368C848 — build the 3D vector a->b (component-wise b - a). */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

real_vector3d * vector_from_points3d(const real_point3d *a, const real_point3d *b, real_vector3d *result)
{
    result->n[0] = b->n[0] - a->n[0];
    result->n[1] = b->n[1] - a->n[1];
    result->n[2] = b->n[2] - a->n[2];
    return result;
}
