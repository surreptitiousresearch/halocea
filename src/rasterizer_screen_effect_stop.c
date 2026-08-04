/* rasterizer_screen_effect_stop @0x83724600 — relinquish the cinematic screen effect's exclusive control of
 * the post-process pipeline (no-op if the globals are not allocated). */

#include "headers/cinematic_screen_effect_globals.h"

void rasterizer_screen_effect_stop(void)
{
    if ( cinematic_screen_effect_globals )
        cinematic_screen_effect_globals->has_control = 0;
}
