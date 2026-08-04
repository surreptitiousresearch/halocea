/* actor_firing_blindly @0x837B7958 — true when the actor's combat fire mode (control.fire_state @1522, same field
 * action_combat_fire_wildly.c writes) is actor_fire_state_wild (4). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_fire_state.h"
#include "headers/blam_data_globals.h"


uint8_t actor_firing_blindly(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    return actor->control.fire_state == actor_fire_state_wild;
}
