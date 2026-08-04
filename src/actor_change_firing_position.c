/* actor_change_firing_position @0x837F1880 — assign (or clear) an actor's firing position: -1 halts the
 * actor and clears its claim (word +952), returning NONE. Otherwise, discards any different previously-
 * held firing position, halts and clears a displaced previous owner (if any), and — if not already at the
 * requested position — claims it (word +952), resets the "arrived" flag (byte +955) and the "path already
 * known available" flag (byte +954, from cached_path_available), and attempts the move; on failure the
 * claim is released. Returns the actor's resulting firing_position claim. `firing_position` itself is
 * unused (dead parameter, matches DB's own 6-arg signature). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/firing_position.h"
#include "headers/path_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_move_halt(uint16_t actor_index);
extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);
extern uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state);

int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( firing_position_index == -1 )
    {
        actor_move_halt(actor_index);
        actor->firing_positions.current_position_index = -1;
        return -1;
    }

    __int16 current_claim = actor->firing_positions.current_position_index;
    if ( current_claim != -1 && current_claim != firing_position_index )
        actor_discard_firing_position(actor_index, current_claim, 1u);

    if ( previous_owner != -1 )
    {
        actor_datum *previous_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, previous_owner);
        actor_move_halt(previous_owner);
        previous_actor->firing_positions.current_position_index = -1;
    }

    if ( actor->firing_positions.current_position_index != firing_position_index )
    {
        actor->firing_positions.current_position_index = firing_position_index;
        actor->firing_positions.moved_away_from_firing_position = 0;
        actor->firing_positions.current_position_found_outside_range = !cached_path_available;

        if ( !actor_move_to_firing_position(actor_index, firing_position_index,
                 cached_path_available == 0 ? 0 : cached_path_state) )
            actor->firing_positions.current_position_index = -1;
    }

    return actor->firing_positions.current_position_index;
}
