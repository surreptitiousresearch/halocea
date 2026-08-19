/* negate_vector2d @0x836C69C8 */
#include "headers/real_vector2d.h"

real_vector2d * negate_vector2d(const real_vector2d *a, real_vector2d *result)
{
    result->n[0] = -a->n[0];
    result->n[1] = -a->n[1];
    return result;
}
