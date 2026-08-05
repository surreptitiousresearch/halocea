/* director_set_camera @ 0x836E48E8 — install a camera proc for a local player, resetting debug
 * controls/input scale. When interpolate is set, arm the camera-change blend pause. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/blam_data_globals.h"

void director_set_camera(
        int16_t local_player_index,
        void (__fastcall *camera_proc)(void *camera_data, const camera_control *, observer_command *),
        uint8_t interpolate)
{
    director *dir = &director_globals.local_players[local_player_index];

    dir->debug_input_scale = 1.0f;
    dir->camera_proc = camera_proc;
    dir->debug_controls = 0;
    if ( interpolate )
        dir->camera_change_pause = 1.0f;
}
