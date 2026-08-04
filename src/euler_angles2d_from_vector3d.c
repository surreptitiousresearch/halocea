/* euler_angles2d_from_vector3d @0x836FD428 — convert a 3D direction into a yaw/pitch pair: yaw =
 * atan2(y, x), pitch = atan2(z, sqrt(x^2 + y^2)). Returns the output angles pointer.
 *
 * Deviation: the decompiler exposes phantom trailing params (a3/a4/a5) from the doubled FPR arg slots; the
 * real signature takes only the output angles and the source vector. */

#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
extern float __fsqrts(float);

extern double atan2(double y, double x);

real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector)
{
    angles->n[0] = (float)atan2(vector->n[1], vector->n[0]);
    angles->n[1] = (float)atan2(vector->n[2],
                       __fsqrts(vector->n[1] * vector->n[1] + vector->n[0] * vector->n[0]));
    return angles;
}
