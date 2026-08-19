/* quaternions_multiply @0x836FD0D8 — Hamilton product result = q0 * q1. If either operand aliases the result
 * buffer it is first copied to a local (the disasm copies the full 16 bytes), so in-place multiplication is safe.
 * Component grouping preserved from the decompiler. n[3]=w. */

#include "headers/real_quaternion.h"

void quaternions_multiply(const real_quaternion *q0, const real_quaternion *q1, real_quaternion *result)
{
    real_quaternion temp;
    if ( q0 == result )
    {
        temp = *q0;
        q0 = &temp;
    }
    if ( q1 == result )
    {
        temp = *q1;
        q1 = &temp;
    }

    result->v.n[0] = -(((q1->v.n[1] * q0->v.n[2]) - ((q0->w * q1->v.n[0]) + ((q1->w * q0->v.n[0]) + (q1->v.n[2] * q0->v.n[1])))));
    result->v.n[1] = -(((q0->v.n[0] * q1->v.n[2]) - ((q0->w * q1->v.n[1]) + ((q0->v.n[2] * q1->v.n[0]) + (q1->w * q0->v.n[1])))));
    result->v.n[2] = -(((q1->v.n[0] * q0->v.n[1]) - ((q0->w * q1->v.n[2]) + ((q1->w * q0->v.n[2]) + (q1->v.n[1] * q0->v.n[0])))));
    result->w = -(((q0->v.n[2] * q1->v.n[2]) - (-((q1->v.n[1] * q0->v.n[1]) - ((q1->w * q0->w) - (q0->v.n[0] * q1->v.n[0]))))));
}
