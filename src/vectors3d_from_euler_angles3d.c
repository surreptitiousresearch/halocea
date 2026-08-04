/* vectors3d_from_euler_angles3d @0x836FD498 — derive forward and up basis vectors from Euler angles by building
 * a rotation matrix and extracting its point-and-vector decomposition (the position output is discarded). */

#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_euler_angles3d.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern void matrix4x3_to_point_and_vectors(const real_matrix4x3 *matrix, real_point3d *point, real_vector3d *forward, real_vector3d *up);

void vectors3d_from_euler_angles3d(real_vector3d *forward, real_vector3d *up,
                                   const real_euler_angles3d *angles)
{
    real_point3d discarded_point;
    real_matrix4x3 rotation;
    matrix4x3_rotation_from_angles(&rotation, angles->n[0], angles->n[1], angles->n[2]);
    matrix4x3_to_point_and_vectors(&rotation, &discarded_point, forward, up);
}
