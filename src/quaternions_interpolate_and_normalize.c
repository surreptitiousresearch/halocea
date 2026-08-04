/* quaternions_interpolate_and_normalize @0x836FD2B0 — interpolate two quaternions then renormalize the result. */

#include "headers/real_quaternion.h"

extern void quaternions_interpolate(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);
extern void quaternion_normalize(real_quaternion *q);

void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result)
{
    quaternions_interpolate(q0, q1, t, result);
    quaternion_normalize(result);
}
