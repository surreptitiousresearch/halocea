/* sound_set_definition_begin @0x83716350 — stages a pending definition-index change for a sound
 * (sound_datum.definition_index at +8, sound_datum.next_definition_index at +152). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_datum.h"
#include "headers/blam_data_globals.h"


void sound_set_definition_begin(uint16_t sound_index, int definition_index)
{
    sound_datum *sound = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
    if (sound->definition_index != definition_index)
        sound->next_definition_index = definition_index;
}
