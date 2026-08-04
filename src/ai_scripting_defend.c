/* ai_scripting_defend @0x837700E0 — mark every platoon in the addressed encounter scope as defending, so
 * its actors hold position rather than maneuver. The AI index selects the encounter and the platoon range
 * via ai_index_platoon_iterator. */

#include "headers/data_array.h"
#include "headers/platoon_datum.h"
#include "headers/encounter_datum.h"
#include "headers/ai_index_platoon_iterator.h"
#include "headers/blam_data_globals.h"

extern void ai_index_platoon_iterator_new(unsigned int ai_index, ai_index_platoon_iterator *iterator);

void ai_scripting_defend(int ai_index)
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
        platoon->defending = 1;
    }
}
