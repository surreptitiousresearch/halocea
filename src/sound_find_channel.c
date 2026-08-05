/* sound_find_channel @ 0x83719AA8 — choose a hardware channel for a sound. If it already owns one,
 * return it. A speech sound with a source first tries to reuse a channel already playing speech from
 * the same source (so a unit's dialogue replaces its own previous line, copying its type flags). When
 * no such channel exists — or the sound is not speech — it enforces the class's per-source and
 * per-definition instance limits (preempting a like channel) and otherwise picks the best free/lowest
 * priority channel. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_channel_summary.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern void sound_channel_summary_build(sound_channel_summary *summary, uint16_t sound_index);
extern int16_t sound_find_like_channel(uint16_t sound_index, const int16_t *channel_indices, int16_t channel_count);
extern int16_t sound_find_best_channel(uint16_t sound_index);

int16_t sound_find_channel(uint16_t sound_index)
{
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    int assigned = (uint16_t)datum->playing_channel_index;
    int source_identifier;

    if ( assigned != 0xFFFF )
        return assigned;

    /* speech with a source: reuse the channel already voicing that source */
    if ( sound_classes[TAG_GET(sound_definition, datum->definition_index)->class_index].speech
      && (source_identifier = datum->source_identifier) != -1 )
    {
        int channel;
        if ( sound_manager_globals.channel_count <= 0 )
            return sound_find_best_channel(sound_index);

        for ( channel = 0; ; channel = (int16_t)(channel + 1) )
        {
            if ( sound_channels[channel].sound_index != -1 )
            {
                sound_datum *other = DATA_ARRAY_ELEMENT(sound_data, sound_datum,
                                                       sound_channels[channel].sound_index);
                if ( other->source_identifier == source_identifier
                  && sound_classes[TAG_GET(sound_definition, other->definition_index)->class_index].speech )
                {
                    datum->source.spatialization_mode = other->source.spatialization_mode; /* inherit spatialization */
                    return channel;
                }
            }
            if ( (int16_t)(channel + 1) >= sound_manager_globals.channel_count )
                return sound_find_best_channel(sound_index);
        }
    }

    /* otherwise enforce the instance limits, else pick the best channel */
    {
        sound_channel_summary summary;
        sound_channel_summary_build(&summary, sound_index);
        if ( summary.like_source_count >= summary.maximum_source_instance_count )
            return sound_find_like_channel(sound_index, summary.like_source_channels, summary.like_source_count);
        if ( summary.like_definition_count >= summary.maximum_instance_count )
            return sound_find_like_channel(sound_index, summary.like_definition_channels, summary.like_definition_count);
        return sound_find_best_channel(sound_index);
    }
}
