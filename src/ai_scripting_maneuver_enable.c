/* ai_scripting_maneuver_enable @0x83770200 — enable or disable maneuvering for every platoon in the
 * addressed encounter scope (sets each platoon's maneuver_disable to the negation of `enabled`). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/platoon_datum.h"
#include "headers/ai_index_platoon_iterator.h"
#include "headers/blam_data_globals.h"

extern void ai_index_platoon_iterator_new(unsigned int ai_index, ai_index_platoon_iterator *iterator);

void ai_scripting_maneuver_enable(int ai_index, uint8_t enabled)
{
    if ( ai_index == -1 )
        return;

    ai_index_platoon_iterator iterator;
    ai_index_platoon_iterator_new(ai_index, &iterator);
    int platoon_index = iterator.platoon_index;
    int last_platoon_index = iterator.last_platoon_index;
    int encounter_index = iterator.encounter_index;

    while ( encounter_index != -1 )
    {
        if ( platoon_index > last_platoon_index )
            break;
        __int16 base_platoon_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->platoon_base;
        platoon_datum *platoon = &platoon_array[(__int16)(base_platoon_index + platoon_index++)];
        if ( !platoon )
            break;
        platoon->maneuver_disable = enabled == 0;
    }
}
