/* scripted_player_effect_start @0x83737810 — begin the scripted player camera-feedback effect at the given
 * maximum intensity, ramping in over `attack` seconds (converted to 30Hz ticks). Sets the active flag (and
 * clears the ending flag) in global_flags. */

#include <stdint.h>
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
    /* DEVIATION: fctiwz @0x83737838 / stfd @0x8373783C / lwz r7,back_chain+4 @0x83737840 / extsh
     * @0x83737844 / sth x2 — the LOW halfword of the int result is stored; the decompiler's `>>16`
     * read the high word of the stfd'd double and was a transcription artifact. */
    int16_t ticks = (int16_t)(int)(attack * 30.0f);
    player_effect_globals->scripted_effect.timer = ticks;
    player_effect_globals->scripted_effect.total_time = ticks;
}
