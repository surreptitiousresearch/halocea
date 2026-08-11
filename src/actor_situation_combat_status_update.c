/* actor_situation_combat_status_update @0x837D73D0 — recompute an actor's combat status for the frame.
 * First it folds any externally-injected pending status (stimuli.suspicion_combat_status) into the
 * running status (state.suspicion_combat_status) and its timestamp (state.suspicion_timer /
 * stimuli.suspicion_timer), keeping the strongest. Then it combines the current alertness
 * (state.artificial_combat_status), the status-floor looked up from the actor's combat-status type
 * (target.target_type) via global_combat_status_table, and the running status into the final combat
 * status (state.combat_status), clearing the running status once consumed.
 * Finally it maintains the dwell counters (state.combat_mode_timer / state.in_combat_timer /
 * state.certain_combat_timer / state.uncertain_combat_timer) and sets the in-combat flag
 * (state.had_visible_enemy) at status >= 7. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"

extern const int16_t   global_combat_status_table[12];

void actor_situation_combat_status_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int16_t pending = actor->stimuli.suspicion_combat_status;
    if (pending > 0)
    {
        int16_t running = actor->state.suspicion_combat_status;
        if (running < pending)
        {
            actor->state.suspicion_combat_status = pending;
            actor->state.suspicion_timer = actor->stimuli.suspicion_timer;
        }
        else if (running == pending)
        {
            int timestamp = actor->stimuli.suspicion_timer;
            if (actor->state.suspicion_timer > timestamp)
                timestamp = actor->state.suspicion_timer;
            actor->state.suspicion_timer = timestamp;
        }
        actor->stimuli.suspicion_combat_status = _actor_combat_status_none;
    }

    int16_t alertness = actor->state.artificial_combat_status;
    int16_t status_floor = global_combat_status_table[actor->target.target_type];
    int16_t peak = alertness;
    if (alertness <= status_floor)
        peak = status_floor;

    int16_t running = actor->state.suspicion_combat_status;
    int16_t status = peak;
    if (running > peak)
        status = running;

    actor->state.combat_status = status;
    if (status > running)
        actor->state.suspicion_combat_status = _actor_combat_status_none;

    if (actor->state.mode >= _actor_mode_combat)
        ++actor->state.combat_mode_timer;
    else
        actor->state.combat_mode_timer = 0;

    if (status)
        ++actor->state.in_combat_timer;
    else
        actor->state.in_combat_timer = 0;

    if (status < _actor_combat_status_certain)
    {
        int ticks_out_of_combat = actor->state.uncertain_combat_timer;
        actor->state.certain_combat_timer = 0;
        if (ticks_out_of_combat != -1)
            actor->state.uncertain_combat_timer = ticks_out_of_combat + 1;
    }
    else
    {
        int ticks_in_combat = actor->state.certain_combat_timer;
        actor->state.uncertain_combat_timer = 0;
        actor->state.certain_combat_timer = ticks_in_combat + 1;
    }

    if (status >= _actor_combat_status_visible)
        actor->state.had_visible_enemy = 1;
}
