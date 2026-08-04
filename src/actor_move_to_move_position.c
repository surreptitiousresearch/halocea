/* actor_move_to_move_position @0x837CB5E8 — command an actor to move to a scripted "move position": clears
 * the actor's firing-position claim, un-dormants it, and if it's already moving to this exact
 * position (destination_type == 4 and move_position_index match), either reports done (timeslice
 * clear or refreshed_this_tick set) or asks for a path refresh without a new destination. Otherwise
 * records the new target (move_position_index in orders._opaque, clears ignore_target_object_index,
 * sets destination_type to 4), copies the cached destination/step block (24 bytes) into the
 * active-move slot, and asks for a full path refresh with a new destination. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/path_state.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


#include "headers/path_state.h"
extern void actor_set_dormant(uint16_t actor_index, uint8_t dormant);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_to_move_position(int actor_index, int16_t move_position_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor->firing_positions.current_position_index = -1;
    actor_set_dormant(actor_index, 0);

    if ( actor->control.path.destination_orders.destination_type == _destination_move_position && actor->control.path.destination_orders.___u3.move_position_index == move_position_index )
    {
        if ( !actor->meta.timeslice || actor->control.path.refreshed_this_tick )
            return 1;
        return actor_path_refresh(actor_index, 0, 0);
    }

    actor->orders.move.destination.___u3.move_position_index = move_position_index;
    actor->orders.move.destination.ignore_target_object_index = -1;
    actor->orders.move.destination.destination_type = _destination_move_position;

    int *source = (int *)&actor->orders.move.destination.destination_type - 1;
    int *destination = (int *)&actor->control.path.destination_orders.destination_type - 1;
    for ( int i = 0; i < 6; i++ )
        *++destination = *++source;

    return actor_path_refresh(actor_index, 1u, 0);
}
