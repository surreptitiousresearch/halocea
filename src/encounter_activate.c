/* encounter_activate @0x8370A900 — activate an encounter if it belongs to the current structure BSP (or none).
 * Activates every actor currently chained into the encounter, stamps the activation time, and marks the
 * encounter active. Returns the encounter's active flag. No-op (returns current flag) for encounters bound to a
 * different BSP or already active.
 *
 * Deviation: in the (unreachable) branch where AI is not initialised for the map, Hex-Rays reads an
 * uninitialised stack slot for the start index; the subsequent loop is guarded on ai_initialized_for_map and
 * never executes, so the start index is modelled as -1. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


extern int actor_set_active(uint16_t actor_index, uint8_t active);
extern int game_time_get(void);

uint8_t encounter_activate(int encounter_index)
{
    encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    __int16 bsp_index = ((encounter_definition *)global_scenario->ai_encounters.address)
                        [(unsigned __int16)encounter_index].runtime_structure_bsp_reference_index;
    if ( bsp_index == -1 || bsp_index == global_structure_bsp_index )
    {
        if ( !encounter->active )
        {
            ai_globals_t *globals = ai_globals;
            int actor_index;
            if ( ai_globals->ai_initialized_for_map )
            {
                if ( encounter_index == -1 )
                    actor_index = ai_globals->first_encounterless_actor_index;
                else
                    actor_index = encounter->first_actor_index;
            }
            else
            {
                actor_index = -1;
            }

            while ( globals->ai_initialized_for_map && actor_index != -1 )
            {
                int current = actor_index;
                actor_index = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.next_actor_index;
                actor_set_active(current, 1u);
                globals = ai_globals;
            }
        }
        encounter->last_active_time = game_time_get();
        encounter->active = 1;
    }
    return encounter->active;
}
