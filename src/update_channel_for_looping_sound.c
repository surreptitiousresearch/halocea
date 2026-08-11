/* update_channel_for_looping_sound @ 0x83717E58 — drive a looping sound's hardware channel each
 * update. Computes the faded, distance-attenuated gain and bent pitch from the sound definition and
 * the owning looping-sound track, advances the loop state machine (intro -> loop -> outro), queues
 * the next permutation when the platform channel needs feeding, and pushes the properties. On the
 * first update the channel is unassigned (playing_channel_index == -1) and is started + queued;
 * thereafter it is refreshed. Music sounds (class 32) are stopped/skipped when the music gain is
 * zero. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_permutation.h"
#include "headers/sound_class_definition.h"
#include "headers/looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/platform_sound_channel_properties.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_flags.h"
#include "headers/sound_class.h"
#include "headers/sound_type.h"
#include "headers/looping_sound_track_flags.h"
#include "headers/looping_sound_definition_flags.h"
#include "headers/sound_channel_state.h"
#include "headers/blam_data_globals.h"
#include "headers/sound_fade_mode.h"


extern float sound_manager_master_gain(int16_t class_index);
extern float sound_definition_get_minimum_distance(int sound_definition_index);
extern void sound_stop(int sound_index);
extern int looping_sound_new_sound(int looping_sound_index, int definition_index, int16_t track_index, int16_t type);
extern void sound_start_fade(int16_t mode, float seconds, int fade_out_sound_index, int fade_up_sound_index);
extern int16_t sound_definition_find_pitch_range_by_pitch(const sound_definition *sound, float pitch, int16_t current_range);
extern int16_t channel_get_state(int16_t channel_index);
extern int16_t sound_definition_next_permutation(sound_definition *sound, int16_t pitch_range_index, int16_t looping_last_permutation_index);
extern void sound_set_definition_end(int sound_index);
extern uint8_t _sound_cache_sound_request(sound_permutation *sound, uint8_t block, uint8_t load, uint8_t reference);
extern int get_object_by_looping_sound(int sound_index);
extern void channel_queue_sound(int16_t channel_index, sound_permutation *permutation, int identifier, uint8_t is_local_player, int16_t class_index, uint8_t lapping);

void update_channel_for_looping_sound(int16_t channel_index, float fade)
{
    int channel_index_s = channel_index;
    int restart_flag = 0;
    sound_datum *datum = DATUM_GET(sound_data, sound_datum,
                                            sound_channels[channel_index_s].sound_index);
    int definition_index = datum->definition_index;
    float distance_t = datum->source.scale;
    sound_definition *def = TAG_GET(sound_definition, definition_index);
    looping_sound_datum *loop_datum = DATUM_GET(looping_sound_data, looping_sound_datum,
                                                         datum->source_identifier);
    int loop_track_index = datum->loop_track_index;
    looping_sound_definition *loop_def =
        TAG_GET(looping_sound_definition, loop_datum->definition_index);
    looping_sound_track *track = (looping_sound_track *)loop_def->tracks.address + loop_track_index;

    float pitch = (((def->scale_upper_bound.pitch - def->scale_lower_bound.pitch) * distance_t)
                          + def->scale_lower_bound.pitch) * datum->pitch;

    float scale_upper_gain;
    float scale_lower_gain;
    float master_gain;
    float looping_gain;       /* gain factor for the already-assigned branch */
    int assigned_channel;
    platform_sound_channel_properties properties;

    properties.minimum_distance = sound_definition_get_minimum_distance(definition_index);
    properties.maximum_distance = 3.4028235e38f; /* FLT_MAX */
    properties.inner_cone_angle = def->inner_cone_angle;
    properties.outer_cone_angle = def->outer_cone_angle;
    properties.outer_cone_gain = def->outer_cone_gain;
    properties.reverb_damping_factor = sound_classes[def->class_index].reverb_damping_factor;

    scale_upper_gain = def->scale_upper_bound.gain;
    scale_lower_gain = def->scale_lower_bound.gain;
    master_gain = sound_manager_master_gain(def->class_index);
    /* shared distance/fade/track gain factor (also stored into properties.gain) */
    looping_gain = (((((master_gain * track->gain) * def->gain)
                                                  * datum->source.gain) * fade)
                           * (((scale_upper_gain - scale_lower_gain) * distance_t) + scale_lower_gain));
    properties.gain = looping_gain;

    assigned_channel = datum->playing_channel_index;
    if ( assigned_channel != -1 )
    {
        int16_t pitch_range_index;
        sound_pitch_range *pitch_range;
        float current_pitch;
        float bent_pitch;

        if ( def->class_index == _sound_class_music && sound_manager_globals.music_gain == 0.0 )
        {
            sound_stop(sound_channels[channel_index_s].sound_index);
            return;
        }

        pitch_range_index = (uint16_t)datum->pitch_range_index;
        pitch_range = (sound_pitch_range *)def->pitch_ranges.address + pitch_range_index;
        current_pitch = (sound_channels[assigned_channel].pitch * pitch_range->natural_pitch);

        /* clamp the target pitch to within maximum_bend of the channel's current pitch */
        if ( def->maximum_bend == 0.0 || pitch == current_pitch )
        {
            bent_pitch = pitch;
        }
        else if ( pitch <= current_pitch )
        {
            bent_pitch = ((sound_channels[assigned_channel].pitch * pitch_range->natural_pitch)
                                 / def->maximum_bend);
            if ( pitch > bent_pitch )
                bent_pitch = pitch;
        }
        else
        {
            bent_pitch = ((sound_channels[assigned_channel].pitch * pitch_range->natural_pitch)
                                 * def->maximum_bend);
            if ( pitch <= bent_pitch )
                bent_pitch = pitch;
        }
        properties.pitch = pitch_range->runtime_oo_natural_pitch * bent_pitch;

        /* if the bent pitch crossed into a different pitch range, start a crossfade to a new child sound */
        if ( (uint16_t)datum->type == _sound_loop_track
          && (datum->fade_start_time == datum->fade_stop_time || datum->fade_interpolation_end != 0.0)
          /* DEVIATION: current_range is r5 = lhz r5,0x8E(r30) @0x83718170 = pitch_range_index; the
           * `9 *` scale Hex-Rays printed here is fabricated — there is no multiply in the binary. */
          && sound_definition_find_pitch_range_by_pitch(def, bent_pitch, pitch_range_index) != datum->pitch_range_index
          && sound_channels[channel_index_s].sound_index == loop_datum->tracks[loop_track_index].primary_sound_index
          && !sound_manager_globals.idling )
        {
            int new_sound = looping_sound_new_sound(datum->source_identifier, datum->definition_index,
                                                    (uint16_t)datum->loop_track_index, _sound_loop_track);
            if ( new_sound != -1 )
            {
                /* the new crossfade child fades down from full gain (r6 slot); the unused fade-in
                 * and trailing scratch params were register-reuse artifacts at the call site */
                sound_start_fade(_sound_fade_mode_crossfade, 0.5, new_sound, -1);
                loop_datum->tracks[loop_track_index].primary_sound_index = new_sound;
            }
        }

        {
            int loop_state = datum->type;
            if ( loop_state != _sound_stop_track
              && (loop_state != _sound_start_track || (track->flags & (1u << _fade_in_at_start_bit)) == 0)
              && (channel_get_state((uint16_t)datum->playing_channel_index) != _sound_channel_full
                  || (datum->flags & (1u << _sound_waiting_for_cache_bit)) != 0
                  || datum->next_definition_index != -1) )
            {
                if ( datum->next_definition_index == -1 )
                {
                    if ( (datum->flags & (1u << _sound_waiting_for_cache_bit)) == 0 )
                    {
                        int16_t permutation = sound_definition_next_permutation(def,
                                                  (uint16_t)datum->pitch_range_index,
                                                  (uint16_t)datum->permutation_index);
                        restart_flag = 1;
                        if ( permutation == -1 )
                        {
                            if ( (loop_def->flags & (1u << _looping_sound_fake_impulse_sound_bit)) != 0 )
                            {
                                datum->type = _sound_stop_track;   /* enter outro */
                                loop_datum->ordered_permutations_finished = 1;
                            }
                            else
                            {
                                permutation = sound_definition_next_permutation(def,
                                                  (uint16_t)datum->pitch_range_index, -1);
                            }
                        }
                        if ( permutation != -1 )
                        {
                            datum->permutation_index = permutation;
                            datum->flags |= (1u << _sound_waiting_for_cache_bit);
                        }
                    }
                }
                else
                {
                    sound_set_definition_end(sound_channels[channel_index_s].sound_index);
                    def = TAG_GET(sound_definition, datum->definition_index);
                    pitch_range = (sound_pitch_range *)def->pitch_ranges.address + datum->pitch_range_index;
                }

                {
                    sound_permutation *next_permutation =
                        (sound_permutation *)pitch_range->permutations.address + datum->permutation_index;

                    if ( (uint16_t)datum->type != _sound_stop_track
                      && _sound_cache_sound_request(next_permutation, 0, 1u, 1u) )
                    {
                        int owning_object = get_object_by_looping_sound(loop_datum->loop_identifier);
                        unsigned char first_person = datum->is_local_player;
                        datum->flags &= ~(1u << _sound_waiting_for_cache_bit);
                        channel_queue_sound(channel_index, next_permutation, owning_object, first_person,
                                            def->class_index, 1u);
                        if ( datum->next_definition_index == -1 )
                        {
                            int loop_state2 = datum->type;
                            if ( loop_state2 == _sound_start_track )
                                datum->type = _sound_loop_track;        /* intro -> loop */
                            else if ( loop_state2 == _sound_stopping_track )
                                datum->type = _sound_stop_track;        /* -> outro */
                        }
                    }
                }
            }
        }

        /* recovered: *(float *)(perm + 36) -> sound_permutation.gain */
        {
            sound_permutation *gain_permutation =
                (sound_permutation *)pitch_range->permutations.address + datum->permutation_index;
            properties.gain = gain_permutation->gain * properties.gain;
        }
        channel_set_properties(channel_index, &properties, 0, def->class_index);
    }
    else if ( def->class_index != _sound_class_music || sound_manager_globals.music_gain != 0.0 )
    {
        sound_pitch_range *pitch_range =
            (sound_pitch_range *)def->pitch_ranges.address + datum->pitch_range_index;
        sound_permutation *permutation =
            (sound_permutation *)pitch_range->permutations.address + datum->permutation_index;
        int owning_object;

        properties.gain = permutation->gain * looping_gain;
        properties.pitch = pitch_range->runtime_oo_natural_pitch * pitch;
        channel_set_properties(channel_index, &properties, 0, def->class_index);
        owning_object = get_object_by_looping_sound(loop_datum->loop_identifier);
        channel_queue_sound(channel_index, permutation, owning_object, datum->is_local_player,
                            def->class_index, 0);
        datum->playing_channel_index = (uint16_t)channel_index;
    }
    else
    {
        return; /* music muted and channel not yet assigned: nothing to do */
    }

    sound_manager_globals.platform->channel_update(channel_index, restart_flag, def->class_index);
}
