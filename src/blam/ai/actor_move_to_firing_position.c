/* actor_move_to_firing_position @0x837CB6D8 — command an actor to move to an encounter firing position:
 * un-dormants it, and if it's already moving to this exact position
 * (control.path.destination_orders.destination_type == _destination_firing_position and firing_position_index in
 * control.path.destination_orders._opaque match), either reports done (meta.timeslice clear or
 * control.path.refreshed_this_tick set) or asks for a path refresh without a new destination.
 * Otherwise records the new target (firing_position_index in orders.move.destination._opaque,
 * clears firing_positions.moved_away_from_firing_position and orders.move.destination.ignore_target_object_index,
 * sets orders.move.destination.destination_type to 3), copies the cached destination/step block (24 bytes,
 * orders.move.destination -> control.path.destination_orders) into the active-move slot, and asks for a
 * full path refresh with a new destination. Sibling of actor_move_to_move_position.c (mode 4). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/path_state.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_set_dormant(actor_index, 0);

    if ( actor->control.path.destination_orders.destination_type == _destination_firing_position && (uint16_t)actor->control.path.destination_orders.___u3.firing_position_index == firing_position_index )
    {
        if ( !actor->meta.timeslice || actor->control.path.refreshed_this_tick )
            return 1;
        return actor_path_refresh(actor_index, 0, cached_path_state);
    }

    actor->orders.move.destination.___u3.firing_position_index = firing_position_index;
    actor->orders.move.destination.keep_moving = 0;
    actor->firing_positions.moved_away_from_firing_position = 0;
    actor->orders.move.destination.destination_type = _destination_firing_position;
    actor->orders.move.destination.ignore_target_object_index = -1;

    int *source = (int *)&actor->orders.move.destination - 1;
    int *destination = (int *)&actor->control.path.destination_orders - 1;
    for ( int i = 0; i < 6; i++ )
        *++destination = *++source;

    return actor_path_refresh(actor_index, 1u, cached_path_state);
}
