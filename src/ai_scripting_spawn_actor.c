/* ai_scripting_spawn_actor @0x8376F8D8 — spawn a single actor of the addressed encounter scope. The low 16
 * bits are the encounter index; the top two bits select the scope. A squad-scoped index (>>30 == 2) carries
 * the squad index in byte 1 directly; a platoon-scoped index (>>30 == 1) is resolved to the first squad of
 * that platoon. The chosen squad is then spawned via encounter_spawn_actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_globals.h"
#include "headers/ai_index_scope.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_spawn_actor(uint16_t encounter_index, int16_t squad_index);

void ai_scripting_spawn_actor(unsigned int ai_index)
{
    if ( !ai_globals->ai_initialized_for_map || ai_index == -1 )
        return;

    __int16 squad_index = (ai_index >> 8) & 0xFF;
    if ( ai_index >> 30 != _ai_index_squad )
        squad_index = -1;

    __int16 chosen_squad = squad_index;
    if ( squad_index == -1 && ai_index >> 30 == _ai_index_platoon )
    {
        /* Platoon-scoped: find the first squad belonging to the requested platoon (squad def +0x22). */
        encounter_definition *encounter_def = &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)ai_index];
        int squad_count = encounter_def->squads.count;
        if ( squad_count > 0 )
        {
            squad_definition *squads = (squad_definition *)encounter_def->squads.address;
            for ( int i = 0; i < squad_count; i = (__int16)(i + 1) )
            {
                if ( squads[i].platoon_index == ((ai_index >> 8) & 0xFF) )
                {
                    chosen_squad = i;
                    break;
                }
            }
        }
    }

    if ( chosen_squad != -1 )
        encounter_spawn_actor((unsigned __int16)ai_index, chosen_squad);
}
