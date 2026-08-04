/* nonuniform_cubic_spline_vector3d @0x8380B2F0 — evaluate a cubic through 4 non-uniformly-spaced knots
 * (f0..f3 at parameters t0..t3) at parameter `t`, via Newton's divided-difference formula, component-wise. */

#include "headers/real_vector3d.h"

void nonuniform_cubic_spline_vector3d(real_vector3d *result, const real_vector3d *f0, const real_vector3d *f1,
    const real_vector3d *f2, const real_vector3d *f3, float t0, float t1, float t2, float t3, float t)
{
    float dt10 = t1 - t0;
    float dt21 = t2 - t1;
    float dt32 = t3 - t2;
    float dt20 = t2 - t0;
    float dt31 = t3 - t1;
    float dt30 = t3 - t0;
    float dt_t2 = t - t2;
    float dt_t1 = t - t1;
    float dt_t0 = t - t0;

    for ( int axis = 0; axis < 3; axis++ )
    {
        float d0 = (f1->n[axis] - f0->n[axis]) / dt10;
        float d1 = (f2->n[axis] - f1->n[axis]) / dt21;
        float d2 = (f3->n[axis] - f2->n[axis]) / dt32;
        float d01 = (d1 - d0) / dt20;
        float d12 = (d2 - d1) / dt31;
        float d012 = (d12 - d01) / dt30;

        result->n[axis] = f0->n[axis] + dt_t0 * (d0 + dt_t1 * (d01 + dt_t2 * d012));
    }
}
