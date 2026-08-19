/* player_control_permanent_impulse @0x836E04F8 — apply a permanent yaw/pitch impulse to a local player's
 * control by adding it to the desired look angles. */

#include <stdint.h>
#include "headers/real_euler_angles2d.h"

extern void player_control_modify_desired_angles(int16_t local_player_index, float delta_yaw, float delta_pitch);

void player_control_permanent_impulse(int16_t local_player_index, const real_euler_angles2d *delta)
{
    player_control_modify_desired_angles(local_player_index, delta->yaw, delta->pitch);
}
