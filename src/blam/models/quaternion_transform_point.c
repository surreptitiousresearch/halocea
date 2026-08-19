/* quaternion_transform_point @0x836FD2E0 — rotate a point by a quaternion (result = q * p * q^-1), expanded into
 * the direct rotation form: diag*p + 2w*(axis x p) + 2*(axis·p)*axis, where diag = 2w^2 - 1. Component grouping
 * preserved from the decompiler. n[3]=w. */

#include "headers/real_quaternion.h"
#include "headers/real_point3d.h"

void quaternion_transform_point(const real_quaternion *q, const real_point3d *p, real_point3d *result)
{
    float two_w = q->w * 2.0f;
    float cross_z = (q->v.n[0] * p->n[1]) - (p->n[0] * q->v.n[1]);
    float diagonal = ((q->w * q->w) * 2.0f) - 1.0f;
    float cross_y = (p->n[0] * q->v.n[2]) - (q->v.n[0] * p->n[2]);
    float dot_twice = ((p->n[2] * q->v.n[2]) + ((p->n[0] * q->v.n[0]) + (p->n[1] * q->v.n[1]))) * 2.0f;

    result->n[0] = (p->n[0] * diagonal)
                 + ((((q->v.n[1] * p->n[2]) - (p->n[1] * q->v.n[2])) * two_w) + (q->v.n[0] * dot_twice));
    result->n[1] = (cross_y * two_w) + ((q->v.n[1] * dot_twice) + (p->n[1] * diagonal));
    result->n[2] = (cross_z * two_w) + ((q->v.n[2] * dot_twice) + (p->n[2] * diagonal));
}
