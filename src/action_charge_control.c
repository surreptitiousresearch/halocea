/* action_charge_control @0x837F92E0 — per-tick update of an actor's melee "charge" action working state. It
 * derives the actor's charge/berserk flags from its charge goal, combat status, current path and the
 * actor definition's flags (_actor_definition_defensive_crouch_while_charging_bit /
 * _actor_definition_berserk_use_panic_movement_bit), primes throttle/animation request fields, snapshots
 * the charge target on entry, and finally records a charge "phase" byte.
 * The `(cntlzw(x) & 0x20) != 0` idiom is `x == 0`; rendered as such. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_combat_status.h"
#include "headers/charge_state_data.h"
#include "headers/charge_goal.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_path_has_path(int actor_index);
extern int game_time_get(void);

void action_charge_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    charge_state_data *charge_state = &actor->state.action_data.___u0.charge;
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    actor->orders.look.primary_direction.type = _direction_specification_target;
    actor->orders.look.idle_look_type = _idle_look_combat;

    short mode = charge_state->goal;
    if ( (mode == charge_goal_melee || mode == charge_goal_melee_leaping) && charge_state->alignment_incorrect && !actor->control.moving && !actor_path_has_path(actor_index) )
    {
        actor->orders.look.primary_priority = _primary_priority_exact_facing;
    }
    else
    {
        short charge_look_priority = _primary_priority_locked_aiming;
        if ( actor->state.combat_status < _actor_combat_status_clear_los || (uint16_t)charge_state->goal == charge_goal_stalking )
            charge_look_priority = _primary_priority_aiming;
        actor->orders.look.primary_priority = charge_look_priority;
    }

    if ( (uint16_t)charge_state->goal == charge_goal_stalking )
    {
        char at_rest = charge_state->stalking_catch_target == 0;
        actor->orders.move.stationary_crouch = at_rest;
        actor->orders.move.moving_crouch = at_rest;
    }
    else if ( actor->orders.move.panicked || (actor_def->flags & (1u << _actor_definition_defensive_crouch_while_charging_bit)) == 0 )
    {
        actor->orders.move.moving_crouch = 0;
        actor->orders.move.stationary_crouch = 0;
    }
    else
    {
        char charging = actor->emotions.defensive_crouch;
        actor->orders.move.stationary_crouch = charging;
        actor->orders.move.moving_crouch = charging;
    }

    if ( charge_state->leap_pending )
    {
        actor->orders.move.jump = 1;
        char target_close = charge_state->leap_vertical_velocity
                < (charge_state->leap_horizontal_velocity * 0.69999999f);
        actor->orders.move.jump_targeted = 1;
        actor->orders.move.jump_leap = target_close;
        /* DEVIATION: decompiler word-punned the two-word real_vector2d copy (lwz/stw pairs 0x14/0x18 -> 0x444/0x448); plain struct assignment */
        actor->orders.move.jump_alignment_vector = charge_state->leap_alignment_vector;
        actor->orders.move.jump_target_horizontal_vel = charge_state->leap_horizontal_velocity;
        actor->orders.move.jump_target_vertical_vel = charge_state->leap_vertical_velocity;
        charge_state->launched_leap = 1;
        charge_state->leap_pending = 0;
        charge_state->leap_start_time = game_time_get();
        charge_state->leap_failure_timer = 0;
    }

    if ( (actor_def->flags & (1u << _actor_definition_berserk_use_panic_movement_bit)) != 0 )
    {
        short berserk_mode = charge_state->goal;
        if ( actor->emotions.berserk || berserk_mode == charge_goal_melee || berserk_mode == charge_goal_melee_leaping )
        {
            char berserking = 0;
            if ( charge_state->advancing && !actor->orders.move.moving_crouch )
                berserking = 1;
            actor->orders.move.panicked = berserking;
        }
    }

    actor->orders.move.move_face_exactly = 1;
    actor->orders.move.emerge_from_cover = 0;
    actor->orders.move.dive_into_cover = 0;
    /* DEVIATION: decompiler mangled the addic/subfe carry idiom (goal-1 != 0) into byte-pun
     * arithmetic; disasm computes CA of (goal-1)+(-1), i.e. goal != charge_goal_stalking (1). */
    actor->orders.combat.shoot_at_target = charge_state->goal != charge_goal_stalking;
}
