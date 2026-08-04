/* vector3d_from_euler_angles2d @ 0x836FD4F8 — unit direction vector from a yaw/pitch pair (spherical
 * to cartesian): x = cos(yaw)cos(pitch), y = sin(yaw)cos(pitch), z = sin(pitch). */

#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"

extern double cos(double x);
extern double sin(double x);

real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles)
{
    float cos_pitch = (float)cos(angles->pitch);

    vector->n[0] = (float)cos(angles->yaw) * cos_pitch;
    vector->n[1] = (float)sin(angles->yaw) * cos_pitch;
    vector->n[2] = (float)sin(angles->pitch);
    return vector;
}
