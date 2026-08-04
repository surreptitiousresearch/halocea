/* rasterizer_set_near_clip_distance @0x8372492C — set the near clip distance used by the cinematic screen
 * effect, if that subsystem is present. */

#include "headers/cinematic_screen_effect_globals.h"

/* DEVIATION: decompiler widened this arg to double; disasm stores it with stfs (single) — it is a float. */
void rasterizer_set_near_clip_distance(float near_clip_distance)
{
    if ( cinematic_screen_effect_globals )
        cinematic_screen_effect_globals->near_clip_distance = near_clip_distance;
}
