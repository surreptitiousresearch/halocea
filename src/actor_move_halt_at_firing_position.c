/* actor_move_halt_at_firing_position @0x837CB998 — like actor_move_halt.c, but takes the fast path when the
 * actor is already in movement mode 4 (see action_fight_control.c's "firing_position" mode-word at +350):
 * redirect straight to its held firing-position index (+952) via actor_move_to_firing_position, or
 * actor_move_halt if it has none. Otherwise falls back to the same repath sequence as actor_move_halt.c
 * (clear path state +1132, mark repath +1024, copy the 6-dword pending-goal block +1020..+1128), then
 * refreshes the path. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/path_state.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_vehicle_driver_type.h"


#include "headers/path_state.h"
#include "headers/path_state.h"
extern uint8_t actor_move_halt(int actor_index);
extern uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_halt_at_firing_position(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
    {
        int16_t firing_position_index = actor->firing_positions.current_position_index;

        if ( firing_position_index == -1 )
            return actor_move_halt(actor_index);

        return actor_move_to_firing_position(actor_index, firing_position_index, nullptr);
    }

    if ( actor->control.path.destination_orders.destination_type != _destination_halt )
    {
        actor->orders.move.destination.destination_type = _destination_halt;

        uint32_t *source = (uint32_t *)&actor->orders.look.idle_look_type;
        uint32_t *dest = (uint32_t *)&actor->orders.combat.target_point.z;
        for ( int count = 6; count; --count )
            *++dest = *++source;
    }

    return actor_path_refresh(actor_index, 1u, nullptr);
}
