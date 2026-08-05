#include <stdint.h>
#include "headers/flee_state_data.h"
#include "headers/action_state_data.h"
#include "headers/actor_action.h"

extern uint8_t action_flee_setup(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_state_entry, uint8_t flee_from_last_visible_location, uint8_t allow_occluded_points, flee_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_try_to_panic(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_flee_transition)
{
    flee_state_data state;

    if ( !action_flee_setup(actor_index, panic_type, panic_prop_index, force_flee_transition, 0, 0, &state) )
        return 0;

    actor_action_change(actor_index, actor_action_flee, (action_state_data *)&state);
    return 1;
}
