/* angle_between_vectors2d @0x836FC200 — unsigned angle (radians, [0, pi]) between two 2D vectors.
 * Degenerate (either vector zero-length) returns 0. Uses the double-angle identity
 * cos(2*theta) = 2*cos^2(theta) - 1, with cos^2(theta) = dot(a,b)^2 / (|a|^2 * |b|^2), then halves
 * acos(cos(2*theta)) to recover theta and reflects it across pi when the raw dot product is negative
 * (the direct acos(theta) form would be numerically unstable near the poles).
 *
 * DEVIATION: the decompiler shows a phantom 3rd parameter (long double a3) that is never read on entry —
 * disasm confirms the DB's real 2-argument prototype; the "a3" stack slot is just where the compiled code
 * stages the acos() argument. */

#include "headers/real_vector2d.h"
#include "headers/math_constants.h"

extern double acos(double x);

float angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b)
{
    float dot = a->n[0] * b->n[0] + a->n[1] * b->n[1];
    float length_squared_a = a->n[0] * a->n[0] + a->n[1] * a->n[1];
    float length_squared_b = b->n[0] * b->n[0] + b->n[1] * b->n[1];

    if ( length_squared_a * length_squared_b == 0.0f )
        return 0.0f;

    float cos_double_angle = 2.0f * (dot / (length_squared_a * length_squared_b) * dot) - 1.0f;
    if ( cos_double_angle < -1.0f )
        cos_double_angle = -1.0f;
    else if ( cos_double_angle > 1.0f )
        cos_double_angle = 1.0f;

    float angle = (float)(acos(cos_double_angle) * 0.5);
    if ( dot < 0.0f )
        angle = PI - angle;
    return angle;
}
