/* observer_apply_rotational_displacement @ 0x8370F3C8 — treat a small rotation vector as axis-angle
 * (direction = axis, magnitude = angle in radians) and rotate both the forward and up basis vectors
 * about it. A near-zero displacement is ignored. The `a4` parameter the decompiler invents is FP scratch
 * for the sin() argument, not a real argument. */

#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern double sin(double x);
extern double cos(double x);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void observer_apply_rotational_displacement(const real_vector3d *rotational_displacement,
                                            real_vector3d *forward,
                                            real_vector3d *up)
{
    real_vector3d axis;
    double angle;

    axis.n[0] = rotational_displacement->n[0];
    axis.n[1] = rotational_displacement->n[1];
    axis.n[2] = rotational_displacement->n[2];

    angle = __fsqrts(((axis.n[2] * axis.n[2])
                  + ((axis.n[0] * axis.n[0]) + (axis.n[1] * axis.n[1]))));

    if ( __fabs(angle) >= 0.000099999997 )
    {
        float inv = 1.0f / (float)angle;
        axis.n[0] = inv * axis.n[0];
        axis.n[1] = inv * axis.n[1];
        axis.n[2] = inv * axis.n[2];
        if ( angle != 0.0 )
        {
            double sin_angle = (float)sin(angle);
            double cos_angle = (float)cos(angle);
            rotate_vector_about_axis(forward, &axis, sin_angle, cos_angle);
            rotate_vector_about_axis(up, &axis, sin_angle, cos_angle);
        }
    }
}
