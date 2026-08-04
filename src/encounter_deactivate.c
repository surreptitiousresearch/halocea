/* encounter_deactivate @0x8370AAB8 — deactivate an AI encounter: clear its active flag (encounter datum
 * active flag), drop any pursuit, then walk its actor chain and set each active actor inactive. The chain
 * head is the encounter's first_actor_index, or the global encounterless-actor list when encounter_index is -1.
 * DEVIATION: when AI isn't initialized for the map the decompiler leaves the head index uninitialized; the
 * while-loop guard (ai_initialized_for_map) is false in that case so it is never read — set to -1 here. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern void encounter_clear_pursuit(uint16_t encounter_index);
extern int actor_set_active(uint16_t actor_index, uint8_t active);

void encounter_deactivate(int encounter_index)
{
    DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->active = 0;
    encounter_clear_pursuit(encounter_index);

    ai_globals_t *globals = ai_globals;
    int actor_index = -1;
    if ( ai_globals->ai_initialized_for_map )
    {
        if ( encounter_index == -1 )
            actor_index = ai_globals->first_encounterless_actor_index;
        else
            actor_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->first_actor_index;
    }

    while ( globals->ai_initialized_for_map )
    {
        int current = actor_index;
        if ( actor_index == -1 )
            break;
        actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
        actor_index = actor->meta.next_actor_index;   /* next actor in chain */
        if ( actor->meta.active )                        /* actor active */
        {
            actor_set_active(current, 0);
            globals = ai_globals;
        }
    }
}
