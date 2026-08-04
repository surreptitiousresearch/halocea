/* uniform_cubic_spline_vector3d @0x837C18E8 — evaluate a uniform cubic spline through four control vectors
 * (f0..f3) at parameter t, with knot spacing h starting at t0; writes the interpolated vector to result.
 *
 * Deviation: the decompiler unrolls the three components (computing the first via a shared temporary and
 * inlining the other two); reproduced as a per-component loop — the float-cast arithmetic is identical. */

#include "headers/real_vector3d.h"

void uniform_cubic_spline_vector3d(real_vector3d *result, const real_vector3d *f0, const real_vector3d *f1,
                                   const real_vector3d *f2, const real_vector3d *f3,
                                   float t0, float h, float t)
{
    float two_h          = (h * (float)2.0);
    float three_h        = (h * (float)3.0);
    float inv_h          = ((float)1.0 / h);
    float t0_plus_two_h  = (two_h + t0);
    float t_minus_t0_h   = (t - (t0 + h));

    for ( int c = 0; c < 3; ++c )
    {
        float d10 = (f1->n[c] - f0->n[c]);
        float d21 = (f2->n[c] - f1->n[c]);
        float d32 = (f3->n[c] - f2->n[c]);
        float second_diff = ((d32 - d21) - (d21 - d10));
        float term = (((((t - t0_plus_two_h) * second_diff) / three_h)
                                     + (d21 - d10))
                             * t_minus_t0_h)
                   / two_h;
        result->n[c] = (((term + d10) * inv_h) * (t - t0)) + f0->n[c];
    }
}
