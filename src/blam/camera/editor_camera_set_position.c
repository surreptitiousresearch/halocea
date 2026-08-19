/* editor_camera_set_position @0x837C3858 — set the free-fly editor camera's position/orientation if it
 * exists, otherwise seed the persistent focus point (also marking it initialized) for the next
 * editor_camera_new. */

#include "headers/flying_camera.h"
#include "headers/real_point3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/editor_camera_focus_t.h"
#include "headers/blam_data_globals.h"


void editor_camera_set_position(const real_point3d *point, const real_euler_angles2d *angles)
{
    if ( editor_camera )
    {
        editor_camera->position = *point;
        editor_camera->orientation = *angles;
    }
    else
    {
        editor_camera_focus.position = *point;
        initialized_0 = 1;
        editor_camera_focus.angles = *angles;
    }
}
