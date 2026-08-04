/* scripted_camera_set_first_person @0x83704ED8 — switch the scripted camera to first-person mode on the given
 * unit. */

#include "headers/camera_script_globals.h"
#include "headers/camera_script_mode.h"

void scripted_camera_set_first_person(int unit_index)
{
    if ( unit_index != -1 )
    {
        camera_script_globals.mode = _camera_script_mode_first_person;
        camera_script_globals.first_update = 1;
        camera_script_globals.relative_object_index = unit_index;
    }
}
