/* action_guard_end @0x83823C40 — clear the actor's "guard" position/structure cache when guard-active
 * (byte offset 161, word offset 242, dword offset 122 — inside actor_datum's action_data union, no DB
 * struct name established for this arm). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_postcombat_type.h"
#include "headers/blam_data_globals.h"


void action_guard_end(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.action_data.___u0.guard.post_combat )
    {
        actor->external_orders.postcombat_type = actor_postcombat_none;
        actor->external_orders.postcombat_prop_index = -1;
    }
}
