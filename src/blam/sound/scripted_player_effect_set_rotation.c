/* scripted_player_effect_set_rotation @0x837377D8 — set the maximum angular shake for the scripted player
 * camera-feedback effect; the yaw/pitch/roll arguments are given in degrees and stored in radians. */

#include "headers/player_effect_globals.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


void scripted_player_effect_set_rotation(float yaw, float pitch, float roll)
{
    player_effect_globals->scripted_effect.max_rotation.n[0] = yaw * DEG_TO_RAD;
    player_effect_globals->scripted_effect.max_rotation.n[1] = pitch * DEG_TO_RAD;
    player_effect_globals->scripted_effect.max_rotation.n[2] = roll * DEG_TO_RAD;
}
