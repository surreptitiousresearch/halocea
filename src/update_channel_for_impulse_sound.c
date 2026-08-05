/* update_channel_for_impulse_sound @ 0x83717860 — push the current parameters of a playing impulse
 * (one-shot) sound into its hardware channel. On the sound's first update the channel is unassigned
 * (datum +140 == 0xFFFF): the full property block (cones, distances, reverb, pitch) is built, the
 * channel is started via channel_set_properties, the chosen permutation is queued, and the channel
 * index is recorded. On subsequent updates only the gain is recomputed and the channel is refreshed. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_permutation.h"
#include "headers/sound_class_definition.h"
#include "headers/platform_sound_channel_properties.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern float sound_manager_master_gain(int16_t class_index);
extern float sound_definition_get_minimum_distance(int sound_definition_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void channel_queue_sound(int16_t channel_index, sound_permutation *permutation, int identifier, uint8_t is_local_player, int16_t class_index, uint8_t lapping);

void update_channel_for_impulse_sound(int16_t channel_index, float fade)
{
    sound_channel_datum *channel = &sound_channels[channel_index];
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, channel->sound_index);
    int definition_index = datum->definition_index;
    sound_definition *def = TAG_GET(sound_definition, definition_index);

    float distance_t = datum->source.scale;
    float scale_upper_gain = def->scale_upper_bound.gain;
    float scale_lower_gain = def->scale_lower_bound.gain;
    float master_gain = sound_manager_master_gain((uint16_t)def->class_index);
    /* distance-attenuated, scaled, faded gain factor shared by both branches */
    float distance_gain = (scale_upper_gain - scale_lower_gain) * distance_t + scale_lower_gain;
    float fade_factor = (master_gain * datum->source.gain) * fade * distance_gain;

    int16_t owning_object_type = -1;
    platform_sound_channel_properties properties;

    if ( (uint16_t)datum->playing_channel_index == 0xFFFF )
    {
        float pitch_modifier = datum->pitch;
        sound_pitch_range *pitch_range =
            (sound_pitch_range *)def->pitch_ranges.address + datum->pitch_range_index;
        const sound_permutation *permutation =
            (const sound_permutation *)pitch_range->permutations.address + datum->permutation_index;

        properties.gain = (permutation->gain * def->gain) * fade_factor;
        properties.pitch = pitch_modifier * pitch_range->runtime_oo_natural_pitch;
        properties.minimum_distance = sound_definition_get_minimum_distance(definition_index);
        properties.inner_cone_angle = def->inner_cone_angle;
        properties.outer_cone_angle = def->outer_cone_angle;
        properties.outer_cone_gain = def->outer_cone_gain;
        properties.reverb_damping_factor =
            sound_classes[(uint16_t)def->class_index].reverb_damping_factor;
        properties.maximum_distance = 3.4028235e38f; /* FLT_MAX */

        channel_set_properties(channel_index, &properties, 0, (uint16_t)def->class_index);

        {
            int source_object = datum->source_identifier;
            if ( source_object != -1 )
            {
                int *object = object_try_and_get_and_verify_type(source_object, object_mask_all);
                if ( object )
                    owning_object_type = *object;
            }
        }

        /* channel_queue_sound takes the permutation into non-const channel storage; this
           function only reads it, hence the const-boundary cast (matches the binary) */
        channel_queue_sound(channel_index, (sound_permutation *)permutation, owning_object_type,
                            datum->is_local_player, (uint16_t)def->class_index, 0);
        datum->playing_channel_index = (uint16_t)channel_index;
    }
    else
    {
        sound_permutation *playing_permutation = channel->playing_permutation;
        if ( playing_permutation )
            properties.gain = (playing_permutation->gain * def->gain) * fade_factor;
        else
            properties.gain = def->gain * fade_factor;

        channel_set_properties(channel_index, &properties, 1u, (uint16_t)def->class_index);
        sound_manager_globals.platform->channel_update(channel_index, 0, (uint16_t)def->class_index);
    }
}
