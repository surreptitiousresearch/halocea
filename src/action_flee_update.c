/* action_flee_update @0x83825CE0 — per-tick update for the flee behavior (counterpart of
 * action_flee_begin.c); all working state resolved to the DB-named flee_state_data arm. Increments
 * flee.flee_timer every call; decrements flee.forced_flee_ticks while positive; once
 * flee.flee_stationary_ticks reaches zero, if the actor has a valid unit (meta.unit_index != -1) and
 * flee.panic_type falls in [9, 12], kicks off a blind sprint toward it (mirrors action_flee_begin's own
 * [9,12] gate). Finally, if panic_type is still positive, refreshes emotions.last_flee_failed_time to
 * now + 750 ticks. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/flee_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void unit_start_running_blindly(int unit_index);

void action_flee_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    flee_state_data *flee_state = &actor->state.action_data.___u0.flee;

    int16_t recently_started_cooldown = flee_state->forced_flee_ticks;
    ++flee_state->flee_timer;   /* flee-duration tick counter */
    if ( recently_started_cooldown > 0 )
        flee_state->forced_flee_ticks = recently_started_cooldown - 1;

    int16_t blind_sprint_countdown = flee_state->flee_stationary_ticks;
    if ( blind_sprint_countdown > 0 )
    {
        int16_t next_countdown = blind_sprint_countdown - 1;
        flee_state->flee_stationary_ticks = next_countdown;
        if ( !next_countdown )
        {
            int flee_target = actor->meta.unit_index;
            if ( flee_target != -1 )
            {
                int16_t flee_mode_value = flee_state->panic_type;
                if ( flee_mode_value >= _actor_panic_grenade_attached_to_us && flee_mode_value <= _actor_panic_burning_to_death )
                    unit_start_running_blindly(flee_target);
            }
        }
    }

    if ( flee_state->panic_type > _actor_panic_none )
        actor->emotions.last_flee_failed_time = game_time_get() + 750;
}
