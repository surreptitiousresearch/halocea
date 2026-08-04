/* actor_move_keep_moving_past_destination @0x837C7380 — sets the "keep moving past destination" flag in
 * both the move orders (byte 1026) and the path control copy (byte 1134). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_move_keep_moving_past_destination(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->orders.move.destination.keep_moving = 1;
    actor->control.path.destination_orders.keep_moving = 1;
}
