/* sound_refresh_looping @0x83718C50 — keep one looping sound's per-track voices in sync with its source
 * and refresh state each mix frame. Finds (or creates) the looping-sound datum, copies in the latest
 * spatialized source, and for each track either starts the start/loop voice, swaps alternate voices, or
 * begins the stop voice with a fade. When fully stopped, or when the source is inaudible, every
 * permutation's streaming cache block is released and the datum freed. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_manager_globals.h"
#include "headers/sound_source.h"
#include "headers/sound_permutation.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_definition.h"
#include "headers/sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/looping_sound_datum.h"
#include "headers/real_point3d.h"
#include "headers/looping_sound_definition_flags.h"
#include "headers/looping_sound_track_flags.h"
#include "headers/looping_sound_refresh_state.h"
#include "headers/sound_type.h"
#include "headers/sound_class.h"
#include "headers/blam_data_globals.h"


extern void render_debug_looping_sound(int definition_index, const sound_source *source);
extern int looping_sound_find(int identifier);
extern uint16_t looping_sound_new(int definition_index, int identifier, const sound_source *source);
extern int looping_sound_new_sound(int looping_sound_index, int definition_index, int16_t track_index, int16_t type);
extern uint8_t sound_cache_software_locks(const sound_permutation *sound);
extern void sound_cache_sound_delete(sound_permutation *sound);
extern void player_effect_continuous_refresh(int effect_index, const real_point3d *origin);
extern void sound_start_fade(int16_t mode, float seconds, int fade_out_sound_index, int fade_up_sound_index);
extern int16_t source_audible(sound_source *source, float maximum_distance);
extern void datum_delete(data_array *data, int index);

/* The five cache-bearing sound references within a track, in the binary's release order
 * (offsets 60/92/156/76/140): start, stop, alternate stop, loop, alternate loop. */
#define TRACK_CACHE_LAYER_SOUNDS(track) \
    { (track)->start_sound.index, (track)->stop_sound.index, (track)->alternate_stop_sound.index, \
      (track)->loop_sound.index, (track)->alternate_loop_sound.index }

/* Release every streaming cache block held by the permutations of a sound tag. */
static void looping_release_sound_cache(int sound_tag)
{
    sound_definition *definition = TAG_GET(sound_definition, sound_tag);
    int pitch_range_count = definition->pitch_ranges.count;
    int pitch_range;

    for ( pitch_range = 0; pitch_range < pitch_range_count; ++pitch_range )
    {
        sound_pitch_range *range = (sound_pitch_range *)definition->pitch_ranges.address + pitch_range;
        int permutation_count = range->permutations.count;
        int permutation;
        for ( permutation = 0; permutation < permutation_count; ++permutation )
        {
            sound_permutation *sound = (sound_permutation *)range->permutations.address + permutation;
            if ( sound->cache_block_index != -1 && !sound_cache_software_locks(sound) )
                sound_cache_sound_delete(sound);
        }
    }
}

/* As above, but only for music-class (class 32) sounds; returns 0 to signal the caller to stop. */
static int looping_release_sound_cache_if_simple(int sound_tag)
{
    sound_definition *definition = TAG_GET(sound_definition, sound_tag);
    int pitch_range_count;
    int pitch_range;

    if ( definition->class_index != _sound_class_music )
        return 0;

    pitch_range_count = definition->pitch_ranges.count;
    for ( pitch_range = 0; pitch_range < pitch_range_count; ++pitch_range )
    {
        sound_pitch_range *range = (sound_pitch_range *)definition->pitch_ranges.address + pitch_range;
        int permutation_count = range->permutations.count;
        int permutation;
        for ( permutation = 0; permutation < permutation_count; ++permutation )
        {
            sound_permutation *sound = (sound_permutation *)range->permutations.address + permutation;
            if ( sound->cache_block_index != -1 && !sound_cache_software_locks(sound) )
                sound_cache_sound_delete(sound);
        }
    }
    return 1;
}

/* was: int ret / char alternate / double force_stop_time — DB prototype:
 * unsigned __int8 ret, unsigned __int8 alternate, float force_stop_time */
uint8_t sound_refresh_looping(int definition_index, int identifier, sound_source *source,
                          int16_t refresh_state, uint8_t alternate, float force_stop_time)
{
    int stopped_result = (refresh_state == _looping_sound_refresh_stop); /* BOOL undefined in C TU */
    unsigned __int8 newly_created = 0;
    int index;
    looping_sound_datum *datum;
    looping_sound_definition *definition;
    int track;
    int *voice;          /* current track voice slot in the datum */
    looping_sound_track *track_record;
    int sound_index;
    int new_voice;
    float fade;

    render_debug_looping_sound(definition_index, source);

    if ( !sound_manager_globals.initialized
      || !sound_manager_globals.active
      || sound_manager_globals.disable_from_commandline )
        return stopped_result;

    index = looping_sound_find(identifier);
    if ( index == -1 )
    {
        if ( refresh_state == _looping_sound_refresh_stop )
            return 1;   /* nothing playing, asked to stop: already stopped */
        index = looping_sound_new(definition_index, identifier, source);
        newly_created = 1;
        if ( index == -1 )
            return 0;   /* newly_created==1, so the original `return v16==0` yields 0 here */
    }

    datum = DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, index);
    definition = TAG_GET(looping_sound_definition, definition_index);
    memcpy(&datum->source, source, sizeof(datum->source));
    datum->flip_flop = sound_manager_globals.flip_flop;

    /* full-stop path: release all cache blocks and free the datum */
    if ( (refresh_state == _looping_sound_refresh_stop || datum->ordered_permutations_finished) && !datum->component_sound_count )
    {
        if ( (definition->flags & (1u << _looping_sound_fake_impulse_sound_bit)) != 0 && definition->tracks.count > 0 )
        {
            for ( track = 0; track < definition->tracks.count; track = (__int16)(track + 1) )
            {
                looping_sound_track *record = (looping_sound_track *)definition->tracks.address + track;
                int layer_sounds[5] = TRACK_CACHE_LAYER_SOUNDS(record);
                int layer;
                for ( layer = 0; layer < 5; ++layer )
                {
                    int sound_tag = layer_sounds[layer];
                    if ( sound_tag != -1 )
                        looping_release_sound_cache(sound_tag);
                }
            }
        }
        datum_delete(looping_sound_data, index);
        return 1;
    }

    {
        int continuous_effect = definition->continuous_damage_effect.index;
        if ( continuous_effect != -1 )
            player_effect_continuous_refresh(continuous_effect, &source->location.position);
    }

    track = 0;
    if ( definition->tracks.count <= 0 )
        goto check_audibility;

    sound_index = 0;
track_loop:
    track_record = (looping_sound_track *)definition->tracks.address + track;
    voice = &datum->tracks[track].primary_sound_index;
    if ( newly_created )
        *voice = -1;

    if ( refresh_state )
    {
        if ( refresh_state == _looping_sound_refresh_stop )
            goto begin_stop;
    }
    else
    {
        sound_index = track_record->start_sound.index;
        if ( sound_index != -1 )
            *voice = looping_sound_new_sound(index, sound_index, track, _sound_start_track);
    }

    if ( datum->ordered_permutations_finished )   /* track is stopping */
    {
begin_stop:
        if ( datum->state != _looping_sound_refresh_stop )
        {
            if ( force_stop_time == 0.0 )
            {
                if ( *voice != -1
                  && ((track_record->flags & (1u << _fade_out_at_stop_bit)) != 0
                   || (track_record->stop_sound.index == -1 && (definition->flags & (1u << _looping_sound_fake_impulse_sound_bit)) == 0)) )
                    sound_start_fade(0, track_record->fade_out_duration, sound_index, -1);

                if ( track_record->stop_sound.index != -1 )
                {
                    sound_index = track_record->stop_sound.index;
                    if ( alternate && track_record->alternate_stop_sound.index != -1 )
                        sound_index = track_record->alternate_stop_sound.index;
                    if ( (track_record->flags & (1u << _fade_out_at_stop_bit)) != 0 )
                    {
                        looping_sound_new_sound(index, sound_index, track, _sound_stop_track);
                    }
                    else if ( *voice != -1 )
                    {
                        sound_datum *playing = DATA_ARRAY_ELEMENT(sound_data, sound_datum, *voice);
                        if ( playing->playing_channel_index != (__int16)0xFFFF )
                        {
                            if ( playing->definition_index != sound_index )
                                playing->next_definition_index = sound_index;
                            playing->type = _sound_stopping_track;
                        }
                    }
                }
            }
            else
            {
                sound_start_fade(0, force_stop_time, sound_index, -1);
            }
        }
        goto next_track;
    }

    /* track is playing: (re)start its loop voice, swapping alternate variants */
    sound_index = track_record->loop_sound.index;
    if ( alternate && track_record->alternate_loop_sound.index != -1 )
        sound_index = track_record->alternate_loop_sound.index;
    if ( sound_index != -1 )
    {
        if ( *voice != -1 && (refresh_state || (track_record->flags & (1u << _fade_in_at_start_bit)) == 0) )
        {
            if ( alternate == datum->alternate || (track_record->flags & (1u << _fade_in_alternate_bit)) == 0 )
            {
                if ( !newly_created )
                {
                    sound_datum *playing = DATA_ARRAY_ELEMENT(sound_data, sound_datum, *voice);
                    if ( playing->definition_index != sound_index )
                        playing->next_definition_index = sound_index;
                }
            }
            else
            {
                new_voice = looping_sound_new_sound(index, sound_index, track, _sound_loop_track);
                if ( new_voice != -1 )
                {
                    sound_start_fade(0, track_record->fade_out_duration, sound_index, new_voice);
                    *voice = new_voice;
                }
            }
            goto next_track;
        }
        new_voice = looping_sound_new_sound(index, sound_index, track, _sound_loop_track);
        if ( new_voice != -1 )
            goto started_new_voice;
    }

next_track:
    track = (__int16)(track + 1);
    if ( track >= definition->tracks.count )
        goto check_audibility;
    goto track_loop;

started_new_voice:
    if ( refresh_state )
    {
        fade = 2.0f;
    }
    else
    {
        if ( (track_record->flags & (1u << _fade_in_at_start_bit)) == 0 )
        {
            *voice = new_voice;
            goto next_track;
        }
        fade = track_record->fade_in_duration;
    }
    sound_start_fade(0, fade, sound_index, new_voice);
    *voice = new_voice;
    goto next_track;

check_audibility:
    if ( !datum->component_sound_count
      && source_audible(source, definition->runtime_maximum_distance) == -1 )
    {
        if ( definition->tracks.count > 0 )
        {
            for ( track = 0; track < definition->tracks.count; track = (__int16)(track + 1) )
            {
                looping_sound_track *record = (looping_sound_track *)definition->tracks.address + track;
                int layer_sounds[5] = TRACK_CACHE_LAYER_SOUNDS(record);
                int layer;
                int aborted = 0;
                for ( layer = 0; layer < 5; ++layer )
                {
                    int sound_tag = layer_sounds[layer];
                    if ( sound_tag != -1 && !looping_release_sound_cache_if_simple(sound_tag) )
                    {
                        aborted = 1;   /* non-simple sound: stop walking tracks */
                        break;
                    }
                }
                if ( aborted )
                    break;
            }
        }
        datum_delete(looping_sound_data, index);
    }

    datum->alternate = alternate;
    datum->state = refresh_state;
    return 0;
}
