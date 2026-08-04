/* ai_index_squad_iterator_new @0x8376F010 — initialize an iterator over the squads selected by an AI index.
 * Low 16 bits = encounter index; top two bits select a sub-scope: 0 = all squads in the encounter, 1 = all
 * squads but restricted to a platoon (platoon index in byte 1), 2 = a single squad (squad index in byte 1),
 * 3 = invalid. An out-of-range or uninitialized state marks the iterator empty (encounter_index = -1). */

#include "headers/ai_index_squad_iterator.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/ai_globals.h"
#include "headers/ai_index_scope.h"
#include "headers/blam_data_globals.h"


void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator)
{
    scenario *scen = global_scenario;
    iterator->encounter_index = (unsigned short)ai_index;

    if ( !scen || !ai_globals->ai_initialized_for_map || (unsigned short)ai_index >= scen->ai_encounters.count )
    {
        iterator->encounter_index = -1;
        return;
    }

    encounter_definition *encounter =
        &((encounter_definition *)scen->ai_encounters.address)[(unsigned short)ai_index];

    if ( ai_index >> 30 >= _ai_index_squad )   /* squad (2) or reserved/invalid (3) */
    {
        if ( ai_index >> 30 == _ai_index_squad && ((ai_index >> 8) & 0xFF) < encounter->squads.count )
        {
            iterator->last_squad_index = (ai_index >> 8) & 0xFF;   /* BYTE1 */
            iterator->next_squad_index = (ai_index >> 8) & 0xFF;
            iterator->squad_index = -1;
            iterator->required_platoon_index = -1;
            return;
        }
        iterator->encounter_index = -1;
        return;
    }

    iterator->squad_index = -1;
    iterator->next_squad_index = 0;
    iterator->last_squad_index = encounter->squads.count - 1;
    if ( !(ai_index >> 30) )   /* _ai_index_encounter: all squads, no platoon restriction */
    {
        iterator->required_platoon_index = -1;
        return;
    }
    /* _ai_index_platoon: restrict to the platoon in byte 1 */
    iterator->required_platoon_index = (ai_index >> 8) & 0xFF;      /* BYTE1: platoon restriction */
}
