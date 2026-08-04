/* angle_between_vectors3d @ 0x836FC2E8 — unsigned angle (0..pi) between two 3D vectors. Computed
 * robustly via the double-angle identity: cos(2*phi) = 2*cos^2(phi) - 1, where cos^2(phi) =
 * dot^2 / (|a|^2 * |b|^2). acos of that yields 2*phi; halving gives phi, and a negative dot product is
 * folded into the obtuse half (pi - phi). Returns 0 when either vector is zero-length. */

#include "headers/real_vector3d.h"
#include <math.h>
#include "headers/math_constants.h"

float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b)
{
    float a_magnitude_squared = a->n[2] * a->n[2] + (a->n[0] * a->n[0] + a->n[1] * a->n[1]);
    float b_magnitude_squared = b->n[2] * b->n[2] + (b->n[0] * b->n[0] + b->n[1] * b->n[1]);
    float magnitude_squared_product = a_magnitude_squared * b_magnitude_squared;
    if ( magnitude_squared_product == 0.0f )
        return 0.0f;

    float dot = a->n[1] * b->n[1] + (a->n[0] * b->n[0] + a->n[2] * b->n[2]);
    float cos_double_angle = (dot / magnitude_squared_product) * dot * 2.0f - 1.0f;
    if ( cos_double_angle < -1.0f )
        cos_double_angle = -1.0f;
    else if ( cos_double_angle > 1.0f )
        cos_double_angle = 1.0f;

    float half_angle = (float)acos(cos_double_angle) * 0.5f;
    if ( dot < 0.0f )
        half_angle = PI - half_angle;
    return half_angle;
}
