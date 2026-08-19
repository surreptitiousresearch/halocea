/* convert_to_spherical @0x8379C6F8 — convert a cartesian direction into (theta, phi) spherical angles
 * (inverse of convert_to_cartesian.c): normalizes `in` if its magnitude clears 0.0001, then
 * out->phi = atan2(y, x) (computed as atan(y/x) with a +pi correction for x<0, or +-pi/2 directly when x is
 * within 0.0001 of zero) and out->theta = acos(z).
 *
 * DEVIATION: the decompiler renders `in` (passed by value, packed across two GPRs on this ABI) as a raw
 * float read through a mistyped pointer, with two phantom trailing parameters. DB confirms the real
 * 2-parameter by-value prototype. */

#include "headers/real_vector3d.h"
#include "headers/spherical_coordinate_vector.h"
#include "headers/math_constants.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern double atan(double x);
extern double acos(double x);

void convert_to_spherical(real_vector3d in, spherical_coordinate_vector *out)
{
    float x = in.n[0];
    float y = in.n[1];
    float z = in.n[2];

    float magnitude = __fsqrts(x * x + y * y + z * z);
    if ( __fabs(magnitude) >= 0.0001f )
    {
        float inverse_magnitude = 1.0f / magnitude;
        x *= inverse_magnitude;
        y *= inverse_magnitude;
        z *= inverse_magnitude;
    }

    if ( x > 0.0001f || x < -0.0001f )
    {
        out->phi = (float)atan(y / x);
        if ( x < 0.0f )
            out->phi += PI;
    }
    else
    {
        out->phi = y <= 0.0f ? -1.5707964f : 1.5707964f;
    }

    out->theta = (float)acos(z);
}
