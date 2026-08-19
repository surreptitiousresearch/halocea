/* scripted_player_effect_set_translation @0x837377C0 — set the maximum positional shake (in world units) for
 * the scripted player camera-feedback effect. */

#include "headers/player_effect_globals.h"
#include "headers/blam_data_globals.h"


void scripted_player_effect_set_translation(float x, float y, float z)
{
    player_effect_globals->scripted_effect.max_translation.n[0] = x;
    player_effect_globals->scripted_effect.max_translation.n[1] = y;
    player_effect_globals->scripted_effect.max_translation.n[2] = z;
}
