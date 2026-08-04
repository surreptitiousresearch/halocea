/* scripted_camera_object_is_first_person_camera @0x83704F28 — true when the scripted (cinematic) camera is
 * active in first-person mode (mode 2) and bound to the given object. */

#include "headers/camera_script_globals.h"
#include "headers/camera_script_mode.h"
#include <stdint.h>

/* attested: uint8_t return — clrlwi r3,r11,24 @0x83704F5C; both callers byte-normalize.
 * object_index stays int: full-word cmpw vs relative_object_index @0x83704F4C (datum handle). */
uint8_t scripted_camera_object_is_first_person_camera(int object_index)
{
    if ( !camera_script_globals.enabled )
        return 0;
    if ( camera_script_globals.mode != _camera_script_mode_first_person )
        return 0;
    if ( camera_script_globals.relative_object_index != object_index )
        return 0;
    return 1;
}
