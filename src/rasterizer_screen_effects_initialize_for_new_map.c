/* rasterizer_screen_effects_initialize_for_new_map @0x83724254 — reset the cinematic screen-effect block for a
 * new map, defaulting its four script-controlled values to 1.0. */

#include <string.h>
#include "headers/cinematic_screen_effect_globals.h"

void rasterizer_screen_effects_initialize_for_new_map(void)
{
    struct cinematic_screen_effect_globals *globals = cinematic_screen_effect_globals;
    if ( globals )
    {
        memset(globals, 0, sizeof(*globals));
        globals->script_values[0] = 1.0f;
        globals->script_values[1] = 1.0f;
        globals->script_values[2] = 1.0f;
        globals->script_values[3] = 1.0f;
    }
}
