/* actor_action_handle_panic_transition @0x837F43C0 — process an actor's accumulated panic level.
 * Bails out (clearing panic level to 0) if the panic level hasn't reached minimum_panic_level yet, or if
 * the actor is panic-immune (+352 flag). If the actor is mid-shoot-action with a positive shot counter,
 * instead of transitioning, just floors the shot counter at the current panic level and clears panic
 * level (deferring the actual panic reaction). Otherwise, at most once every 7+ ticks, either forces a
 * "help me, I'm scared" AI communication event (if enforced_calm keeps the panic level under
 * force_panic_level) or attempts the panic transition itself via actor_action_try_to_panic — either way
 * clearing panic level afterward. Returns whether a panic transition actually occurred.
 *
 * +108 (action) and +168 (shoot-action shot counter, action_data+0xC) reuse the fields already
 * resolved in actor_action_handle_grenade_throwing.c. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern int game_time_get(void);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern uint8_t actor_action_try_to_panic(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_flee_transition);

int actor_action_handle_panic_transition(int actor_index, int16_t minimum_panic_level, uint8_t enforced_calm, int16_t force_panic_level)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 0;

    int16_t *panic_level_ref = &actor->stimuli.panic_type;
    int16_t panic_level = *panic_level_ref;

    if ( panic_level < minimum_panic_level || actor->input.vehicle_passenger )
    {
        *panic_level_ref = 0;
        return result;
    }

    int16_t shot_counter = actor->state.action_data.___u0.flee.panic_type;

    if ( actor->state.action != actor_action_flee || shot_counter <= 0 )
    {
        int *last_attempt_time_ref = &actor->emotions.last_flee_failed_time;

        if ( *last_attempt_time_ref == -1 || *last_attempt_time_ref + 7 < game_time_get() )
        {
            if ( enforced_calm && panic_level < force_panic_level )
            {
                ai_communication_event(_ai_communication_attempted_flee, actor->meta.unit_index, -1, -1, -1, -1, nullptr);
                *panic_level_ref = 0;
                return 0;
            }

            result = actor_action_try_to_panic(actor_index, panic_level, actor->stimuli.panic_prop_index,
                panic_level >= force_panic_level);
        }

        *panic_level_ref = 0;
        return result;
    }

    /* floor the shot counter at the current panic level, deferring the actual panic reaction */
    int16_t *shot_counter_ref = &actor->state.action_data.___u0.flee.panic_type;
    *shot_counter_ref = (shot_counter > panic_level) ? shot_counter : panic_level;

    *panic_level_ref = 0;
    return 0;
}
