/* convert_to_cartesian @0x8379C800 — convert a (theta, phi) spherical direction into a unit cartesian
 * vector: out = (cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta)). Each of sin(theta)/sin(phi)/
 * cos(theta)/cos(phi) is snapped to exactly 0 when within 0.0001 of it, before combining.
 *
 * DEVIATION: the decompiler garbles this badly (uninitialized locals, `in` read as a raw float instead of
 * a struct). Rebuilt from disasm/DB prototype: `in` is passed by value (theta/phi packed into one 64-bit
 * GPR on this ABI), `out` in r4. */

#include "headers/spherical_coordinate_vector.h"
#include "headers/real_vector3d.h"

extern double sin(double x);
extern double cos(double x);

static float snap_to_zero(float value)
{
    return (value < 0.0001f && value > -0.0001f) ? 0.0f : value;
}

void convert_to_cartesian(spherical_coordinate_vector in, real_vector3d *out)
{
    float sin_theta = snap_to_zero((float)sin(in.theta));
    float sin_phi = snap_to_zero((float)sin(in.phi));
    float cos_theta = snap_to_zero((float)cos(in.theta));
    float cos_phi = snap_to_zero((float)cos(in.phi));

    out->n[0] = cos_phi * sin_theta;
    out->n[1] = sin_phi * sin_theta;
    out->n[2] = cos_theta;
}
