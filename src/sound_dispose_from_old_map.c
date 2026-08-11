/* sound_dispose_from_old_map @ 0x8371AD70 — fade out and stop all sounds over ~300ms,
 * unpause the mixer, then stop everything and clear the looping-sound pool. */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_datum.h"
#include "headers/sound_manager_globals.h"
#include "headers/sound_fade_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern uint32_t system_milliseconds(void);
extern float sound_calculate_fade(int sound_index);
extern void sound_idle(void);
extern void sound_stop_all(void);
extern void data_delete_all(data_array *data);
extern int data_next_index(const data_array *data, int16_t index);
extern uint32_t main_get_mseconds(void);

void sound_dispose_from_old_map(void)
{
    unsigned char paused = sound_manager_globals.paused;

    if ( !paused
      && sound_manager_globals.initialized
      && sound_manager_globals.active
      && !sound_manager_globals.disable_from_commandline )
    {
        unsigned int start_ms = system_milliseconds();
        unsigned char any = 0;
        int index;

        for ( index = data_next_index(sound_data, -1); index != -1; index = data_next_index(sound_data, index) )
        {
            int render_time = sound_manager_globals.render_time - 1;
            int fade_end = (int)((float)render_time + 300.0f);
            sound_datum *snd = DATA_ARRAY_ELEMENT(sound_data, sound_datum, index);
            any = 1;
            if ( fade_end <= sound_manager_globals.render_time )
                fade_end = sound_manager_globals.render_time;
            snd->fade_interpolation_start = sound_calculate_fade(index);
            snd->fade_start_time = render_time;
            snd->fade_interpolation_end = 0.0f;
            snd->fade_mode = _sound_fade_mode_linear;
            snd->fade_stop_time = fade_end;
        }

        if ( any )
        {
            float deadline = ((float)start_ms + 300.0f);
            while ( (float)system_milliseconds() < deadline )
                sound_idle();
        }
        paused = sound_manager_globals.paused;
    }

    if ( paused )
    {
        sound_manager_globals.paused = 0;
        if ( sound_manager_globals.platform )
            sound_manager_globals.platform->set_pause(0);
        sound_manager_globals.render_time = main_get_mseconds();
    }

    sound_stop_all();
    if ( looping_sound_data && looping_sound_data->valid )
        data_delete_all(looping_sound_data);
}
