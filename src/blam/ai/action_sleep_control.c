/* action_sleep_control @0x83826DD8 — clears the actor's idle-look mode
 * (resolved to the DB-named actor_datum member orders.look.idle_look_type @0x3FC). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


void action_sleep_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->orders.look.idle_look_type = _idle_look_none;
}
