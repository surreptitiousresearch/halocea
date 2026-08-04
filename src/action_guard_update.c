/* action_guard_update @0x83823FE0 — per-tick update for an actor's "guard" action. Runs three independent
 * timers on the actor's guard working-state: (1) a post-combat "all clear" countdown that, on expiry, makes the
 * actor speak a post-combat line and clears its pursued target; (2) a short secondary countdown that clears the
 * pursuit target on expiry; (3) the dormancy/deactivation timer — either a flag-driven "value still positive"
 * gate or a plain countdown — that, on expiry, wakes the actor (undormant), fires an AI "lost him" communication
 * event when it has a valid target, discards any assigned firing position, and re-enters guard or idle.
 *
 * DEVIATION: actor typed actor_datum*; the behaviour payload is accessed via the typed guard arm
 * (actor->state.action_data.___u0.guard). The `-x & ~x` >> 31 at 0x83824108 is a branchless (x > 0) test on the
 * sign-extended deactivation timer (disasm-verified). The goto-based dispatch is reflowed into conditionals. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_postcombat_type.h"
#include "headers/actor_combat_status.h"
#include "headers/guard_state_data.h"
#include "headers/ai_communication_type.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern int action_guard_speak_post_combat(uint16_t actor_index);
extern void actor_set_dormant(uint16_t actor_index, uint8_t dormant);
extern int actor_target_unit_index(uint16_t actor_index);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);

void action_guard_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    guard_state_data *guard = &actor->state.action_data.___u0.guard;

    if ( !actor->meta.dormant && actor->control.path.at_destination )
    {
        __int16 timer = guard->wait_ticks;
        if ( timer > 0 )
        {
            guard->wait_ticks = timer - 1;
            if ( timer == 1 && !actor->input.vehicle_passenger && !actor->meta.swarm )
            {
                if ( guard->post_combat )
                {
                    action_guard_speak_post_combat(actor_index);
                    actor->external_orders.postcombat_type = actor_postcombat_none;
                    actor->external_orders.postcombat_prop_index = -1;
                    guard->post_combat = 0;
                    guard->post_combat_shooting = 0;
                    guard->guard_look_prop_index = -1;
                }
                guard->find_new_guard_position = 1;
            }
        }
    }

    int secondary_timer = guard->look_ticks;
    if ( secondary_timer > 0 && (!guard->guard_look_until_reached_point || actor->control.path.at_destination) )
    {
        __int16 remaining = secondary_timer - 1;
        guard->look_ticks = remaining;
        if ( !remaining )
            guard->guard_look_prop_index = -1;
    }

    if ( guard->path_begun && actor->control.path.at_destination )
    {
        bool expired = 0;
        if ( guard->cower_from_retreat )
        {
            /* DEVIATION: `-x & ~x` >> 31 is a branchless (x > 0) test on the sign-extended timer (disasm 0x83824108). */
            short deactivation_timer = actor->emotions.unopposable_retreat_timer;
            guard->cower_from_retreat = deactivation_timer > 0;
            expired = deactivation_timer <= 0;
        }
        else
        {
            int dormancy_timer = guard->cower_ticks;
            if ( dormancy_timer > 0 )
            {
                __int16 remaining = (__int16)(dormancy_timer - 1);
                guard->cower_ticks = remaining;
                expired = remaining == 0;
            }
        }

        if ( expired )
        {
            actor_set_dormant(actor_index, 0);
            guard->cower = 0;
            guard->cower_panicked = 0;
            guard->cower_from_retreat = 0;
            guard->cower_ticks = 0;
            if ( actor->state.combat_status >= _actor_combat_status_investigate && actor->meta.unit_index != -1 )
            {
                int target_unit = actor_target_unit_index(actor_index);
                ai_communication_event(_ai_communication_hiding_finished, actor->meta.unit_index, target_unit, -1, -1, -1, nullptr);
            }
            if ( guard->guard_location_type == _actor_guard_location_firing_position )
            {
                actor_discard_firing_position(actor_index, guard->___u17.guard_firing_position_index, 0);
                guard->___u17.guard_firing_position_index = -1;
            }
            unsigned char guarding = actor->input.vehicle_passenger;
            actor->firing_positions.current_position_index = -1;
            if ( guarding )
            {
                guard->guard_location_type = _actor_guard_location_current;
            }
            else
            {
                guard->guard_location_type = _actor_guard_location_none;
                guard->find_new_guard_position = 1;
            }
        }
    }
}
