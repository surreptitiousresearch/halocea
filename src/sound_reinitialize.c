/* sound_reinitialize @0x83718418 — reinitialize the sound engine with new platform preferences: stops
 * every active sound, tears down and reinitializes the platform sound backend, and (on success) rebuilds
 * the channel table — for each of the 4 channel type groups, adds preferences->virtual_channel_counts[i]
 * fresh channels (sound-less, tagged with that group's type flags) to the running channel_count/
 * sound_channels[] array. Returns whether platform reinitialization succeeded. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_manager_globals.h"
#include "headers/sound_preferences.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void sound_stop(int sound_index);

unsigned __int8 sound_reinitialize(sound_preferences *preferences)
{
    for ( int i = data_next_index(sound_data, -1); i != -1; i = data_next_index(sound_data, i) )
        sound_stop(i);

    sound_manager_globals.platform->dispose();
    sound_manager_globals.initialized = 0;
    sound_manager_globals.active = 1;
    sound_manager_globals.channel_count = 0;

    unsigned __int8 result = sound_manager_globals.platform->initialize(preferences);

    if ( result )
    {
        __int16 channel_count = sound_manager_globals.channel_count;
        __int16 channel_index = 0;

        for ( int group = 0; group < 4; ++group )
        {
            channel_count += preferences->virtual_channel_counts[group];
            sound_manager_globals.channel_count = channel_count;

            for ( __int16 added = 0; added < preferences->virtual_channel_counts[group]; ++added )
            {
                sound_channel_datum *channel = &sound_channels[channel_index++];
                channel->sound_index = -1;
                channel->type_flags = sound_channel_type_flags[group];
                channel->playing_permutation = nullptr;
                channel->queued_permutation = nullptr;
            }
        }

        sound_manager_globals.initialized = 1;
    }

    return result;
}
