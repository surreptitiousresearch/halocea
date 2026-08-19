/* sound_set_definition_end @ 0x83717D08 — transition a playing looping sound to its "end"/outro
 * definition. Swaps the active definition to the queued end definition (datum +152 idx38), marks a
 * fresh permutation needed (flag bit3), and reselects pitch range + first permutation for the new
 * definition at the current pitch. If the instance is on a channel and now exceeds the definition's
 * or source's instance limit, the least-important like channel (or this sound itself) is stopped. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_channel_summary.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t sound_definition_find_pitch_range_by_pitch(const sound_definition *sound, float pitch, int16_t current_range);
extern int16_t sound_definition_next_permutation(sound_definition *sound, int16_t pitch_range_index, int16_t looping_last_permutation_index);
extern void sound_channel_summary_build(sound_channel_summary *summary, int sound_index);
extern int16_t sound_find_like_channel(int sound_index, const int16_t *channel_indices, int16_t channel_count);
extern void sound_stop(int sound_index);

void sound_set_definition_end(int sound_index)
{
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    int end_definition_index = datum->next_definition_index;
    float pitch_modifier = datum->pitch;
    int16_t flags = datum->flags;
    sound_definition *definition = TAG_GET(sound_definition, end_definition_index);
    int16_t pitch_range;
    int16_t permutation;
    int assigned_channel;
    sound_channel_summary summary;

    datum->definition_index = end_definition_index;
    datum->next_definition_index = -1;
    datum->flags = flags | (1u << _sound_waiting_for_cache_bit);

    /* DEVIATION: current_range is r5 = lhz r5,0x8E(r31) @0x83717D4C = pitch_range_index, not the
     * flags word at +0x04 that Hex-Rays attributed to this slot. */
    pitch_range = sound_definition_find_pitch_range_by_pitch(definition, pitch_modifier,
                                                             datum->pitch_range_index);
    datum->pitch_range_index = pitch_range;
    permutation = sound_definition_next_permutation(definition, pitch_range, -1);
    assigned_channel = (uint16_t)datum->playing_channel_index;
    datum->permutation_index = permutation;

    if ( assigned_channel == 0xFFFF )
        return;

    sound_channel_summary_build(&summary, sound_index);
    if ( summary.like_source_count < summary.maximum_source_instance_count )
    {
        int16_t like_channel;
        if ( summary.like_definition_count < summary.maximum_instance_count )
            return;
        like_channel = sound_find_like_channel(sound_index, summary.like_definition_channels,
                                               summary.like_definition_count);
        if ( like_channel != -1 )
        {
            sound_stop(sound_channels[like_channel].sound_index);
            return;
        }
    }
    else
    {
        int16_t like_channel = sound_find_like_channel(sound_index, summary.like_source_channels,
                                                       summary.like_source_count);
        if ( like_channel != -1 )
        {
            sound_stop(sound_channels[like_channel].sound_index);
            return;
        }
    }

    sound_stop(sound_index);
}
