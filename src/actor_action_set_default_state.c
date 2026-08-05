/* actor_action_set_default_state @0x83? — drive an actor back to a default behaviour when it has nothing more
 * pressing to do. An explicit override_state may be supplied; otherwise the actor's pending default state
 * (actor+96) is consumed, or its fallback (actor+98) used, and the routine is throttled to once every 45 game
 * ticks. The resolved state selects an action to enter — move/alert (0,2..7), stop (1), guard a current or
 * found position (8,9), recover from lost contact (10) or flee (11) — each set up through the matching
 * action_*_setup helper and committed with actor_action_change. If nothing changed and the actor is idle, a
 * neutral alert action is entered. Returns 1 if the actor's action changed.
 *
 * Fields resolved to named actor_datum members: state.initial_state/default_state (+96/+98),
 * state.last_default_state_time (+100), state.action (+108), and the action_data union (+156). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_state_data.h"
#include "headers/action_specification.h"
#include "headers/action_class.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_default_state.h"
#include "headers/actor_mode.h"
#include "headers/actor_guard_location_type.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"

typedef struct alert_state_data alert_state_data;
typedef struct guard_state_data guard_state_data;
typedef struct flee_state_data flee_state_data;

extern int16_t global_state_move_position_orders[];
extern action_specification global_action_functions[];

extern int game_time_get(void);
extern uint8_t action_alert_setup(uint16_t actor_index, int16_t move_position_order, int16_t initial_move_position_index, alert_state_data *state_data);
extern uint8_t action_guard_setup_current_position(uint16_t actor_index, guard_state_data *state_data);
extern uint8_t action_guard_setup_find_position(uint16_t actor_index, int16_t delay_timer, guard_state_data *state_data);
extern uint8_t action_flee_setup(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_state_entry, uint8_t flee_from_last_visible_location, uint8_t allow_occluded_points, flee_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);
extern uint8_t actor_action_handle_lost_contact(int actor_index);

uint8_t actor_action_set_default_state(int actor_index, int16_t override_state)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int game_time = game_time_get();
    int state = override_state;
    uint8_t changed = 0;

    if ((int16_t)state == -1)
    {
        int last_time = actor->state.last_default_state_time; /* +100 */
        if (last_time != -1 && last_time + 45 >= game_time)
            return 0;
    }
    actor->state.last_default_state_time = game_time;

    if ((int16_t)state == -1)
    {
        uint16_t pending = (uint16_t)actor->state.initial_state; /* +96 */
        if (pending == 0xFFFF)
            state = (actor->state.default_state == -1) ? 0 : (uint16_t)actor->state.default_state; /* +98 */
        else
        {
            actor->state.initial_state = -1;
            state = pending;
        }
    }

    action_state_data action_data;

    switch ((int16_t)state)
    {
    case actor_default_state_none:
    case actor_default_state_alert:
    case actor_default_state_moving_repeat_position:
    case actor_default_state_moving_loop:
    case actor_default_state_moving_loop_back_and_forth:
    case actor_default_state_moving_loop_randomly:
    case actor_default_state_moving_randomly:
        /* recovered: *(u16*)&action_data -> alert.move_position_order (action 2 == alert) */
        if (((uint16_t)actor->state.action != actor_action_alert /* current action state +108 */
              || (uint16_t)actor->state.action_data.___u0.alert.move_position_order != (uint16_t)global_state_move_position_orders[(int16_t)state])
            && action_alert_setup(actor_index, global_state_move_position_orders[(int16_t)state], -1,
                                  &action_data.___u0.alert))
        {
            actor_action_change(actor_index, actor_action_alert, &action_data);
            changed = 1;
        }
        break;
    case actor_default_state_asleep:
        if ((uint16_t)actor->state.mode != _actor_mode_asleep)
        {
            actor->state.mode = _actor_mode_asleep;
            actor_action_change(actor_index, actor_action_sleep, nullptr);
            changed = 1;
        }
        break;
    case actor_default_state_guarding:
        /* recovered: *(u16*)((char*)&action_data + 36) -> guard.guard_location_type (action 6 == guard) */
        if (((uint16_t)actor->state.action != actor_action_guard || (uint16_t)actor->state.action_data.___u0.guard.guard_location_type != _actor_guard_location_current)
            && action_guard_setup_current_position(actor_index, &action_data.___u0.guard))
        {
            actor_action_change(actor_index, actor_action_guard, &action_data);
            changed = 1;
        }
        break;
    case actor_default_state_guarding_at_guard_point:
        if ((uint16_t)actor->state.action == actor_action_guard)
        {
            /* recovered: +36 -> guard.guard_location_type, [14] -> guard.find_new_guard_position (action 6 == guard) */
            if ((uint16_t)actor->state.action_data.___u0.guard.guard_location_type != _actor_guard_location_firing_position)
                actor->state.action_data.___u0.guard.find_new_guard_position = 1;
        }
        else if (action_guard_setup_find_position(actor_index, 0, &action_data.___u0.guard))
        {
            actor_action_change(actor_index, actor_action_guard, &action_data);
            changed = 1;
        }
        break;
    case actor_default_state_searching:
        if (global_action_functions[actor->state.action].action_class != _action_class_pursuit)
        {
            actor->state.artificial_combat_status = _actor_combat_status_investigate;
            actor->state.mode = _actor_mode_combat;
            actor->state.combat_status = _actor_combat_status_investigate;
            if (!actor_action_handle_lost_contact(actor_index)
                && action_guard_setup_current_position(actor_index, &action_data.___u0.guard))
            {
                actor_action_change(actor_index, actor_action_guard, &action_data);
                changed = 1;
            }
        }
        break;
    case actor_default_state_fleeing:
        if ((uint16_t)actor->state.action != actor_action_flee)
        {
            if (action_flee_setup(actor_index, _actor_panic_scripted, -1, 1, 0, 0, &action_data.___u0.flee))
            {
                actor_action_change(actor_index, actor_action_flee, &action_data);
                changed = 1;
            }
            else if ((uint16_t)actor->state.action != actor_action_guard
                     && action_guard_setup_current_position(actor_index, &action_data.___u0.guard))
            {
                actor_action_change(actor_index, actor_action_guard, &action_data);
                changed = 1;
            }
        }
        break;
    default:
        break;
    }

    if (!changed && (uint16_t)actor->state.action == actor_action_none
        && action_alert_setup(actor_index, 0, -1, &action_data.___u0.alert))
    {
        actor_action_change(actor_index, actor_action_alert, &action_data);
        return 1;
    }
    return changed;
}
