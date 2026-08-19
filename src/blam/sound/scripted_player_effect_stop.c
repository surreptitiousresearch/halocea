/* scripted_player_effect_stop @0x83737858 — stop the scripted player camera-feedback effect, decaying out
 * over `decay` seconds (converted to 30Hz ticks). Sets the ending flag in global_flags. */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/scripted_effect_flags.h"
#include "headers/blam_data_globals.h"


void scripted_player_effect_stop(float decay)
{
    player_effect_globals->global_flags |= (1u << _scripted_effect_ending_bit);
    /* DEVIATION: fctiwz @0x83737878 / stfd @0x8373787C / lwz r7,back_chain+4 @0x83737880 / extsh
     * @0x83737884 / sth x2 — the LOW halfword of the int result is stored; the decompiler's `>>16`
     * read the high word of the stfd'd double and was a transcription artifact. */
    int16_t ticks = (int16_t)(int)(decay * 30.0f);
    player_effect_globals->scripted_effect.timer = ticks;
    player_effect_globals->scripted_effect.total_time = ticks;
}
