/* rasterizer_screen_effect_set_filter @0x8372445C — script entry point arming the colour-filter screen
 * effect: stores the light-enhancement and desaturation intensity interpolation endpoints, the
 * desaturation-additive flag, clears the convolution-mask and video-feedback parameters, and sets the
 * [start, end] interpolation times relative to the current game time (ticks * 1/30 s).
 * DEVIATION: the decompiler invented 10 params (a7..a10) from soft-float ABI confusion. The disassembly
 * shows 6: four floats (f1..f4), the additive byte (r7), and the filter time (f5). The int->float tick
 * conversion (fcfid/frsp/fmuls by 0x3d088889 = 1/30) is reproduced as (float)game_time_get()*0.033333335. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"
#include "headers/game_time_constants.h"

extern int game_time_get(void);

void rasterizer_screen_effect_set_filter(float filter_light_enhancement_intensity_lower_bound, float filter_light_enhancement_intensity_upper_bound, float filter_desaturation_intensity_lower_bound, float filter_desaturation_intensity_upper_bound, uint8_t filter_desaturation_is_additive, float filter_time)
{
    struct cinematic_screen_effect_globals *globals = cinematic_screen_effect_globals;
    if ( globals )
    {
        globals->filter_light_enhancement_intensity[0] = filter_light_enhancement_intensity_lower_bound;
        globals->filter_light_enhancement_intensity[1] = filter_light_enhancement_intensity_upper_bound;
        globals->filter_desaturation_intensity[0] = filter_desaturation_intensity_lower_bound;
        globals->filter_desaturation_intensity[1] = filter_desaturation_intensity_upper_bound;
        globals->parameters.video_on = 0;
        globals->parameters.video_overbright_mode = 0;
        globals->parameters.video_scanline_map = 0;
        globals->parameters.video_noise_intensity = 0.0f;
        globals->parameters.video_noise_map = 0;
        globals->parameters.video_noise_map_scale = 0.0f;
        globals->parameters.filter_desaturation_is_additive = filter_desaturation_is_additive;
        globals->parameters.filter_light_enhancement_uses_convolution_mask = 0;
        globals->parameters.filter_desaturation_uses_convolution_mask = 0;
        float now = (float)game_time_get() * SECONDS_PER_TICK;
        globals->filter_time[0] = now;
        globals->filter_time[1] = now + filter_time;
    }
}
