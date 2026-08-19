/* normals_interpolate @0x836FC798 — spherically interpolate a unit normal from `a` (t=0) to `b` (t=1),
 * writing the result. For numerical stability it rotates whichever endpoint is nearer: for t <= 0.5 it
 * rotates `a` toward `b` by t*angle about (a x b); for t > 0.5 it rotates `b` back toward `a` by
 * (1-t)*angle about (b x a). The rotation is Rodrigues' formula about the normalized cross-product axis; if
 * the endpoints are (anti)parallel (degenerate axis, length < 0.0001) the nearer endpoint is returned
 * unrotated.
 *
 * DEVIATION 1: the DB prototype's 4th arg `result` is the real output pointer (r6, confirmed at
 * 0x836FC7B8 `mr r29,r6`); the float `t` in f1 burns the r5 shadow GPR, which the decompiler surfaced as a
 * phantom extra parameter `a5` — so the decompiler's `a5` IS `result`, and its separately-listed `result`
 * is the phantom. Reconstructed against the DB prototype.
 * DEVIATION 2: the two branches and the trailing block were rendered as long chains of nested float casts
 * and __int64/long-double register puns; traced to (a) a cross product, (b) endpoint selection, and (c)
 * Rodrigues rotation, and rewritten cleanly (arithmetic unchanged). */

#include "headers/real_vector3d.h"

extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern float __fsqrts(float x);
extern double cos(double x);
extern double sin(double x);

void normals_interpolate(const real_vector3d *a, const real_vector3d *b, float t, real_vector3d *result)
{
    float angle = angle_between_vectors3d(a, b);

    real_vector3d axis;
    float rotate_angle;

    if ( t > 0.5f )
    {
        result->n[0] = b->n[0];
        result->n[1] = b->n[1];
        result->n[2] = b->n[2];
        rotate_angle = (1.0f - t) * angle;
        axis.n[0] = b->n[1] * a->n[2] - b->n[2] * a->n[1]; /* (b x a) */
        axis.n[1] = b->n[2] * a->n[0] - a->n[2] * b->n[0];
        axis.n[2] = a->n[1] * b->n[0] - b->n[1] * a->n[0];
    }
    else
    {
        result->n[0] = a->n[0];
        result->n[1] = a->n[1];
        result->n[2] = a->n[2];
        rotate_angle = angle * t;
        axis.n[0] = b->n[2] * a->n[1] - b->n[1] * a->n[2]; /* (a x b) */
        axis.n[1] = b->n[0] * a->n[2] - a->n[0] * b->n[2];
        axis.n[2] = a->n[0] * b->n[1] - b->n[0] * a->n[1];
    }

    float axis_length = __fsqrts(axis.n[1] * axis.n[1] + (axis.n[2] * axis.n[2] + axis.n[0] * axis.n[0]));
    if ( axis_length >= 0.0001f && axis_length != 0.0f )
    {
        float inv_length = 1.0f / axis_length;
        float axis_x = axis.n[0] * inv_length;
        float axis_y = axis.n[1] * inv_length;
        float axis_z = axis.n[2] * inv_length;

        float cos_a = (float)cos(rotate_angle);
        float sin_a = (float)sin(rotate_angle);

        float rx = result->n[0];
        float ry = result->n[1];
        float rz = result->n[2];
        float dot = ry * axis_y + (rx * axis_x + rz * axis_z);
        float one_minus_cos = 1.0f - cos_a;

        result->n[0] = rx * cos_a + dot * one_minus_cos * axis_x - (ry * axis_z - rz * axis_y) * sin_a;
        result->n[1] = ry * cos_a + dot * one_minus_cos * axis_y - (rz * axis_x - rx * axis_z) * sin_a;
        result->n[2] = rz * cos_a + dot * one_minus_cos * axis_z - (rx * axis_y - ry * axis_x) * sin_a;
    }
}
