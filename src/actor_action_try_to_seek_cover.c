/* actor_action_try_to_seek_cover @0x837F3324 — attempts to transition the actor into the flee action to seek
 * cover from its currently-tracked hostile prop (actor+0x270, same "closest hostile prop" index as
 * actor_action_handle_berserking_from_proximity.c). Mirrors actor_action_try_to_panic.c's setup-then-change
 * pattern, with panic_type = _actor_panic_none and force_state_entry=0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/action_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern uint8_t action_flee_setup(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_state_entry, uint8_t flee_from_last_visible_location, uint8_t allow_occluded_points, flee_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_try_to_seek_cover(int actor_index, uint8_t cover_from_last_visible_location, uint8_t allow_occluded_points)
{
    flee_state_data state;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int closest_prop_index = actor->target.target_prop_index;

    if ( !action_flee_setup(actor_index, _actor_panic_none, closest_prop_index, 0,
            cover_from_last_visible_location, allow_occluded_points, &state) )
        return 0;

    actor_action_change(actor_index, actor_action_flee, (action_state_data *)&state);
    return 1;
}
