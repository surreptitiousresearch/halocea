/* rasterizer_screen_effect_set_convolution @0x83724398 — script entry point arming the convolution
 * (radial blur) screen effect: stores the radius interpolation endpoints, the extra-pass count and type,
 * clears the video-feedback parameters, and sets the [start, end] interpolation times relative to the
 * current game time (ticks * 1/30 s).
 * DEVIATION: the decompiler's __SPAIR64__/double juggling is the soft-float int->float conversion of the
 * tick count; reproduced as (float)game_time_get() * 0.033333335. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"
#include "headers/game_time_constants.h"

extern int game_time_get(void);

void rasterizer_screen_effect_set_convolution(int16_t convolution_extra_passes, int16_t convolution_type, float convolution_radius_lower_bound, float convolution_radius_upper_bound, float convolution_time)
{
    struct cinematic_screen_effect_globals *globals = cinematic_screen_effect_globals;
    if ( globals )
    {
        globals->convolution_radius[0] = convolution_radius_lower_bound;
        globals->convolution_radius[1] = convolution_radius_upper_bound;
        globals->parameters.convolution_extra_passes = convolution_extra_passes;
        globals->parameters.video_on = 0;
        globals->parameters.video_overbright_mode = 0;
        globals->parameters.video_scanline_map = 0;
        globals->parameters.video_noise_intensity = 0.0f;
        globals->parameters.video_noise_map = 0;
        globals->parameters.video_noise_map_scale = 0.0f;
        globals->parameters.convolution_type = convolution_type;
        float now = (float)game_time_get() * SECONDS_PER_TICK;
        globals->convolution_time[0] = now;
        globals->convolution_time[1] = now + convolution_time;
    }
}
