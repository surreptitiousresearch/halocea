/* scripted_player_control_set_camera_control @0x836DE4C8 — enable or disable player camera control
 * (player_control_globals control_flags bit _player_control_camera_off_bit: set = camera control
 * suppressed). Returns the argument. */

#include "headers/player_control_globals.h"
#include "headers/player_control_camera_off_flags.h"
#include "headers/blam_data_globals.h"


char scripted_player_control_set_camera_control(char enabled)
{
    unsigned int control_flags = player_control_globals->control_flags;
    player_control_globals->control_flags = enabled
        ? (control_flags & ~(1u << _player_control_camera_off_bit))
        : (control_flags | (1u << _player_control_camera_off_bit));
    return enabled;
}
