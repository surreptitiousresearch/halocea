/* player_control_get_facing_direction @0x836DE6B4 — output the unit-space facing direction a local
 * player is currently aiming toward, derived from that player's desired look angles. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/real_vector3d.h"

#include "headers/real_euler_angles2d.h"
extern int local_player_get_player_index(int16_t local_player_index);
extern void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector, const real_euler_angles2d *desired_facing);

real_vector3d *player_control_get_facing_direction(int16_t local_player_index, real_vector3d *direction)
{
    player_control *control = &player_control_globals->players[local_player_index];
    int player_index = local_player_get_player_index(local_player_index);
    player_aiming_vector_from_facing(player_index, direction, &control->desired_angles);
    return direction;
}
