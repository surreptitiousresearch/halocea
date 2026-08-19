/* editor_camera_set_position_and_roll @0x837C38C8 — place the free-fly editor camera at a point with a given
 * orientation, deriving the roll angle from the requested orientation matrix. If the editor camera exists, its
 * position and yaw/pitch orientation are set from the rotation matrix, and roll is computed as the angle
 * between the matrix's up axis and a reconstructed up vector (from yaw/pitch alone), signed by whether that
 * reconstructed frame agrees with the facing direction. Otherwise the request is stashed into the persistent
 * editor_camera_focus for when the camera is next created. Setting the camera also flags a full reset and, if
 * a unit is focused, records the point as the unit offset.
 *
 * DEVIATION: the decompiler rendered the 12-byte vector copies as an n[0] store plus a QWORD copy of the
 * remaining two components (via IDA's ___u1 / .v / .j union views); reproduced as component-wise assignments.
 * The rotation matrix's forward/up rows use the named real_matrix4x3 union fields. */

#include "headers/flying_camera.h"
#include "headers/editor_camera_focus_t.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_euler_angles3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"


extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern float normalize3d(real_vector3d *v);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);

void editor_camera_set_position_and_roll(const real_point3d *point, const real_euler_angles3d *angles)
{
    if ( editor_camera )
    {
        editor_camera->position.n[0] = point->n[0];
        editor_camera->position.n[1] = point->n[1];
        editor_camera->position.n[2] = point->n[2];

        real_matrix4x3 rotation;
        matrix4x3_rotation_from_angles(&rotation, angles->__s1.yaw, angles->__s1.pitch, angles->__s1.roll);
        euler_angles2d_from_vector3d(&editor_camera->orientation, &rotation.forward);

        real_vector3d facing;
        vector3d_from_euler_angles2d(&facing, &editor_camera->orientation);

        real_euler_angles2d up_orientation = editor_camera->orientation;
        up_orientation.n[1] = up_orientation.n[1] + 1.5707964f; /* +pi/2 pitch */
        real_vector3d up_vector;
        vector3d_from_euler_angles2d(&up_vector, &up_orientation);

        normalize3d(&facing);
        normalize3d(&up_vector);

        real_vector3d roll_reference;
        roll_reference.n[0] = rotation.up.n[2] * up_vector.n[1] - rotation.up.n[1] * up_vector.n[2];
        roll_reference.n[1] = up_vector.n[2] * rotation.up.n[0] - rotation.up.n[2] * up_vector.n[0];
        roll_reference.n[2] = rotation.up.n[1] * up_vector.n[0] - rotation.up.n[0] * up_vector.n[1];
        normalize3d(&roll_reference);

        float agreement = roll_reference.n[1] * facing.n[1]
                        + (roll_reference.n[2] * facing.n[2] + roll_reference.n[0] * facing.n[0]);
        editor_camera->roll = angle_between_vectors3d(&up_vector, &rotation.up) * agreement;

        reset_all = 1;
        if ( unit_focus != -1 )
        {
            unit_offset.n[0] = point->n[0];
            unit_offset.n[1] = point->n[1];
            unit_offset.n[2] = point->n[2];
        }
    }
    else
    {
        initialized_0 = 1;
        reset_all = 1;
        editor_camera_focus.position.n[0] = point->n[0];
        editor_camera_focus.position.n[1] = point->n[1];
        editor_camera_focus.position.n[2] = point->n[2];
        editor_camera_focus.angles = *(const real_euler_angles2d *)angles;
    }
}
