/* quaternion_from_angle_and_vector @0x836FCF38 — build a quaternion representing a rotation of `angle` radians
 * about `axis` (assumed unit length): w = cos(angle/2), xyz = axis * sin(angle/2). n[3]=w. */

#include <math.h>
#include "headers/real_quaternion.h"
#include "headers/real_vector3d.h"

void quaternion_from_angle_and_vector(real_quaternion *q, float angle, const real_vector3d *axis)
{
    float half_angle = angle * 0.5f;
    float sine = sinf(half_angle);
    q->w = cosf(half_angle);
    q->v.n[0] = axis->n[0] * sine;
    q->v.n[1] = axis->n[1] * sine;
    q->v.n[2] = axis->n[2] * sine;
}
