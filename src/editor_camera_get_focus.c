/* editor_camera_get_focus @0x837C3778 — read out the editor camera's persistent focus point. */

#include "headers/editor_camera_focus_t.h"
#include "headers/blam_data_globals.h"


void editor_camera_get_focus(real_point3d *position, real_euler_angles2d *angles)
{
    *position = editor_camera_focus.position;
    *angles = editor_camera_focus.angles;
}
