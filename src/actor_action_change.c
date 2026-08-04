/* actor_action_change @ 0x837F2D98 — switch an actor to a new action. Ends the current action (end callback),
 * updates the action-class category counter (actor->state.mode) when crossing the combat-class threshold (class 3),
 * clears discarded firing positions, copies the supplied action state into the action data block (actor->state.action_data,
 * data_size bytes) when present, records the new action type (actor->state.action), raises the action-changed flag
 * (actor->state.action_changed), and runs the new action's begin callback. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/action_state_data.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


extern void actor_clear_discarded_firing_positions(uint16_t actor_index, uint8_t clear_temporary_only);

void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    void (__fastcall *end)(int) = global_action_functions[actor->state.action].end;
    if ( end )
        end(actor_index);

    int16_t action_category = actor->state.mode;
    if ( global_action_functions[new_action_type].action_class )
    {
        if ( action_category < _actor_mode_combat )
            actor->state.mode = _actor_mode_combat;
    }
    else if ( action_category >= _actor_mode_combat )
    {
        actor->state.mode = _actor_mode_alert;
    }

    actor_clear_discarded_firing_positions(actor_index, 0);

    unsigned int data_size = global_action_functions[new_action_type].data_size;
    if ( data_size && new_action_data )
        memcpy(&actor->state.action_data, new_action_data, data_size);

    actor->state.action = new_action_type;
    actor->state.action_changed = 1;

    void (__fastcall *begin)(int) = global_action_functions[(int16_t)new_action_type].begin;
    if ( begin )
        begin(actor_index);
}
