/* rasterizer_get_near_clip_distance @0x83724938 — returns the active near-clip distance: the cinematic
 * screen-effect override when one is set (positive), otherwise the rasterizer default. */

#include "headers/rasterizer_global_defaults.h"
#include "headers/cinematic_screen_effect_globals.h"
#include "headers/blam_data_globals.h"


float rasterizer_get_near_clip_distance(void)
{
    float near_clip = rasterizer_global_defaults.z_near;

    struct cinematic_screen_effect_globals *effect = cinematic_screen_effect_globals;
    if (effect && effect->near_clip_distance > 0.0f)
        near_clip = effect->near_clip_distance;

    return near_clip;
}
