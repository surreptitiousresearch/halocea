/* sound_find_like_channel @ 0x83715AC8 — among a set of channels playing sounds "like" a candidate,
 * pick one that can be preempted for it: the first whose playing sound is at least as far from the
 * listener as the candidate (distance difference < 1.0) and has been playing at least its class's
 * preemption time. Returns that channel index, or -1 if none qualifies. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_source.h"
#include "headers/sound_class_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"

extern float source_distance_squared(int16_t listener_index, const sound_source *source);

int16_t sound_find_like_channel(int sound_index, const int16_t *channel_indices, int16_t channel_count)
{
    sound_datum *candidate = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    char *sound_base = (char *)sound_data->data;
    sound_definition *definition = TAG_GET(sound_definition, candidate->definition_index);
    float candidate_distance = source_distance_squared(candidate->listener_index, &candidate->source);
    int preemption_time;
    int i;

    if ( channel_count <= 0 )
        return -1;

    preemption_time = sound_classes[definition->class_index].preemption_time;

    for ( i = 0; i < channel_count; ++i )
    {
        int channel = (uint16_t)channel_indices[i];
        sound_datum *other = (sound_datum *)(176 * (uint16_t)sound_channels[(int16_t)channel].sound_index + sound_base);
        float distance = 0.0f;

        if ( sound_manager_globals.render_time - other->start_time >= preemption_time )
        {
            const sound_source *source = &other->source;
            unsigned int mode = (uint16_t)source->spatialization_mode;
            if ( mode )
            {
                if ( mode == _sound_spatialization_mode_absolute )
                {
                    float *listener = sound_manager_globals.listeners[other->listener_index].matrix.___u1.n[3];
                    float dx = listener[0] - source->location.position.n[0];
                    float dy = listener[1] - source->location.position.n[1];
                    float dz = listener[2] - source->location.position.n[2];
                    distance = dy * dy + (dx * dx + dz * dz);
                }
                else if ( mode < NUMBER_OF_SOUND_SPATIALIZATION_MODES )
                {
                    distance = source->location.position.n[2] * source->location.position.n[2]
                             + (source->location.position.n[0] * source->location.position.n[0]
                                + source->location.position.n[1] * source->location.position.n[1]);
                }
            }

            if ( (candidate_distance - distance) < 1.0f )
                return channel;
        }
    }
    return -1;
}
