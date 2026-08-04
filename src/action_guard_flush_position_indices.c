/* action_guard_flush_position_indices @0x83824638 — resets a guarding actor's cached firing/position group
 * indices (word +168, word +196) and associated flag bytes (+164, +166, +352, +170) when its guard state
 * (word +192) is "waiting to move" (3) or "moving" (1, unless byte +352 is set). Shares the +164/+166 color
 * flag bytes with action_guard_modify_color.c. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


void action_guard_flush_position_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.guard.cower && actor->state.action_data.___u0.guard.guard_location_type == _actor_guard_location_firing_position )
    {
        actor->state.action_data.___u0.guard.cower = 0;
        actor->state.action_data.___u0.guard.cower_ticks = 0;
        actor->state.action_data.___u0.guard.cower_from_retreat = 0;
    }

    unsigned char should_reset = 0;
    __int16 guard_state = actor->state.action_data.___u0.guard.guard_location_type;
    /* RAW (irreducible): action_data+196 is beyond the guard arm (guard_state_data, 68 bytes) — it
     * lands in the action_state_data union's unnamed tail; no arm field models it. */
    if ( guard_state == _actor_guard_location_firing_position || (guard_state == _actor_guard_location_current && !((char *)&actor->state.action_data)[196]) )
        should_reset = 1;

    if ( should_reset )
    {
        actor->state.action_data.___u0.guard.guard_location_type = _actor_guard_location_none;
        actor->state.action_data.___u0.guard.___u17.guard_firing_position_index = -1;
        actor->state.action_data.___u0.guard.find_new_guard_position = 1;
    }
}
