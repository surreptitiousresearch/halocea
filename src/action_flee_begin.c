/* action_flee_begin @0x83825898 — enters the flee behavior; all working state resolved to the DB-named
 * flee_state_data arm of the action_data union. Zeroes flee.flee_timer; if flee.panic_type > _actor_panic_none, also
 * clears state.searching. If flee.flee_stationary_ticks == 0 and the actor has a valid unit
 * (meta.unit_index != -1) and panic_type (re-read fresh) falls in [9,12], kicks off a blind sprint
 * toward that unit via unit_start_running_blindly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"

extern void unit_start_running_blindly(int unit_index);

void action_flee_begin(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t flee_mode_value = actor->state.action_data.___u0.flee.panic_type;

    actor->state.action_data.___u0.flee.flee_timer = 0;

    if ( flee_mode_value > 0 )
        actor->state.searching = 0;

    if ( !actor->state.action_data.___u0.flee.flee_stationary_ticks )
    {
        int flee_target = actor->meta.unit_index;

        if ( flee_target != -1 )
        {
            int16_t flee_mode_value2 = actor->state.action_data.___u0.flee.panic_type;

            if ( flee_mode_value2 >= _actor_panic_grenade_attached_to_us
              && flee_mode_value2 <= _actor_panic_burning_to_death )
                unit_start_running_blindly(flee_target);
        }
    }
}
