/* editor_camera_move_to_point @0x837C37D8 — reposition the editor camera 2.5 units back along its current
 * facing from the given point (i.e. so the point sits 2.5 units in front of the camera). */

#include "headers/flying_camera.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_euler_angles2d.h"
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void editor_camera_move_to_point(const real_point3d *point)
{
    real_vector3d forward;

    vector3d_from_euler_angles2d(&forward, &editor_camera->orientation);

    editor_camera->position.n[0] = point->n[0] - forward.n[0] * 2.5f;
    editor_camera->position.n[1] = point->n[1] - forward.n[1] * 2.5f;
    editor_camera->position.n[2] = point->n[2] - forward.n[2] * 2.5f;
}
