/* rasterizer_screen_effect_start @0x83724340 — begin script control of the full-screen post-process
 * effect: optionally (or on first use) reset its parameter block, then take control. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"
#include <string.h>

void rasterizer_screen_effect_start(uint8_t clear)
{
    if ( cinematic_screen_effect_globals )
    {
        if ( clear || !cinematic_screen_effect_globals->initialized )
        {
            memset(cinematic_screen_effect_globals, 0, 0x38u);
            cinematic_screen_effect_globals->initialized = 1;
        }
        cinematic_screen_effect_globals->has_control = 1;
    }
}
