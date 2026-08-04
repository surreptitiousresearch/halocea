/* rasterizer_screen_effect_set_filter_desaturation_tint @0x837244EC — set the RGB tint applied by the
 * screen-effect desaturation filter, when a cinematic screen effect is active.
 * DEVIATION: the decompiler typed the components as double (soft-float ABI); they are floats. The DB
 * models the color as {r,g,b}, equivalent to the decompiler's .n[0..2]. */

#include "headers/cinematic_screen_effect_globals.h"

void rasterizer_screen_effect_set_filter_desaturation_tint(float red, float green, float blue)
{
    struct cinematic_screen_effect_globals *globals = cinematic_screen_effect_globals;
    if ( globals )
    {
        globals->parameters.filter_desaturation_tint.red = red;
        globals->parameters.filter_desaturation_tint.green = green;
        globals->parameters.filter_desaturation_tint.blue = blue;
    }
}
