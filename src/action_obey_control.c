/* action_obey_control @0x837DF3C8 — per-tick "obey orders" controller: translate the actor's current
 * order/encounter state into the low-level movement/look/aim working state that the actor mover consumes.
 * It first resolves the movement mode:
 *   - if a forced move-to-point order is active (+254), copy the ordered destination (+256..) into the
 *     move target (+1120..) and lock aiming at the target (_primary_priority_locked_aiming /
 *     _direction_specification_target / _idle_look_combat);
 *   - else if the actor has an order path and has reached its destination (+224), switch to the ordered
 *     facing (_primary_priority_exact_facing / _direction_specification_point), copying the order look point and, when no explicit facing was given
 *     (+153 == 0), the ordered facing distance;
 *   - else derive the mode from the combat status (+404 == guarding/idle → stop), the alertness/aggression
 *     gate (+440 alert level >= 5 and enabled), or a plain follow/idle.
 * It then flushes several one-shot order effects: a queued animation impulse (+248), a look-at order
 * (+169 bit 0), and an aiming order (+169 bit 2) whose direction is either the ordered facing or (if that
 * degenerates) global_forward2d.
 *
 * DEVIATIONS: (1) the ordered-aim block at +1088.. — the decompiler merged the 8-byte ordered-direction
 * store at +1092 with the flags byte at +169 into one __int64 (v9) with a HIDWORD pun; disasm shows a
 * plain 8-byte real_vector2d store at +1092 and a *separate* byte +1090 = (actor[169] >> 4) & 1. (2) All
 * +NNN fields are in the actor's opaque behaviour payload / order-working-state and are accessed raw with
 * comments per project convention; only meta.unit_index (actor+24) is a named field. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/data_array.h"
#include "headers/real_vector2d.h"
#include "headers/actor_combat_status.h"
#include "headers/obey_state_data.h"
#include "headers/actor_mode.h"
#include "headers/actor_movement_type.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern uint8_t actor_path_at_destination(int actor_index);
extern uint8_t actor_move_animation_busy(int actor_index);
extern uint8_t actor_move_animation_impulse(int actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment);
extern float normalize2d(real_vector2d *v);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern uint8_t unit_is_busy(int object_index);

void action_obey_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    obey_state_data *obey = &actor->state.action_data.___u0.obey;

    if ( obey->complex_control.shoot_at_target )   /* forced move-to-point order active */
    {
        actor->orders.combat.shoot_at_target = 1;
        actor->orders.combat.override_firing_restrictions = 1;
        /* deviation: prior comments mislabeled these as movement mode/gait/stance — they are look-order fields */
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        actor->orders.look.primary_direction.type = _direction_specification_target;
        actor->orders.look.idle_look_type = _idle_look_combat;
        actor->orders.combat.use_manual_target_point = 1;
        /* DEVIATION: 3x lwz/stw real_point3d copy (0x837DF42C..40) word-punned by the decompiler; plain assignment */
        actor->orders.combat.target_point = obey->complex_control.shoot_target;       /* ordered destination -> move target */
        *&actor->orders.combat.override_burst_duration = obey->complex_control.shoot_burst_length;
        goto flush_order_effects;
    }

    if ( obey->complex_control.destination_facing && actor_path_at_destination(actor_index) )   /* reached ordered path destination */
    {
        int16_t stance = _idle_look_combat;
        uint8_t explicit_facing = actor->state.flying;
        /* deviation: prior comments mislabeled these as movement mode/gait — they are look-order fields */
        actor->orders.look.primary_priority = _primary_priority_exact_facing;
        actor->orders.look.primary_direction.type = _direction_specification_point;
        /* DEVIATION: 3x lwz/stw real_point3d copy (0x837DF488..9C) word-punned across the union arms; plain assignment into the point arm */
        actor->orders.look.primary_direction.___u1.point = obey->complex_control.destination_facing_point;   /* ordered look point */
        if ( !explicit_facing )
            /* decompiler *((float*)actor+74) @837DF4A8 = byte 296 = actor->input.position.head_position.z;
             * the reconstructor's "&action_data+140" reached the same byte (156+140==296) but named the wrong base. */
            actor->orders.look.primary_direction.___u1.vector.n[2] = actor->input.position.head_position.z;

        if ( actor->state.mode < _actor_mode_combat )
        {
            actor->orders.look.idle_look_type = _idle_look_noncombat;
            goto flush_order_effects;
        }
        actor->orders.look.idle_look_type = stance;   /* LABEL_17 */
        goto flush_order_effects;
    }

    int order_state = obey->complex_control.override_movement_type;
    /* override_movement_type domain is actor_movement_type (see actor_move_update.c consumer) */
    if ( order_state == actor_movement_type_panic || order_state == actor_movement_type_asleep )
    {
        actor->orders.look.primary_direction.type = _direction_specification_movement;
        actor->orders.look.idle_look_type = _idle_look_none;
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        goto flush_order_effects;
    }

    if ( actor->state.combat_status >= _actor_combat_status_clear_los && (obey->simple_control.metadata_flags & 1) != 0 )   /* alert & aggression enabled */
    {
        actor->orders.combat.shoot_at_target = 1;
        actor->orders.look.primary_direction.type = _direction_specification_target;
        actor->orders.look.idle_look_type = _idle_look_combat;
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        goto flush_order_effects;
    }

    actor->orders.look.primary_priority = _primary_priority_none;
    if ( obey->allow_looking )
    {
        int16_t stance = _idle_look_combat;
        if ( actor->state.mode < _actor_mode_combat )
            stance = _idle_look_noncombat;
        actor->orders.look.idle_look_type = stance;   /* LABEL_17 */
    }
    else
    {
        actor->orders.look.idle_look_type = _idle_look_none;
    }

flush_order_effects:
    if ( obey->complex_control.grenade_throw_depress_trigger )   /* one-shot: order changed */
    {
        actor->orders.combat.throw_grenade = 1;
        obey->complex_control.grenade_throw_depress_trigger = 0;
    }

    actor->orders.move.stationary_crouch = obey->complex_control.override_crouch;
    actor->orders.move.moving_crouch = obey->complex_control.override_crouch;
    actor->orders.move.override_movement_type = obey->complex_control.override_movement_type;

    if ( obey->complex_control.play_action && !actor_move_animation_busy(actor_index) )   /* queued animation impulse */
    {
        uint16_t animation_impulse = (uint16_t)obey->complex_control.action_animation_impulse;
        if ( animation_impulse != 0xFFFF )
        {
            real_vector2d impulse_direction = *(real_vector2d *)&actor->control.desired_facing_vector.n[0];
            normalize2d(&impulse_direction);
            actor_move_animation_impulse(actor_index, animation_impulse, &impulse_direction);
        }
        uint16_t communication_type = (uint16_t)obey->complex_control.action_communication_type;
        if ( communication_type != 0xFFFF )
            ai_communication_event(communication_type, actor->meta.unit_index, -1, -1, -1, -1, nullptr);
        obey->complex_control.play_action = 0;
    }

    if ( (obey->simple_control.simple_control_flags & (1u << _obey_simple_directmovement_bit)) != 0 )   /* directmovement (look/move) order */
    {
        actor->orders.move.override_movement_direction = 1;
        /* DEVIATION: 3x lwz/stw real_vector3d copy (0x837DF618..2C) word-punned by the decompiler; plain assignment */
        actor->orders.move.override_movement_direction_vector = obey->simple_control.___u5.directmovement.vector;   /* ordered move direction */
        actor->orders.move.override_movement_facing = obey->simple_control.___u5.directmovement.facing;
    }

    if ( (obey->simple_control.simple_control_flags & (1u << _obey_simple_jump_bit)) != 0 )   /* jump order */
    {
        uint8_t aim_set = 0;
        if ( (obey->simple_control.simple_control_flags & (1u << _obey_simple_jump_jumped_bit)) != 0 )
        {
            /* already aiming: keep the aim only while the aim timer (jump.delay_ticks) is still positive.
             * (-x & ~x) >> 31 == (x > 0) on a signed short. */
            int16_t aim_timer = obey->simple_control.___u5.jump.delay_ticks;
            aim_set = ((unsigned int)(-aim_timer & ~aim_timer)) >> 31;
        }
        else if ( obey->simple_control.___u5.jump.delay_ticks || actor->input.in_midair || unit_is_busy(actor->meta.unit_index) )
        {
            aim_set = 1;
        }
        else
        {
            /* begin a new aim: direction = ordered facing (or global_forward2d if degenerate) */
            real_vector2d ordered_facing = *(real_vector2d *)&actor->input.facing_vector.n[0];
            real_vector2d aim_direction;
            if ( normalize2d(&ordered_facing) == 0.0f )
                aim_direction = *global_forward2d;
            else
                aim_direction = ordered_facing;

            actor->orders.move.jump = 1;
            actor->orders.move.jump_leap = obey->simple_control.___u5.jump.target_vertical_vel < (double)(obey->simple_control.___u5.jump.target_horizontal_vel * 0.69999999f);
            *(real_vector2d *)&actor->orders.move.jump_alignment_vector.__s1.i = aim_direction;
            actor->orders.move.jump_targeted = (obey->simple_control.simple_control_flags >> _obey_simple_jump_targeted_bit) & 1;
            actor->orders.move.jump_target_horizontal_vel = obey->simple_control.___u5.jump.target_horizontal_vel;
            actor->orders.move.jump_target_vertical_vel = obey->simple_control.___u5.jump.target_vertical_vel;

            char flags = obey->simple_control.simple_control_flags;
            obey->simple_control.simple_control_flags = flags | (1u << _obey_simple_jump_jumped_bit);
            if ( (flags & 0x10) == 0 )
                obey->simple_control.___u5.jump.delay_ticks = 15;
        }

        if ( aim_set )
        {
            /* DEVIATION: 3x lwz/stw real_vector3d copy (0x837DF750..6C) word-punned through int scratch locals; plain assignment */
            actor->orders.move.override_movement_direction = 1;
            actor->orders.move.override_movement_facing = 0;
            actor->orders.move.override_movement_direction_vector = actor->input.facing_vector;
        }
    }
}