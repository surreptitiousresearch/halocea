/* vector_from_points2d @0x836FBE20 — build the 2D vector a->b (component-wise b - a). */

#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

real_vector2d * vector_from_points2d(const real_point2d *a, const real_point2d *b, real_vector2d *result)
{
    result->n[0] = b->n[0] - a->n[0];
    result->n[1] = b->n[1] - a->n[1];
    return result;
}
