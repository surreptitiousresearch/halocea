/* cinematic_set_title_delayed @0x836911D8 — queue a cinematic title (subtitle) into the first free of the
 * four active-title slots. The delay (seconds) is converted to a countdown in 30Hz game ticks, stored as a
 * 16-bit timer (the value is negated: delay * -30.0f truncated to int, low halfword stored). */

#include <stdint.h>
#include "headers/cinematic_globals.h"

void cinematic_set_title_delayed(int16_t index, float delay)
{
    int slot;
    for ( slot = 0; slot < 4; slot = (int16_t)(slot + 1) )
    {
        if ( (uint16_t)cinematic_globals->active_titles[slot].title_index == 0xFFFF )
            break;
    }

    if ( slot < 4 )
    {
        cinematic_globals->active_titles[slot].title_index = index;
        /* DEVIATION: fctiwz f13,f0 @0x83691238 / stfd @0x8369123C / lwz r4,back_chain+4 @0x83691240 /
         * sth r4,0xE(r11) @0x83691244 — the LOW halfword of the int result is stored; the decompiler's
         * `>>16` read the high word of the stfd'd double and was a transcription artifact. */
        cinematic_globals->active_titles[slot].title_timer = (int16_t)(int)(delay * -30.0f);
    }
}
