/* encounter_build_firing_position_owner_actor_indices @0x8370D190 — fill a per-firing-position table
 * (sized by the encounter's firing-position count, scenario ai_encounters stride 176, count dword +152)
 * with NONE, then walk the encounter's actor list (encounter_data stride 108, first-actor dword +20; or
 * the global encounterless list when encounter_index is NONE) recording each actor's index at its claimed
 * firing-position slot (actor word +952 = firing position index, next-in-encounter link dword +44).
 *
 * DEVIATION: when ai_globals->ai_initialized_for_map is false the compiled code loads the list head from
 * an uninitialized stack slot (lwz r10, var_28 — never stored). Harmless: the walk loop re-checks the
 * same flag and never executes on that path. Reproduced as a 0-init to keep the code well-defined. */

#include <stdint.h>
#include <string.h>
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/ai_globals.h"
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void encounter_build_firing_position_owner_actor_indices(int encounter_index,
        int *firing_position_owner_actor_indices)
{
    memset(firing_position_owner_actor_indices, -1,
           4 * ((encounter_definition *)global_scenario->ai_encounters.address)
               [(uint16_t)encounter_index].firing_positions.count);

    int actor_index = 0;   /* shipped code leaves this uninitialized on the !initialized path */
    if ( ai_globals->ai_initialized_for_map )
    {
        if ( encounter_index == -1 )
            actor_index = ai_globals->first_encounterless_actor_index;
        else
            /* recovered: (unsigned int *)encounter_data->data + 27*idx + 5
             * -> encounter_datum.first_actor_index (27*4 = 108 = sizeof(encounter_datum), +5*4 = +20). */
            actor_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum,
                                             encounter_index)->first_actor_index;
    }

    while ( ai_globals->ai_initialized_for_map && actor_index != -1 )
    {
        actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
        /* recovered: *((unsigned __int16*)actor+476) -> +952 firing_positions.current_position_index */
        int16_t firing_position_index = (actor)->firing_positions.current_position_index;
        int next_actor_index = actor->meta.next_actor_index;  /* +44 next-in-encounter link */
        if ( firing_position_index != -1 )
            firing_position_owner_actor_indices[firing_position_index] = actor_index;
        actor_index = next_actor_index;
    }
}
