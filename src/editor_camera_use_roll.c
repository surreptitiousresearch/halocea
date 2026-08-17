/* editor_camera_use_roll @0x837C3B30 */
#include <stdint.h>
#include "headers/flying_camera.h"
#include "headers/blam_data_globals.h"


uint8_t editor_camera_use_roll(uint8_t new_use_roll)
{
    uint8_t previous = use_roll;
    use_roll = new_use_roll;
    if ( !new_use_roll && editor_camera )
        editor_camera->roll = 0.0f;
    return previous;
}
