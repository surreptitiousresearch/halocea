/* sound_stop_impulse_by_source_and_definition @0x83718BE0 — finds the first active, non-looping sound
 * whose definition index and source identifier match, and stops it as an impulse sound.
 *
 * Sound datum (176 bytes, see refresh_sound.c): +2 (word) type (_sound_impulse for one-shots),
 * +8 definition index, +12 source identifier. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_datum.h"
#include "headers/sound_type.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void sound_stop_impulse(uint16_t sound_index);

void sound_stop_impulse_by_source_and_definition(int source_identifier, int definition_index)
{
    int index = data_next_index(sound_data, -1);
    if ( index == -1 )
        return;

    while ( 1 )
    {
        sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, index);

        if ( datum->type == _sound_impulse
          && datum->source_identifier == source_identifier
          && datum->definition_index == definition_index )
            break;

        index = data_next_index(sound_data, index);
        if ( index == -1 )
            return;
    }

    sound_stop_impulse(index);
}
