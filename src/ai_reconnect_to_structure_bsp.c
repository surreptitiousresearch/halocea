/* ai_reconnect_to_structure_bsp @0x836EA1E0 — after a structure BSP swap, re-attach every encounterless
 * actor that had been detached from an encounter belonging to the newly-loaded BSP. Walks the
 * encounterless actor list; for each actor whose stored disconnected_encounter_index is valid and whose
 * encounter definition's runtime structure-BSP reference matches the current BSP, detaches it from the
 * encounterless list and re-attaches it (active) to that encounter with its saved disconnected squad
 * index. Counterpart to ai_disconnect_from_structure_bsp, which populated the disconnected_* fields. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/encounter_definition.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern int global_structure_bsp_index_get(void);
extern void encounterless_detach_actor(int actor_index);
extern void encounter_attach_actor(int actor_index, int encounter_index, int16_t squad_index, uint8_t has_previous_team);

void ai_reconnect_to_structure_bsp(void)
{
    int16_t current_bsp = global_structure_bsp_index_get();

    int actor_index = ai_globals->first_encounterless_actor_index;
    if ( actor_index != -1 )
    {
        int next;
        do
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
            next = actor->meta.next_actor_index;

            int disconnected_encounter_index = actor->meta.disconnected_encounter_index;
            if ( disconnected_encounter_index != -1
              && ((const encounter_definition *)global_scenario->ai_encounters.address)
                     [(uint16_t)disconnected_encounter_index].runtime_structure_bsp_reference_index == current_bsp )
            {
                encounterless_detach_actor(actor_index);
                encounter_attach_actor(actor_index, disconnected_encounter_index,
                        actor->meta.disconnected_squad_index, 1u);
            }

            actor_index = next;
        }
        while ( next != -1 );
    }
}
