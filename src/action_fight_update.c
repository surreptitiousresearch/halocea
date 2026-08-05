/* action_fight_update @0x838263E0 — countdown timer (word +78) for the "fight" action; when it reaches
 * zero and the actor's fighting flag (byte +1156) is set, discard the actor's held firing position (word
 * +476) unless it is invalid (0xFFFF) or byte +954 is set. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);

void action_fight_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t timer = actor->state.action_data.___u0.fight.firing_position_timer;

    if ( timer > 0 && actor->control.path.at_destination )
    {
        int16_t remaining = timer - 1;

        actor->state.action_data.___u0.fight.firing_position_timer = remaining;
        if ( !remaining )
        {
            uint16_t firing_position_index = actor->firing_positions.current_position_index;

            if ( firing_position_index != 0xFFFF && !actor->firing_positions.current_position_found_outside_range )
                actor_discard_firing_position(actor_index, firing_position_index, 0);
        }
    }
}
