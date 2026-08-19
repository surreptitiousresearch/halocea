/* orientations_interpolate @0x836FD388 — interpolate between two node orientations.
 *
 * Slerps (via quaternions_interpolate + normalize) the rotation, then linearly interpolates the
 * translation (3 floats) and the uniform scale (1 float) — i.e. every float from offset 0x10 through
 * 0x1C — by the fraction t. */

#include "headers/real_orientation.h"
#include "headers/real_quaternion.h"

extern void quaternions_interpolate(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternion_normalize(real_quaternion *q);

void orientations_interpolate(const real_orientation *a, const real_orientation *b, float t,
                              real_orientation *result)
{
    quaternions_interpolate(&a->rotation, &b->rotation, t, &result->rotation);
    quaternion_normalize(&result->rotation);

    result->translation.n[0] = a->translation.n[0] * (1.0f - t) + b->translation.n[0] * t;
    result->translation.n[1] = a->translation.n[1] * (1.0f - t) + b->translation.n[1] * t;
    result->translation.n[2] = a->translation.n[2] * (1.0f - t) + b->translation.n[2] * t;
    result->scale            = a->scale            * (1.0f - t) + b->scale            * t;
}
