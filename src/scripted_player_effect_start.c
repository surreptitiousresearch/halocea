/* scripted_player_effect_start @0x83737810 — begin the scripted player camera-feedback effect at the given
 * maximum intensity, ramping in over `attack` seconds (converted to 30Hz ticks). Sets the active flag (and
 * clears the ending flag) in global_flags. */

#include "headers/player_effect_globals.h"
#include "headers/scripted_effect_flags.h"
#include "headers/blam_data_globals.h"


void scripted_player_effect_start(float max_intensity, float attack)
{
    player_effect_globals->scripted_effect.max_intensity = max_intensity;
    /* set running, clear ending */
    player_effect_globals->global_flags =
        (player_effect_globals->global_flags & ~((1u << _scripted_effect_running_bit) | (1u << _scripted_effect_ending_bit)))
        | (1u << _scripted_effect_running_bit);
    __int16 ticks = (unsigned int)(int)(attack * 30.0f) >> 16;
    player_effect_globals->scripted_effect.timer = ticks;
    player_effect_globals->scripted_effect.total_time = ticks;
}
