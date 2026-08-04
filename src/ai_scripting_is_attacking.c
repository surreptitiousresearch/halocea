/* ai_scripting_is_attacking @0x8376FF98 — return 1 if any platoon in the addressed encounter scope is
 * attacking (i.e. not defending), 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/platoon_datum.h"
#include "headers/ai_index_platoon_iterator.h"
#include "headers/blam_data_globals.h"

extern void ai_index_platoon_iterator_new(unsigned int ai_index, ai_index_platoon_iterator *iterator);

uint8_t ai_scripting_is_attacking(int ai_index)
{
    if ( ai_index == -1 )
        return 0;

    ai_index_platoon_iterator iterator;
    ai_index_platoon_iterator_new(ai_index, &iterator);
    int platoon_index = iterator.platoon_index;
    while ( iterator.encounter_index != -1 )
    {
        if ( platoon_index > iterator.last_platoon_index )
            break;
        __int16 base_platoon_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, iterator.encounter_index)->platoon_base;
        platoon_datum *platoon = &platoon_array[(__int16)(base_platoon_index + platoon_index++)];
        if ( !platoon )
            break;
        if ( !platoon->defending )
            return 1;
    }
    return 0;
}
