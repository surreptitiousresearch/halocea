/* ai_index_squad_iterator_next @0x8376F0E0 — advance the AI-index squad iterator, returning the next squad
 * datum that matches the platoon restriction (if any), or null at the end. The squad datum is located via the
 * encounter's runtime base-squad index (encounter_data element +4) plus the per-encounter squad ordinal.
 *
 * Deviation: the decompiler renders the array index as HIWORD(iterator->squad_index); the disassembly
 * (lwz 8(r11); add; extsh; slwi r,5) shows it is the full squad_index dword added to the 16-bit base index,
 * with squad_array elements 32 bytes wide — reconstructed accordingly. The platoon index is read raw from the
 * squad_definition.platoon_index (offset 34 in the 232-byte squad_definition element). */

#include <stdint.h>
#include "headers/ai_index_squad_iterator.h"
#include "headers/squad_datum.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/encounter_datum.h"
#include "headers/squad_definition.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator)
{
    int encounter_index = iterator->encounter_index;
    if ( encounter_index == -1 )
        return 0;

    encounter_definition *encounter =
        &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned short)encounter_index];
    encounter_datum *encounter_runtime = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);

    if ( iterator->next_squad_index > iterator->last_squad_index )
        return 0;

    int required_platoon_index = iterator->required_platoon_index;
    while ( 1 )
    {
        int next = iterator->next_squad_index;
        iterator->next_squad_index = next + 1;
        iterator->squad_index = next;

        if ( required_platoon_index == -1
          || ((squad_definition *)encounter->squads.address)[next].platoon_index == required_platoon_index )
            break;

        if ( iterator->next_squad_index > iterator->last_squad_index )
            return 0;
    }

    int base_squad_index = (unsigned short)encounter_runtime->squad_base;
    return &squad_array[(int16_t)(base_squad_index + iterator->squad_index)];
}
