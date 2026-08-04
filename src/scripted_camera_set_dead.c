/* scripted_camera_set_dead @0x83704F00 — switch the scripted camera to death-cam mode following the given
 * unit. */

#include "headers/camera_script_globals.h"
#include "headers/camera_script_mode.h"

void scripted_camera_set_dead(int unit_index)
{
    if ( unit_index != -1 )
    {
        camera_script_globals.mode = _camera_script_mode_dead;
        camera_script_globals.first_update = 1;
        camera_script_globals.relative_object_index = unit_index;
    }
}
