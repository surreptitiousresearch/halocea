#include <stdint.h>
#include <string.h>

#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void actor_clear_orders(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(&actor->orders, 0, sizeof(actor->orders));
    actor->orders.move.animation.impulse = -1;
    actor->orders.move.override_movement_type = -1;
    actor->orders.move.override_movement_facing = -1;
}
