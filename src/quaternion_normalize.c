/* quaternion_normalize @ 0x836FCEB8 — scale a quaternion to unit length in place. A zero-or-negative
 * squared magnitude collapses to the identity quaternion (0,0,0,1). real_quaternion is { real_vector3d v;
 * float w; } — v is the vector part, w the scalar. */

#include "headers/real_quaternion.h"

extern float __fsqrts(float x);

void quaternion_normalize(real_quaternion *q)
{
    float magnitude_squared = q->w * q->w
        + (q->v.n[2] * q->v.n[2] + (q->v.n[0] * q->v.n[0] + q->v.n[1] * q->v.n[1]));

    if ( magnitude_squared <= 0.0f )
    {
        q->v.n[0] = 0.0f;
        q->v.n[1] = 0.0f;
        q->v.n[2] = 0.0f;
        q->w = 1.0f;
    }
    else
    {
        float inverse_length = 1.0f / __fsqrts(magnitude_squared);
        q->v.n[0] = q->v.n[0] * inverse_length;
        q->v.n[1] = q->v.n[1] * inverse_length;
        q->v.n[2] = q->v.n[2] * inverse_length;
        q->w = q->w * inverse_length;
    }
}
