/* quaternions_interpolate @0x836FD210 — normalized linear interpolation (nlerp) between two quaternions, choosing
 * the shortest arc: if the two quaternions point into opposite hemispheres (negative dot product), q1's weight is
 * negated. The q0 weight (1-t) uses the original t; the q1 weight is the sign-adjusted t. n[3]=w. */

#include "headers/real_quaternion.h"

void quaternions_interpolate(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result)
{
    float one_minus_t = 1.0f - t;

    float dot = (q1->w * q0->w)
              + ((q1->v.n[1] * q0->v.n[1])
              + ((q1->v.n[0] * q0->v.n[0]) + (q1->v.n[2] * q0->v.n[2])));
    if ( dot < 0.0f )
        t = -t;

    result->v.n[0] = (q0->v.n[0] * one_minus_t) + (q1->v.n[0] * t);
    result->v.n[1] = (q1->v.n[1] * t) + (one_minus_t * q0->v.n[1]);
    result->v.n[2] = (q1->v.n[2] * t) + (one_minus_t * q0->v.n[2]);
    result->w = (q1->w * t) + (q0->w * one_minus_t);
}
