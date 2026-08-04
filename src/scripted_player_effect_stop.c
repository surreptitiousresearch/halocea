/* scripted_player_effect_stop @0x83737858 — stop the scripted player camera-feedback effect, decaying out
 * over `decay` seconds (converted to 30Hz ticks). Sets the ending flag in global_flags. */

#include "headers/player_effect_globals.h"
#include "headers/scripted_effect_flags.h"
#include "headers/blam_data_globals.h"


void scripted_player_effect_stop(float decay)
{
    player_effect_globals->global_flags |= (1u << _scripted_effect_ending_bit);
    __int16 ticks = (unsigned int)(int)(decay * 30.0f) >> 16;
    player_effect_globals->scripted_effect.timer = ticks;
    player_effect_globals->scripted_effect.total_time = ticks;
}
