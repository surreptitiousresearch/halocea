/* player_control_get_facing_angles @0x836DE668 — return a pointer to the desired facing angles (yaw/pitch) of
 * the given local player's control state. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/blam_data_globals.h"


const real_euler_angles2d *player_control_get_facing_angles(int16_t local_player_index)
{
    return &player_control_globals->players[local_player_index].desired_angles;
}
