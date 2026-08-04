/* vector3d_from_angle @0x836FD580 — build a unit vector in the XY plane from a heading angle (radians):
 * x = cos(angle), y = sin(angle), z = 0.
 *
 * Deviation: the decompiler's locals are garbled ("local variable allocation has failed"); reconstructed
 * from disasm — sin and cos are both called on the original angle, results stored single-precision. */

#include "headers/real_vector3d.h"

extern double sin(double);
extern double cos(double);

real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle)
{
    float sine = (float)sin(angle);
    vector->n[0] = (float)cos(angle);
    vector->n[1] = sine;
    vector->n[2] = 0.0f;
    return vector;
}
