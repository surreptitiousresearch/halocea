/* ai_scripting_spawn_actor @0x8376F8D8 — spawn a single actor of the addressed encounter scope. The low 16
 * bits are the encounter index; the top two bits select the scope. A squad-scoped index (>>30 == 2) carries
 * the squad index in byte 1 directly; a platoon-scoped index (>>30 == 1) is resolved to the first squad of
 * that platoon. The chosen squad is then spawned via encounter_spawn_actor.
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F904 (explicit squad) and 0x8376F954 (platoon search key) are
 * `extrwi rX, r7, 8, 8` (rlwinm SH=16 MB=24 ME=31, words 0x54EB863E / 0x54E7863E), i.e. (x >> 16) & 0xFF
 * — r7 is the saved copy of the raw ai_index (mr r7, r3 @0x8376F8DC), r3 having been narrowed to the
 * encounter index at 0x8376F8FC. Now AI_INDEX_SUB_INDEX. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_globals.h"
#include "headers/ai_index.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_spawn_actor(int encounter_index, int16_t squad_index);

void ai_scripting_spawn_actor(unsigned int ai_index)
{
    if ( !ai_globals->ai_initialized_for_map || ai_index == -1 )
        return;

    int16_t squad_index = AI_INDEX_SUB_INDEX(ai_index);
    if ( AI_INDEX_SCOPE(ai_index) != _ai_index_squad )
        squad_index = -1;

    int16_t chosen_squad = squad_index;
    if ( squad_index == -1 && AI_INDEX_SCOPE(ai_index) == _ai_index_platoon )
    {
        /* Platoon-scoped: find the first squad belonging to the requested platoon (squad def +0x22). */
        encounter_definition *encounter_def = &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)ai_index];
        int squad_count = encounter_def->squads.count;
        if ( squad_count > 0 )
        {
            squad_definition *squads = (squad_definition *)encounter_def->squads.address;
            for ( int i = 0; i < squad_count; i = (int16_t)(i + 1) )
            {
                if ( squads[i].platoon_index == AI_INDEX_SUB_INDEX(ai_index) )
                {
                    chosen_squad = i;
                    break;
                }
            }
        }
    }

    if ( chosen_squad != -1 )
        encounter_spawn_actor((uint16_t)ai_index, chosen_squad);
}
