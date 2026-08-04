/* sound_start_fade @ 0x83716AC0 — set up a timed gain fade on up to two sound instances over
 * `seconds`. The fade window runs from one render tick ago to render_time + seconds*1000 (clamped to
 * at least render_time). `fade_out_sound_index` fades from its current gain down toward its target
 * (start gain captured, target gain set to 1.0); `fade_up_sound_index` fades up from 0. Both
 * stamp the fade window and fade function (mode) into the runtime sound datum.
 *
 * Attested register map (corrects the earlier reconstruction): the binary reads only r3=mode,
 * f1=seconds (float, shadows the r4 GPR slot), r5=fade_out_sound_index (down-fader) and
 * r6=fade_up_sound_index (up-fader). There is no r7/r8-r9 parameter — the prior note claiming r5
 * unused and r6/r7 meaningful was wrong; r5 and r6 are the two live sound-index args. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern float sound_calculate_fade(uint16_t sound_index);

void sound_start_fade(int16_t mode, float seconds, int fade_out_sound_index, int fade_up_sound_index)
{
    int previous_tick = sound_manager_globals.render_time - 1;
    int fade_end_time = (int)((seconds * 1000.0f) + (float)previous_tick);

    if ( fade_end_time <= sound_manager_globals.render_time )
        fade_end_time = sound_manager_globals.render_time;

    if ( fade_out_sound_index != -1 )
    {
        sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, fade_out_sound_index);
        if ( datum->fade_start_time == datum->fade_stop_time )
            datum->fade_interpolation_start = 0.0f;
        else
            datum->fade_interpolation_start = sound_calculate_fade(fade_out_sound_index);
        datum->fade_mode = mode;
        datum->fade_start_time = previous_tick;
        datum->fade_stop_time = fade_end_time;
        datum->fade_interpolation_end = 1.0f;
    }

    if ( fade_up_sound_index != -1 )
    {
        sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, fade_up_sound_index);
        datum->fade_interpolation_start = sound_calculate_fade(fade_up_sound_index);
        datum->fade_mode = mode;
        datum->fade_interpolation_end = 0.0f;
        datum->fade_start_time = previous_tick;
        datum->fade_stop_time = fade_end_time;
    }
}
