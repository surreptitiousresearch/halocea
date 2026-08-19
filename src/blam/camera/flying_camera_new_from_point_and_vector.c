/* flying_camera_new_from_point_and_vector @ 0x837C2DB0 — initialize a free-flying camera positioned at
 * a focus point, looking along an orientation vector, with a default field of view. */

#include "headers/flying_camera.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

#include "headers/real_euler_angles2d.h"
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void flying_camera_new_from_point_and_vector(flying_camera *camera, const real_point3d *focus,
                                             const real_vector3d *orientation)
{
    camera->roll = 0.0f;
    camera->field_of_view = 1.2217305f;
    camera->position = *focus;
    euler_angles2d_from_vector3d(&camera->orientation, orientation);
}
