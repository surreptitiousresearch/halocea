/* ai_index_squad_iterator_new @0x8376F010 — initialize an iterator over the squads selected by an AI index.
 * Low 16 bits = encounter index; top two bits select a sub-scope: 0 = all squads in the encounter, 1 = all
 * squads but restricted to a platoon (platoon index in byte 1), 2 = a single squad (squad index in byte 1),
 * 3 = invalid. An out-of-range or uninitialized state marks the iterator empty (encounter_index = -1).
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F074 (squad scope) and 0x8376F0C8 (platoon restriction) are
 * `extrwi r11, r3, 8, 8` (rlwinm SH=16 MB=24 ME=31, word 0x546B863E), i.e. (x >> 16) & 0xFF.
 * Now AI_INDEX_SUB_INDEX. */

#include "headers/ai_index_squad_iterator.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/ai_globals.h"
#include "headers/ai_index.h"
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

    if ( AI_INDEX_SCOPE(ai_index) >= _ai_index_squad )   /* squad (2) or reserved/invalid (3) */
    {
        if ( AI_INDEX_SCOPE(ai_index) == _ai_index_squad && AI_INDEX_SUB_INDEX(ai_index) < encounter->squads.count )
        {
            iterator->last_squad_index = AI_INDEX_SUB_INDEX(ai_index);
            iterator->next_squad_index = AI_INDEX_SUB_INDEX(ai_index);
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
    if ( !AI_INDEX_SCOPE(ai_index) )   /* _ai_index_encounter: all squads, no platoon restriction */
    {
        iterator->required_platoon_index = -1;
        return;
    }
    /* _ai_index_platoon: restrict to the platoon in byte 1 */
    iterator->required_platoon_index = AI_INDEX_SUB_INDEX(ai_index);
}
