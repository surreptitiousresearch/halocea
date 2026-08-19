/* rasterizer_script_screen_effect_get_value @0x837242E8 — read one of the four script-controlled cinematic
 * screen-effect interpolation values; returns 0 when the globals are absent or the index is out of range. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"

float rasterizer_script_screen_effect_get_value(int16_t index)
{
    float value = 0.0f;
    if (cinematic_screen_effect_globals && index >= 0 && index < 4)
        value = cinematic_screen_effect_globals->script_values[index];
    return value;
}
