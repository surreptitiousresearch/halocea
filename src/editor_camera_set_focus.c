/* editor_camera_set_focus @0x837C37A8 — set the editor camera's persistent focus point. */

#include "headers/editor_camera_focus_t.h"
#include "headers/blam_data_globals.h"


void editor_camera_set_focus(const real_point3d *position, const real_euler_angles2d *angles)
{
    editor_camera_focus.position = *position;
    editor_camera_focus.angles = *angles;
}
