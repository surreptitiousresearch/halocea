/* player_control_set_facing @0x836DE278 — point a local player's desired look angles along a forward
 * vector, normalizing the yaw into [0, 2pi). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/real_vector3d.h"
#include "headers/math_constants.h"

extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward)
{
    real_euler_angles2d *desired_angles = &player_control_globals->players[local_player_index].desired_angles;
    euler_angles2d_from_vector3d(desired_angles, forward);
    if ( desired_angles->n[0] < 0.0f )
        desired_angles->n[0] = desired_angles->n[0] + TWO_PI;
}
