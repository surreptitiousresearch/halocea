/* sound_channel_summary_build @ 0x83715928 — census the active channels that conflict with a given
 * sound, to enforce per-definition and per-object instance limits. Records the class's configured
 * maxima, then walks every active channel: a channel counts as "like-definition" if it plays a
 * compatible format from the same definition, and additionally "like-source" if it shares the same
 * (non-null) source object. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_channel_summary.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern uint8_t sound_valid_for_channel(int16_t compression, int16_t encoding, int16_t sample_rate, int16_t spatialization_mode, int16_t channel_type_flags);

void sound_channel_summary_build(sound_channel_summary *summary, uint16_t sound_index)
{
    sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    sound_definition *def = TAG_GET(sound_definition, datum->definition_index);
    int channel;

    summary->like_definition_count = 0;
    summary->like_source_count = 0;
    summary->maximum_instance_count = sound_classes[def->class_index].maximum_number_per_definition;
    summary->maximum_source_instance_count = sound_classes[def->class_index].maximum_number_per_object;

    for ( channel = 0; channel < sound_manager_globals.channel_count; ++channel )
    {
        sound_channel_datum *channel_datum = &sound_channels[channel];
        if ( channel_datum->sound_index != -1 && channel_datum->sound_index != sound_index )
        {
            sound_datum *other = DATA_ARRAY_ELEMENT(sound_data, sound_datum, channel_datum->sound_index);
            if ( sound_valid_for_channel(def->compression, def->encoding, def->sample_rate,
                                         datum->source.spatialization_mode,
                                         channel_datum->type_flags) )
            {
                if ( datum->definition_index == other->definition_index )
                {
                    summary->like_definition_channels[summary->like_definition_count++] = (int16_t)channel;
                    {
                        int source = datum->source_identifier;
                        if ( source != -1 && source == other->source_identifier )
                            summary->like_source_channels[summary->like_source_count++] = (int16_t)channel;
                    }
                }
            }
        }
    }
}
