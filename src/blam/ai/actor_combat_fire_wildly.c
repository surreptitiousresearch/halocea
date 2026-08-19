/* actor_combat_fire_wildly @0x837B78F8 — sets an actor's combat-fire mode to "wildly" (4) for fire_ticks. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_fire_state.h"
#include "headers/blam_data_globals.h"


void actor_combat_fire_wildly(int actor_index, int16_t fire_ticks)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->control.fire_state = actor_fire_state_wild;
    actor->control.fire_state_timer = fire_ticks;
}
