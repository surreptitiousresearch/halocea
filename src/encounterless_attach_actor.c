/* encounterless_attach_actor @0x837097E8 — push an actor onto the head of the global encounterless-actor list
 * and reset its activation/state flags. No-op if AI is not initialised for the current map. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_flush_position_indices(uint16_t actor_index);

void encounterless_attach_actor(int actor_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->meta.next_actor_index = ai_globals->first_encounterless_actor_index;
    ai_globals->first_encounterless_actor_index = actor_index;
    char active_mask = (actor->meta.active == 0) - 1;  /* 0x00 when inactive, 0xFF when active */
    actor->meta.encounterless = 1;
    actor->meta.encounterless_active_timer = active_mask & 0x5A;
    actor_flush_position_indices(actor_index);
}
