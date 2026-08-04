/* actor_is_leaping @0x8371DD08 — an actor is "leaping" only while running the charge action
 * (state.action == actor_action_charge, the DB-verified actor_action enumerator == 10); the charge
 * sub-state check is delegated to action_charge_is_leaping. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

extern uint8_t action_charge_is_leaping(int actor_index);

uint8_t actor_is_leaping(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.action == actor_action_charge )
        return action_charge_is_leaping(actor_index);
    return 0;
}
