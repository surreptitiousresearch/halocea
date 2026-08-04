/* rasterizer_script_screen_effect_set_value @0x837242D0 — script hook: store one of the four
 * script-controlled screen-effect values, when the effect block exists and the index is in range.
 *
 * DEVIATION: the value arrives as a double via the soft-float ABI; the stored field is a float. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"

void rasterizer_script_screen_effect_set_value(int16_t index, float value)
{
    if ( cinematic_screen_effect_globals && index >= 0 && index < 4 )
        cinematic_screen_effect_globals->script_values[index] = value;
}
