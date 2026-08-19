/* set_real_quaternion @0x8375CCB0 — raw 4-float field setter. */

#include "headers/real_quaternion.h"

real_quaternion *set_real_quaternion(real_quaternion *q, float i, float j, float k, float w)
{
    q->v.n[0] = i;
    q->v.n[1] = j;
    q->v.n[2] = k;
    q->w = w;
    return q;
}
