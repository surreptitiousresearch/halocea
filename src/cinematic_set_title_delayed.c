/* cinematic_set_title_delayed @0x836911D8 — queue a cinematic title (subtitle) into the first free of the
 * four active-title slots. The delay (seconds) is converted to a countdown in 30Hz game ticks, stored as a
 * 16-bit timer (the value is negated and shifted to pack a tick count). */

#include <stdint.h>
#include "headers/cinematic_globals.h"

void cinematic_set_title_delayed(int16_t index, float delay)
{
    int slot;
    for ( slot = 0; slot < 4; slot = (__int16)(slot + 1) )
    {
        if ( (unsigned __int16)cinematic_globals->active_titles[slot].title_index == 0xFFFF )
            break;
    }

    if ( slot < 4 )
    {
        cinematic_globals->active_titles[slot].title_index = index;
        cinematic_globals->active_titles[slot].title_timer = (unsigned int)(int)(delay * -30.0f) >> 16;
    }
}
