/* projection_sign_from_vector3d @0x836FBF48 — sign bit for a projection: true
 * when the vector's component along the projection axis is positive. */

#include <stdint.h>
#include "headers/real_vector3d.h"

int projection_sign_from_vector3d(const real_vector3d *n, int16_t projection)
{
    return n->n[projection] > 0.0;
}
