/* quaternion_to_angle_and_vector @0x836FCFC8 — decompose a quaternion into an axis (unit vector) and a rotation
 * angle in radians. The axis is the normalized vector part; the angle is 2*atan2(|vector part|, w). If the angle
 * exceeds pi the axis is negated and the angle reflected to 2pi-angle, keeping the angle in [0, pi]. n[3]=w. */

#include <math.h>
#include "headers/real_quaternion.h"
#include "headers/real_vector3d.h"
#include "headers/math_constants.h"

void quaternion_to_angle_and_vector(const real_quaternion *q, float *angle, real_vector3d *axis)
{
    axis->n[0] = q->v.n[0];
    axis->n[1] = q->v.n[1];
    axis->n[2] = q->v.n[2];

    float magnitude = sqrtf((q->v.n[2] * q->v.n[2]) + ((q->v.n[0] * q->v.n[0]) + (q->v.n[1] * q->v.n[1])));
    if ( fabsf(magnitude) < 0.0001f )
    {
        magnitude = 0.0f;
    }
    else
    {
        axis->n[0] = q->v.n[0] * (1.0f / magnitude);
        axis->n[1] = q->v.n[1] * (1.0f / magnitude);
        axis->n[2] = q->v.n[2] * (1.0f / magnitude);
    }

    /* deviation from decompiler: the binary calls double-precision atan2 (bl atan2 @836FD060)
     * and demotes the result (frsp @836FD064) — modeled as (float)atan2, not atan2f. */
    float half_angle = (float)atan2(magnitude, q->w);
    *angle = half_angle * 2.0f;
    if ( (half_angle * 2.0f) > PI )
    {
        axis->n[0] = -axis->n[0];
        axis->n[1] = -axis->n[1];
        axis->n[2] = -axis->n[2];
        *angle = 6.2831855f - *angle;
    }
}
