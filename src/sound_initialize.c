/* sound_initialize @ 0x83716838
   One-time sound system bring-up. Reads the (default) sound preferences, creates
   the PC sound cache, and — unless disabled from the command line — selects the
   platform sound backend, allocates the sound and looping-sound data arrays,
   initializes the backend, and lays out the hardware channels per the platform's
   virtual channel counts.

   Attested void: the decompiler reused r3 (`result`) as scratch across allocations,
   but the tail is `b __restgprlr_27` with no meaningful r3 and the sole caller
   shell_initialize ignores it (0/1 consume). */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_preferences.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void read_sound_preferences(sound_preferences **preferences);
extern void sound_cache_new(void);
extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern void data_make_valid(data_array *data);


void sound_initialize(void)
{
    sound_manager_globals.initialized = 0;
    sound_manager_globals.active = 1;

    sound_preferences *prefs;
    read_sound_preferences(&prefs);
    sound_cache_new();

    if ( sound_manager_globals.disable_from_commandline )
        return;

    sound_manager_globals.sound_variety = 0;
    sound_manager_globals.sound_environment_tag = -1;
    sound_manager_globals.nondialog_gain = 1.0f;
    sound_manager_globals.music_gain = 1.0f;
    sound_manager_globals.master_gain = 1.0f;
    channel_set_properties = channel_set_properties_hardware;
    sound_manager_globals.effects_gain = 1.0f;

    short platform_code = prefs->platform_code;
    if ( platform_code < 0 || platform_code >= 2 )
        return;

    platform_sound_manager_definition *platform = platform_definitions[platform_code];
    if ( !platform || platform->platform_code != platform_code )
        return;

    sound_manager_globals.platform = platform_definitions[platform_code];

    sound_data = data_new("sounds", 512, 176);
    if ( !sound_data )
        return;

    looping_sound_data = data_new("looping sounds", 128, 228);
    if ( !looping_sound_data )
        return;

    if ( !sound_manager_globals.platform->initialize(prefs) )
        return;

    data_make_valid(sound_data);
    data_make_valid(looping_sound_data);

    short next_channel = 0;
    short channel_count = sound_manager_globals.channel_count;
    for ( int i = 0; i < 4; i = (short)(i + 1) )
    {
        short type_count = prefs->virtual_channel_counts[i];
        channel_count = (short)(type_count + channel_count);
        sound_manager_globals.channel_count = channel_count;
        for ( int n = 0; n < type_count; n = (short)(n + 1) )
        {
            sound_channel_datum *channel = &sound_channels[next_channel++];
            channel->playing_permutation = 0;
            channel->queued_permutation = 0;
            channel->sound_index = -1;
            channel->type_flags = sound_channel_type_flags[i];
        }
    }
    sound_manager_globals.initialized = 1;
    return;
}
