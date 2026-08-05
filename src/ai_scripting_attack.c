/* ai_scripting_attack @0x83770050 — script command: order the AI-index-selected platoons to attack, i.e. clear
 * each platoon's "defending" flag. Walks the platoons via an ai_index_platoon_iterator; the absolute platoon datum
 * is encounter_data[encounter].base_platoon_index + relative platoon index.
 *
 * encounter_data is the runtime encounter datum array (108-byte elements; base platoon index at +8). platoon_array
 * elements are 16 bytes with "defending" at offset 0 (disasm-verified at 0x837700A0..0x837700C8). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/platoon_datum.h"
#include "headers/ai_index_platoon_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"

extern void ai_index_platoon_iterator_new(unsigned int ai_index, ai_index_platoon_iterator *iterator);

void ai_scripting_attack(int ai_index)
{
    if ( ai_index == -1 )
        return;

    ai_index_platoon_iterator iterator;
    ai_index_platoon_iterator_new(ai_index, &iterator);

    int platoon_index = iterator.platoon_index;
    while ( iterator.encounter_index != -1 )
    {
        if ( platoon_index > iterator.last_platoon_index )
            break;

        unsigned short base_platoon_index =
            DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, iterator.encounter_index)->platoon_base;
        platoon_datum *platoon = &platoon_array[(int16_t)(base_platoon_index + platoon_index)];
        ++platoon_index;
        if ( !platoon )
            break;
        platoon->defending = 0;
    }
}
