/* sound_find_best_channel @ 0x83717670 — pick the channel a sound should play on. Scans channels
 * whose hardware format matches the sound (sound_valid_for_channel): the first free matching channel
 * wins outright; otherwise the matching channel playing the most-preemptable sound (per
 * sound_preempts_sound, by distance) is chosen, or -1 if none can be preempted.
 *
 * Deviation: the decompiler inlined sound_valid_for_channel's bit test and lost the definition pointer
 * assignment; restored to a call and typed TAG_INSTANCE deref. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_source.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern float source_distance_squared(int16_t listener_index, const sound_source *source);
extern uint8_t sound_valid_for_channel(int16_t compression, int16_t encoding, int16_t sample_rate, int16_t spatialization_mode, int16_t channel_type_flags);
extern uint8_t sound_preempts_sound(uint16_t challenger_sound_index, uint16_t champion_sound_index, float challenger_distance_squared);

int16_t sound_find_best_channel(uint16_t sound_index)
{
    char *data = (char *)sound_data->data;
    sound_datum *datum = (sound_datum *)&data[176 * sound_index];
    sound_definition *def = TAG_GET(sound_definition, datum->definition_index);
    const sound_source *source = &datum->source;
    int best_channel = -1;
    int best_sound = -1;
    float best_distance = 0.0f;
    float distance = source_distance_squared(datum->listener_index, source);
    __int16 compression = def->compression;
    __int16 encoding = def->encoding;
    __int16 sample_rate = def->sample_rate;
    __int16 spatialization = source->spatialization_mode;
    int channel;

    if ( sound_manager_globals.channel_count <= 0 )
        return -1;

    for ( channel = 0; ; channel = (__int16)(channel + 1) )
    {
        sound_channel_datum *ch = &sound_channels[channel];
        if ( sound_valid_for_channel(compression, encoding, sample_rate, spatialization, ch->type_flags) )
        {
            int other_sound = ch->sound_index;
            if ( other_sound == -1 )
                return channel;                       /* a free matching channel */

            if ( sound_preempts_sound(sound_index, other_sound, distance)
              && ((__int16)best_channel == -1 || sound_preempts_sound(best_sound, other_sound, best_distance)) )
            {
                best_channel = channel;
                best_sound = other_sound;
                sound_datum *other = (sound_datum *)&data[176 * (unsigned __int16)other_sound];
                best_distance = source_distance_squared(other->listener_index, &other->source);
            }
        }
        if ( (__int16)(channel + 1) >= sound_manager_globals.channel_count )
            return best_channel;
    }
}
