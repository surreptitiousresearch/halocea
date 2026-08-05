/* sound_preempts_sound @ 0x83715C58 — does a challenger sound outrank the sound currently holding a
 * channel? It wins if its class priority is higher, or — at equal priority — if it is closer to the
 * listener than the champion. Otherwise the champion keeps the channel.
 *
 * Sound datum (176 bytes): +6 (word) audibility token, +8 definition index, +20 sound_source.
 * Definition word[2] (+4) = class index. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_source.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern float source_distance_squared(int16_t listener_index, const sound_source *source);

uint8_t sound_preempts_sound(uint16_t challenger_sound_index, uint16_t champion_sound_index,
                          float challenger_distance_squared)
{
    sound_datum *champion = DATA_ARRAY_ELEMENT(sound_data, sound_datum, champion_sound_index);
    sound_datum *challenger = DATA_ARRAY_ELEMENT(sound_data, sound_datum, challenger_sound_index);

    int16_t challenger_priority = sound_classes[
        TAG_GET(sound_definition, challenger->definition_index)->class_index].priority;
    int champion_priority = sound_classes[
        TAG_GET(sound_definition, champion->definition_index)->class_index].priority;

    if ( challenger_priority <= champion_priority )
    {
        if ( challenger_priority != champion_priority )
            return 0;                                /* strictly lower priority: no preempt */
        {
            float champion_distance =
                source_distance_squared((uint16_t)champion->listener_index, &champion->source);
            if ( challenger_distance_squared >= champion_distance )
                return 0;                            /* equal priority, not closer: no preempt */
        }
    }
    return 1;
}
