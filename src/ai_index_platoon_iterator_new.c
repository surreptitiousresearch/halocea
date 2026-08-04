/* ai_index_platoon_iterator_new @0x8376EEA8 — initialize a platoon iterator from an AI index. The low 16 bits
 * select the encounter; the top two bits (ai_index >> 30) select the scope:
 *   0 = whole encounter        — visit every platoon (0 .. platoons.count-1)
 *   1 = explicit platoon       — byte 1 is the platoon index
 *   2 = squad-scoped           — byte 1 is a squad index; its platoon field (squad+0x22) is the single platoon
 *   3 = invalid                — empties the iterator
 * An out-of-range encounter, AI not yet initialized for the map, or an out-of-range platoon empties the iterator
 * (encounter_index = -1). */

#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_globals.h"
#include "headers/ai_index_platoon_iterator.h"
#include "headers/ai_index_scope.h"
#include "headers/blam_data_globals.h"


void ai_index_platoon_iterator_new(unsigned int ai_index, ai_index_platoon_iterator *iterator)
{
    iterator->encounter_index = (unsigned short)ai_index;

    if ( !global_scenario
      || !ai_globals->ai_initialized_for_map
      || (unsigned short)ai_index >= global_scenario->ai_encounters.count )
    {
        iterator->encounter_index = -1;
        return;
    }

    encounter_definition *encounter =
        &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned short)ai_index];

    switch ( ai_index >> 30 )
    {
        case _ai_index_encounter:
            iterator->platoon_index = 0;
            iterator->last_platoon_index = encounter->platoons.count - 1;
            return;

        case _ai_index_squad:
            if ( ((ai_index >> 8) & 0xFF) >= encounter->squads.count )
                iterator->platoon_index = -1;
            else
                iterator->platoon_index =
                    ((squad_definition *)encounter->squads.address)[(ai_index >> 8) & 0xFF].platoon_index;
            break;

        case 3:   /* reserved/invalid scope (no DB name) */
            iterator->encounter_index = -1;
            return;

        default:   /* _ai_index_platoon — explicit platoon index in byte 1 */
            iterator->platoon_index = (ai_index >> 8) & 0xFF;
            break;
    }

    int platoon_index = iterator->platoon_index;
    if ( platoon_index < 0 || platoon_index >= encounter->platoons.count )
        iterator->encounter_index = -1;
    else
        iterator->last_platoon_index = platoon_index;
}
