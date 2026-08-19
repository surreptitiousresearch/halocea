/* sound_calculate_fade @ 0x83716380 — current gain of a sound instance as it fades between a
 * start gain and a target gain over [fade_start_time, fade_stop_time]. With no fade window the gain
 * is full (1.0). An exponential fade function (type 1) curves the interpolation by
 * 1/sound_fade_exponent (and mirrors the curve when fading down). When the fade completes the
 * window is cleared. Returns the interpolated gain. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_fade_mode.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern double pow(double base, double exponent);

/* attested: r3 first use clrlwi 16 @ 0x83716398 => uint16_t datum index; float return (fmr/fmadds
 * into f1, callers consume f1 or call for the window-clearing side effect only) */
float sound_calculate_fade(int sound_index)
{
    sound_datum *snd = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    int fade_stop_time = snd->fade_stop_time;
    int fade_start_time = snd->fade_start_time;
    float gain;

    if ( fade_start_time == fade_stop_time )
        return 1.0f;

    {
        /* deviation from decompiler: disasm converts render_time and fade_start_time to float
         * SEPARATELY (fcfid+frsp each @ 0x837163E8..0x83716414) and subtracts in single precision;
         * the denominator subtracts in int first (subf @ 0x837163C8). */
        float t = ((float)sound_manager_globals.render_time - (float)fade_start_time)
                  / (float)(fade_stop_time - fade_start_time);
        if ( t >= 0.0f )
        {
            if ( t > 1.0f )
                t = 1.0f;
        }
        else
        {
            t = 0.0f;
        }

        if ( snd->fade_mode == _sound_fade_mode_crossfade )
        {
            float exponent = 1.0f / sound_fade_exponent;
            if ( snd->fade_interpolation_end <= snd->fade_interpolation_start )
                /* fading down: mirror the curve. Deviation: the outer subtraction is DOUBLE
                 * precision in the binary (fsub 1.0 - pow, then frsp @ 0x83716480). */
                t = (float)(1.0 - pow(1.0f - t, exponent));
            else
                t = (float)pow(t, exponent);
        }

        if ( t == 1.0f )
        {
            snd->fade_stop_time = 0;
            snd->fade_start_time = 0;
        }

        gain = (snd->fade_interpolation_end - snd->fade_interpolation_start) * t + snd->fade_interpolation_start;
    }
    return gain;
}
