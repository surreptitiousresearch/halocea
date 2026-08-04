/* rasterizer_screen_effect_get_cinematic_parameters @0x83724620 — advance the cinematic ("filth") screen
 * effect's live parameter block for the current frame. When a cinematic effect has control, it computes two
 * game-time interpolation fractions — one over the convolution ramp [convolution_time[0], convolution_time[1]],
 * one over the filter ramp [filter_time[0], filter_time[1]] (each snapping to 1.0 when the endpoints coincide,
 * otherwise clamped to [0,1]) — and interpolates the convolution blur radius, the light-enhancement intensity,
 * and the desaturation intensity between their scripted start/end values. A desaturation tint left at pure black
 * is treated as "unset" and replaced with green. A negligible convolution radius (<=1e-4) zeroes the whole
 * convolution (radius/type/passes); negligible filter intensities once the filter has fully ramped in zero out
 * both intensities. Read each frame by the post-process pass. Sibling of the rasterizer_screen_effect_set_* API.
 *
 * DEVIATION: the DB signature takes and returns a cinematic_screen_effect_globals* but that register (r3) is
 * pure scratch — the argument is clobbered by the game_time_get() calls and the returned pointer is just the
 * globals (or, on the no-control early-out, the unmodified argument). Reproduced with that behavior. The three
 * repeated game_time_get() reads collapse to one (game time is constant within a frame). The 12-byte black-tint
 * compare the decompiler rendered as a HIBYTE byte loop is an inlined memcmp of the real_rgb_color. */

#include <stdint.h>
#include "headers/cinematic_screen_effect_globals.h"
#include "headers/rasterizer_screen_effect_convolution_type.h"
#include "headers/real_rgb_color.h"
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int game_time_get(void);
extern void scalars_interpolate(float a, float b, float t, float *result);
extern void scalars_interpolate_and_clamp_0_to_1(float a, float b, float t, float *result);
extern int16_t main_get_window_count(void);

static float ramp_fraction(int game_time, const float *time_range)
{
    if ( time_range[1] == time_range[0] )
        return 1.0f;
    float fraction = (((float)game_time * SECONDS_PER_TICK) - time_range[0])
            / (time_range[1] - time_range[0]);
    if ( fraction < 0.0f )
        return 0.0f;
    if ( fraction > 1.0f )
        return 1.0f;
    return fraction;
}

struct cinematic_screen_effect_globals * rasterizer_screen_effect_get_cinematic_parameters(struct cinematic_screen_effect_globals *parameters)
{
    struct cinematic_screen_effect_globals *globals = cinematic_screen_effect_globals;
    if ( !globals || !globals->has_control )
        return parameters;

    int game_time = game_time_get();
    float convolution_fraction = ramp_fraction(game_time, globals->convolution_time);
    float filter_fraction = ramp_fraction(game_time, globals->filter_time);

    scalars_interpolate(globals->convolution_radius[0], globals->convolution_radius[1], convolution_fraction,
            &globals->parameters.convolution_radius);
    scalars_interpolate_and_clamp_0_to_1(globals->filter_light_enhancement_intensity[0],
            globals->filter_light_enhancement_intensity[1], filter_fraction,
            &globals->parameters.filter_light_enhancement_intensity);
    scalars_interpolate_and_clamp_0_to_1(globals->filter_desaturation_intensity[0],
            globals->filter_desaturation_intensity[1], filter_fraction,
            &globals->parameters.filter_desaturation_intensity);

    if ( memcmp(&globals->parameters.filter_desaturation_tint, global_real_rgb_black, sizeof(real_rgb_color)) == 0 )
        globals->parameters.filter_desaturation_tint = *global_real_rgb_green;

    if ( globals->parameters.convolution_radius > 0.000099999997f )
    {
        main_get_window_count();
    }
    else
    {
        globals->parameters.convolution_radius = 0.0f;
        globals->parameters.convolution_type = _rasterizer_screen_effect_convolution_type_none;
        globals->parameters.convolution_extra_passes = 0;
    }

    if ( globals->parameters.filter_light_enhancement_intensity <= 0.000099999997f
      && globals->parameters.filter_desaturation_intensity <= 0.000099999997f
      && filter_fraction >= 1.0f )
    {
        globals->parameters.filter_light_enhancement_intensity = 0.0f;
        globals->parameters.filter_desaturation_intensity = 0.0f;
    }

    return globals;
}
