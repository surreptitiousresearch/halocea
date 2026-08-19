/* signed_angle_between_vectors2d @0x836FC158 — return the signed angle (radians) from vector a to vector b,
 * assuming both are unit length. The magnitude is acos(dot), clamped to [-1,1] before the acos; the sign is
 * taken from the 2D cross product (a x b), so a positive result rotates a toward b counter-clockwise.
 *
 * Deviation: the decompiler exposes a trailing `a3` parameter — it is really an FPR-sized scratch local for
 * the clamped dot product, not an argument. The real signature takes two vectors. */

#include "headers/real_vector2d.h"

extern double acos(double x);

float signed_angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b)
{
    float dot = a->n[0] * b->n[0] + b->n[1] * a->n[1];
    float clamped = -1.0f;
    if ( dot >= -1.0f )
    {
        clamped = 1.0f;
        if ( dot <= 1.0f )
            clamped = dot;
    }

    float angle = (float)acos(clamped);
    if ( (b->n[1] * a->n[0]) - (b->n[0] * a->n[1]) < 0.0f )
        angle = -angle;
    return angle;
}
