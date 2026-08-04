/* sound_stop @ 0x83716BF0 — tear down a playing/queued sound instance. Releases its hardware
 * channel (returning cached permutations), and frees the sound's cache pages. For a music outro
 * (class 32, state 4) that owns a looping-sound definition, it walks every track's start/loop/
 * alt-loop/end/alt-end sound reference and evicts all of their unlocked cached permutations.
 * Finally it decrements the looping-sound's active-instance count, clears any scripted-sound
 * back-reference, and deletes the runtime sound datum.
 *
 * Deviation: the decompiler inlined the per-track permutation-eviction loop five times (once per
 * track sound reference). It is factored here into release_track_sound_cache() for readability;
 * behavior is identical. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_datum.h"
#include "headers/sound_pitch_range.h"
#include "headers/looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/sound_permutation.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_flags.h"
#include "headers/sound_type.h"
#include "headers/sound_class.h"
#include "headers/blam_data_globals.h"


extern void sound_cache_sound_finished(sound_permutation *sound);
extern uint8_t sound_cache_software_locks(const sound_permutation *sound);
extern void sound_cache_sound_delete(sound_permutation *sound);
extern void *datum_try_and_get(const data_array *data, int index);
extern void datum_delete(data_array *data, int index);

/* Evict every unlocked, resident permutation of a track's referenced sound tag. */
static void release_track_sound_cache(int sound_tag_index)
{
    sound_definition *sound_def;
    int pitch_range_count;
    int pr;

    if ( sound_tag_index == -1 )
        return;

    sound_def = TAG_GET(sound_definition, sound_tag_index);
    pitch_range_count = sound_def->pitch_ranges.count;
    for ( pr = 0; pr < pitch_range_count; ++pr )
    {
        sound_pitch_range *pitch_range = (sound_pitch_range *)sound_def->pitch_ranges.address + pr;
        int permutation_count = pitch_range->permutations.count;
        int p;
        for ( p = 0; p < permutation_count; ++p )
        {
            sound_permutation *permutation = (sound_permutation *)pitch_range->permutations.address + p;
            if ( permutation->cache_block_index != -1 && !sound_cache_software_locks(permutation) )
                sound_cache_sound_delete(permutation);
        }
    }
}

void sound_stop(int sound_index)
{
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    int assigned_channel = datum->playing_channel_index;
    sound_definition *definition = TAG_GET(sound_definition, datum->definition_index);
    int handled = 0;

    if ( assigned_channel == -1 )
    {
        if ( (datum->flags & (1u << _sound_cached_bit)) != 0 )
        {
            __int16 class_index;
            sound_cache_sound_finished(
                (sound_permutation *)((sound_pitch_range *)definition->pitch_ranges.address
                                      + datum->pitch_range_index)->permutations.address
                + datum->permutation_index);
            class_index = definition->class_index;
            /* the scripted-dialog classes (44..47) manage their own cache lifetime — skip the eviction below */
            if ( class_index != _sound_class_scripted_dialog_to_player && class_index != _sound_class_scripted_other
              && class_index != _sound_class_scripted_dialog_to_other && class_index != _sound_class_scripted_dialog_force_unspatialized )
                handled = 1;
        }
        else
        {
            handled = 1;
        }
    }
    else
    {
        sound_channel_datum *channel;
        int channel_index;

        sound_channels[assigned_channel].sound_index = -1;
        channel_index = (unsigned __int16)datum->playing_channel_index;
        channel = &sound_channels[(__int16)channel_index];
        if ( channel )
        {
            if ( channel->queued_permutation )
            {
                sound_cache_sound_finished(channel->queued_permutation);
                channel->queued_permutation = 0;
            }
            if ( channel->playing_permutation )
            {
                sound_cache_sound_finished(channel->playing_permutation);
                channel->playing_permutation = 0;
            }
        }
        sound_manager_globals.platform->stop_channel(channel_index);
        datum->playing_channel_index = -1;

        {
            int class_index = definition->class_index;
            if ( definition->runtime_scripting_sound_index == sound_index
              || (class_index != _sound_class_scripted_dialog_to_player && class_index != _sound_class_scripted_other
              && class_index != _sound_class_scripted_dialog_to_other && class_index != _sound_class_scripted_dialog_force_unspatialized) )
            {
                /* music outro: drop the entire backing looping-sound definition from the cache */
                if ( (unsigned __int16)datum->type == _sound_stop_track && definition->class_index == _sound_class_music )
                {
                    int looping_def_index =
                        DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum,
                                           datum->source_identifier)->definition_index;
                    looping_sound_definition *looping_def =
                        TAG_GET(looping_sound_definition, looping_def_index);
                    int track_count = looping_def->tracks.count;
                    int t;
                    for ( t = 0; t < track_count; ++t )
                    {
                        looping_sound_track *track = (looping_sound_track *)looping_def->tracks.address + t;
                        release_track_sound_cache(track->start_sound.index);
                        release_track_sound_cache(track->stop_sound.index);
                        release_track_sound_cache(track->alternate_stop_sound.index);
                        release_track_sound_cache(track->loop_sound.index);
                        release_track_sound_cache(track->alternate_loop_sound.index);
                    }
                }
                handled = 1;
            }
        }
    }

    if ( !handled )
    {
        sound_permutation *permutation =
            (sound_permutation *)((sound_pitch_range *)definition->pitch_ranges.address
                                  + datum->pitch_range_index)->permutations.address
            + datum->permutation_index;
        if ( !sound_cache_software_locks(permutation) )
            sound_cache_sound_delete(permutation);
    }

    if ( (unsigned __int16)datum->type )
    {
        looping_sound_datum *looping = datum_try_and_get(looping_sound_data, datum->source_identifier);
        if ( looping )
        {
            --looping->component_sound_count;
            if ( looping->tracks[datum->loop_track_index].primary_sound_index == sound_index )
                looping->tracks[datum->loop_track_index].primary_sound_index = -1;
        }
    }

    if ( definition->runtime_scripting_sound_index == sound_index )
    {
        __int16 class_index = definition->class_index;
        definition->runtime_scripting_sound_index = -1;
        if ( class_index == _sound_class_scripted_dialog_to_player || class_index == _sound_class_scripted_other
          || class_index == _sound_class_scripted_dialog_to_other || class_index == _sound_class_scripted_dialog_force_unspatialized )
        {
            sound_permutation *permutation =
                (sound_permutation *)((sound_pitch_range *)definition->pitch_ranges.address
                                      + datum->pitch_range_index)->permutations.address
                + datum->permutation_index;
            if ( !sound_cache_software_locks(permutation) )
                sound_cache_sound_delete(permutation);
        }
    }

    datum_delete(sound_data, sound_index);
}
