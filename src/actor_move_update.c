/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* actor_move_update @ 0x837CA9B8 — produce the actor's movement output for the frame: the desired throttle
 * vector (output.throttle, actor+1760), desired facing (control.desired_facing_vector actor+1444 /
 * control.moving_facing_direction actor+1290), crouch, and jump/leap impulses. The body selects a
 * movement "mode" from the actor/unit/vehicle state (idle, vehicle seat role at actor+350, stuck vehicle,
 * blocked, guarding, etc.), each of which sets the committed-movement flag (actor+1284) and the steering
 * inputs, then — when committed — calls actor_move_calculate_movement to convert the desired movement/facing
 * into throttle + facing-direction outputs. Finally it applies crouch, and (for guard/charge actions) the
 * jump/leap behaviors.
 *
 * Most actor/unit/vehicle reads are by raw offset, matching the decompiler. The character definition is
 * resolved through TAG_INSTANCE. NOTE: the actor_move_calculate_movement call was re-derived in full from the
 * call-site disassembly at 0x837CB138-0x837CB18C. On this ABI every float argument DOES shadow (skip) a GPR
 * slot, so the GPR args are r3/r4/r5/r6/(f1)/r8=allow_all_moving_turns/(f2)/(f3), floats ride f1-f6, and all
 * six pointer args 12-17 go on the stack (six stw's to consecutive 8-byte slots, ascending = arg order):
 * arg12 &control.moving_towards_vector (actor+0x518), arg13 &control.desired_facing_vector (actor+0x5A4, r25),
 * arg14 &control.moving_facing_direction (actor+0x50A), arg15 &output.throttle (actor+0x6E0),
 * arg16 &control.movement_thwarted (actor+0x507), arg17 &control.movement_complete (actor+0x506, read back
 * via r30 immediately after the call). A prior reconstruction used a no-shadow ABI model here, which
 * scrambled args 12-15 and mis-passed seat_role as arg6; r21 (arg6) is the boolean set alongside
 * move_in_3d=1 in the directional-flying branch (0x837CAE7C-80). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_type.h" /* _actor_type_* enum */
#include "headers/actor_definition_flags.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/vehicle_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/vehicle_datum.h"

#include <math.h>
#include "headers/actor_movement_type.h"
#include "headers/destination_type.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/actor_type.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"



#include "headers/ai_information_data.h"
extern void actor_move_vector_avoidance(int actor_index, const real_vector3d *movement_direction, real_vector3d *avoidance_rotation, float *emergency_amount);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern uint8_t vehicle_stuck(int vehicle_index, real_vector3d *approximate_direction);
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern void actor_move_calculate_movement(int actor_index, uint8_t move_in_3d, int16_t override_facing, uint8_t free_movement, float free_movement_dist_sq, uint8_t allow_all_moving_turns, float steering_maximum_angle, float oversteer_minimum_angle, float oversteer_maximum_angle, float rotation_emergency_amount, float maximum_throttle, const real_vector3d *desired_movement_vector, real_vector3d *desired_facing_vector, int16_t *desired_facing_direction, real_vector3d *desired_throttle, uint8_t *movement_thwarted, uint8_t *movement_complete);
extern void actor_clear_discarded_firing_positions(uint16_t actor_index, uint8_t clear_temporary_only);
extern void actor_unit_control_crouch(uint16_t actor_index, uint8_t crouch);
extern uint8_t actor_move_animation_busy(uint16_t actor_index);
extern uint8_t actor_move_animation_impulse(uint16_t actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment);
extern void actor_unit_control_jump(uint16_t actor_index);
extern uint8_t unit_leap_begin(int unit_index, const real_vector2d *alignment_vector);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void actor_move_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    char *character = TAG_GET(char, actor->meta.definition_index);
    actor_definition *character_def = (actor_definition *)character; /* recovered: +148/+152 -> moving.stationary_moving_distance / .free_flying_sidestep_distance */

    float steering_maximum_angle = 0.0f, oversteer_minimum_angle = 0.0f, oversteer_maximum_angle = 0.0f;
    float rotation_emergency_amount = 0.0f, maximum_throttle = 1.0f, free_movement_dist_sq = 0.0f;
    unsigned __int8 move_in_3d = 0, free_movement = 0;
    uint8_t allow_all_moving_turns = 0;
    char allow_jump = 0, is_busy_anim = 0, force_throttle_facing = 0;
    __int16 override_facing;

    float *throttle = &actor->control.desired_facing_vector.n[0];
    actor->control.face_exactly = 0;
    actor->control.free_facing_vector = 1;
    actor->control.optional_facing_vector = 1;
    actor->control.desired_facing_vector = actor->input.facing_vector;  /* throttle = facing vector (default) */

    if ( actor->orders.move.override_movement_direction )      /* scripted/forced movement target */
    {
        actor->control.moving = 1;
        actor->control.free_facing_vector = 0;
        actor->control.moving_towards_vector = actor->orders.move.override_movement_direction_vector;  /* dest delta = forced vector */
        actor->control.vector_avoidance_rotation = *global_zero_vector3d;
        actor->control.vector_avoidance_rotation_emergency = 0.0f;
        actor->control.vector_avoidance_rotation_emergency_instantaneous = 0.0f;
    }
    else if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
    {
        const real_vector3d *desired;
        float scratch[4];
        if ( actor->control.moving )
        {
            desired = &actor->control.moving_towards_vector;
        }
        else
        {
            desired = (const real_vector3d *)scratch;
            scratch[0] = actor->input.facing_vector.n[0] * 3.0f;
            scratch[1] = actor->input.facing_vector.n[1] * 3.0f;
            scratch[2] = actor->input.facing_vector.n[2] * 3.0f;
        }

        real_vector3d avoidance_rotation;
        real_vector3d emergency;
        actor_move_vector_avoidance(actor_index, desired, &avoidance_rotation, emergency.n);

        /* low-pass the avoidance rotation accumulator (actor+1500) toward the new value */
        float rotation_sq = avoidance_rotation.n[2] * avoidance_rotation.n[2]
                          + avoidance_rotation.n[0] * avoidance_rotation.n[0]
                          + avoidance_rotation.n[1] * avoidance_rotation.n[1];
        float prev_sq = actor->control.vector_avoidance_rotation.n[2] * actor->control.vector_avoidance_rotation.n[2]
                      + actor->control.vector_avoidance_rotation.n[0] * actor->control.vector_avoidance_rotation.n[0]
                      + actor->control.vector_avoidance_rotation.n[1] * actor->control.vector_avoidance_rotation.n[1];
        float blend = (rotation_sq <= (double)prev_sq) ? 0.050000001f : 0.30000001f;

        *throttle = actor->control.vector_avoidance_rotation.n[0] * (1.0f - blend);   /* note: actor+1500 reused below */
        actor->control.vector_avoidance_rotation.n[1] = (1.0f - blend) * actor->control.vector_avoidance_rotation.n[1];
        actor->control.vector_avoidance_rotation.n[2] = actor->control.vector_avoidance_rotation.n[2] * (1.0f - blend);
        *throttle = avoidance_rotation.n[0] * blend + *throttle;
        actor->control.vector_avoidance_rotation.n[1] = avoidance_rotation.n[1] * blend + actor->control.vector_avoidance_rotation.n[1];
        actor->control.vector_avoidance_rotation.n[2] = avoidance_rotation.n[2] * blend + actor->control.vector_avoidance_rotation.n[2];

        if ( *throttle * *throttle
           + actor->control.vector_avoidance_rotation.n[1] * actor->control.vector_avoidance_rotation.n[1]
           + actor->control.vector_avoidance_rotation.n[2] * actor->control.vector_avoidance_rotation.n[2] < 0.000099999997f )
        {
            *throttle = global_zero_vector3d->n[0];
            actor->control.vector_avoidance_rotation.n[1] = global_zero_vector3d->n[1];
            actor->control.vector_avoidance_rotation.n[2] = global_zero_vector3d->n[2];
        }

        float emergency_blend = blend * emergency.n[0];
        float prev_emergency = actor->control.vector_avoidance_rotation_emergency;
        actor->control.vector_avoidance_rotation_emergency_instantaneous = emergency.n[0];
        actor->control.vector_avoidance_rotation_emergency = prev_emergency * (1.0f - blend) + emergency_blend;
        if ( actor->control.vector_avoidance_rotation_emergency < 0.001f )
            actor->control.vector_avoidance_rotation_emergency = 0.0f;

        if ( actor->control.moving )
        {
            real_vector3d rotation;
            rotation.n[0] = *throttle;
            rotation.n[1] = actor->control.vector_avoidance_rotation.n[1];
            rotation.n[2] = actor->control.vector_avoidance_rotation.n[2];
            float mag_sq = rotation.n[2] * rotation.n[2]
                         + rotation.n[0] * rotation.n[0]
                         + rotation.n[1] * rotation.n[1];
            if ( mag_sq > 0.000099999997f )
            {
                float angle = sqrtf(mag_sq);   /* decompiler __fsqrts (single-precision sqrt) */
                rotation.n[0] = (1.0f / angle) * rotation.n[0];
                rotation.n[1] = rotation.n[1] * (1.0f / angle);
                rotation.n[2] = rotation.n[2] * (1.0f / angle);
                rotate_vector_about_axis(&actor->control.moving_towards_vector, &rotation,
                                         (float)sin(angle), (float)cos(angle));
            }
            rotation_emergency_amount = actor->control.vector_avoidance_rotation_emergency;
        }
    }

    /* movement-animation index (actor+1756) from steering/action state */
    int movement_animation = actor->orders.move.override_movement_type;
    if ( movement_animation == 0xFFFF )
    {
        movement_animation = actor_movement_type_combat;
        if ( actor->orders.move.flaming )
            movement_animation = actor_movement_type_flaming;
        else if ( actor->orders.move.panicked )
            movement_animation = actor_movement_type_panic;
        else
        {
            /* actor->state.mode is actor_mode (DB $7D6E27E2...); maps mode -> movement animation type */
            switch ( actor->state.mode )
            {
                case _actor_mode_asleep: movement_animation = actor_movement_type_asleep; break;
                case _actor_mode_alert:  movement_animation = actor_movement_type_alert; break;
                case _actor_mode_combat: movement_animation = actor_movement_type_combat; break;
            }
        }
    }
    actor->output.movement_type = movement_animation;

    override_facing = actor->orders.move.override_movement_facing;
    unsigned __int8 crouch;
    if ( actor->control.path.path.valid                                /* path active — opaque path block offset */
      && actor->control.path.destination_distance >= (double)character_def->moving.stationary_moving_distance )
        crouch = actor->orders.move.moving_crouch;
    else
        crouch = actor->orders.move.stationary_crouch;

    __int16 seat_role = actor->input.vehicle_driver_type;
    if ( seat_role > 0 )                                       /* in a vehicle seat */
    {
        int   vehicle_index = actor->input.vehicle_index;
        int   vehicle_object = ((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *(int *)vehicle_object);
        steering_maximum_angle = vehicle_def->vehicle.ai_steering_max_angle;
        if ( vehicle_def->vehicle.ai_steering_max_throttle > 0.0f )
            maximum_throttle = vehicle_def->vehicle.ai_steering_max_throttle;
        int role = (unsigned __int16)actor->input.vehicle_driver_type;
        oversteer_minimum_angle = vehicle_def->vehicle.ai_oversteer_angle_lower_bound;
        oversteer_maximum_angle = vehicle_def->vehicle.ai_oversteer_angle_upper_bound;

        vehicle_datum *vehicle = (vehicle_datum *)vehicle_object; /* recovered: raw +1232/+1260/+128/+132/+136 -> named vehicle_datum fields */
        if ( role == _actor_vehicle_driver_hovering_ground )
        {
            if ( vehicle->vehicle.airborne_ticks )
            {
                allow_jump = 1;
                actor->control.moving = 0;
                actor->control.free_facing_vector = 1;
                crouch = 0;
                goto resolve_movement;
            }
            if ( vehicle->vehicle.hover < 0.69999999f
              && vehicle->object.up.n[2] < 0.80000001f )
            {
                allow_jump = 1;
                real_vector3d eject;
                eject.n[0] = vehicle->object.up.n[0];
                eject.n[2] = 0.0f;
                eject.n[1] = vehicle->object.up.n[1];
                if ( normalize3d(&eject) <= 0.0f )
                {
                    actor->control.moving = 0;
                    crouch = 0;
                }
                else
                {
                    actor->control.moving = 1;
                    actor->control.moving_towards_vector.n[0] = eject.n[0] * 3.0f;
                    actor->control.moving_towards_vector.n[1] = eject.n[1] * 3.0f;
                    crouch = 0;
                    actor->control.moving_towards_vector.n[2] = eject.n[2] * 3.0f;
                }
                goto resolve_movement;
            }
        }
        else if ( role != _actor_vehicle_driver_nondirectional_ground )
        {
            if ( role == _actor_vehicle_driver_directional_flying )
            {
                real_vector3d stuck_direction;
                if ( vehicle_stuck(vehicle_index, &stuck_direction) )
                {
                    actor->control.free_facing_vector = 0;
                    crouch = 0;
                    actor->control.optional_facing_vector = 0;
                    actor->control.moving = 0;
                    *throttle = -stuck_direction.n[0];
                    actor->control.desired_facing_vector.n[1] = -stuck_direction.n[1];
                    actor->control.desired_facing_vector.n[2] = -stuck_direction.n[2];
                }
                else
                {
                    override_facing = 0;
                    move_in_3d = 1;
                    allow_all_moving_turns = 1;
                    crouch = 0;
                }
            }
            else                                               /* grounded driver */
            {
                int action_class = (unsigned __int16)actor->meta.type;
                actor->control.moving = 0;
                actor->control.moving_facing_direction = 0;
                actor->control.free_facing_vector = (action_class == _actor_type_mounted_weapon || actor->input.vehicle_gunner) ? 1 : 0;
                crouch = 0;
            }
            goto resolve_movement;
        }

        /* role 1 (driver) and role 3 (gunner) fall through to commit movement */
        free_movement = 1;
        free_movement_dist_sq = vehicle_def->vehicle.ai_sideslip_distance * vehicle_def->vehicle.ai_sideslip_distance;
        crouch = 0;
        goto resolve_movement;
    }

    if ( actor->input.vehicle_passenger )                      /* seat role <= 1 marker */
    {
        int action_class = (unsigned __int16)actor->meta.type;
        actor->control.moving = 0;
        actor->control.moving_facing_direction = 0;
        actor->control.optional_facing_vector = 0;
        crouch = 0;
        actor->control.free_facing_vector = (action_class == _actor_type_mounted_weapon || actor->input.vehicle_gunner) ? 1 : 0;
        goto resolve_movement;
    }

    if ( (unsigned __int16)actor->orders.move.animation.impulse != 0xFFFF )   /* mounted / fixed */
    {
        actor->control.moving = 0;
        crouch = 0;
        actor->control.free_facing_vector = 0;
        actor->control.optional_facing_vector = 0;
        goto resolve_movement;
    }

    if ( actor->output.movement_type == actor_movement_type_asleep )                    /* movement animation == "stop" */
    {
        actor->control.moving = 0;
        force_throttle_facing = 1;
        actor->control.free_facing_vector = 0;
        crouch = 0;
        actor->control.optional_facing_vector = 0;
        goto resolve_movement;
    }

    if ( actor->input.in_midair && !actor->state.flying )      /* airborne, non-flying */
    {
        actor->control.moving = 0;
        crouch = 0;
        actor->control.free_facing_vector = 1;
        goto resolve_movement;
    }

    if ( actor->control.grenade_trying_to_throw )              /* explicit look-at-point steering */
    {
        actor->control.moving = 0;
        crouch = 0;
        real_vector3d to_point;
        to_point.n[0] = actor->control.grenade_current_target.x - actor->input.position.body_position.x;
        to_point.n[1] = actor->control.grenade_current_target.y - actor->input.position.body_position.y;
        to_point.n[2] = actor->control.grenade_current_target.z - actor->input.position.body_position.z;
        if ( normalize3d(&to_point) != 0.0f )
        {
            actor->control.free_facing_vector = 0;
            actor->control.optional_facing_vector = 0;
            *throttle = to_point.n[0];
            actor->control.desired_facing_vector.n[1] = to_point.n[1];
            actor->control.desired_facing_vector.n[2] = to_point.n[2];
            actor->control.face_exactly = 1;
            goto resolve_movement;
        }
        actor->control.free_facing_vector = 1;
        goto resolve_movement;
    }

    if ( actor->orders.move.destination.destination_type <= _destination_none )
    {
        force_throttle_facing = 1;
        if ( actor->output.movement_type == actor_movement_type_combat )
        {
            int turn_flag = character_def->flags & (1u << _actor_definition_crouching_must_move_forward_bit);
            if ( !crouch )
                turn_flag = character_def->flags & (1u << _actor_definition_standing_must_move_forward_bit);
            if ( turn_flag )
                actor->control.moving_forced_by_aiming = 0;
        }
        else
        {
            actor->control.moving_forced_by_aiming = 0;
        }
        if ( actor->output.movement_type == actor_movement_type_flaming )
            force_throttle_facing = 1;
        if ( (character_def->flags & (1u << _actor_definition_flying_bit)) != 0 )
        {
            free_movement = 1;
            free_movement_dist_sq = character_def->moving.free_flying_sidestep_distance * character_def->moving.free_flying_sidestep_distance;
            move_in_3d = 1;
            if ( actor->control.moving_forced_by_aiming )
                free_movement_dist_sq = character_def->moving.free_flying_sidestep_distance * character_def->moving.free_flying_sidestep_distance * 4.0f;
        }
    }
    else
    {
        actor->control.free_facing_vector = 1;
        actor->control.moving = 0;
        crouch = (character_def->flags >> _actor_definition_crouch_in_line_of_fire_bit) & 1;
    }

resolve_movement:
    if ( actor->control.moving && !actor->control.movement_complete )
    {
        /* Args re-derived from disasm 0x837CB138-0x837CB18C (float-GPR-shadow ABI; see file header).
         * arg6 is r21 = allow_all_moving_turns (not seat_role); args 12-17 are the six stack slots. */
        actor_move_calculate_movement(actor_index, move_in_3d, override_facing, free_movement,
                                      free_movement_dist_sq, allow_all_moving_turns,
                                      steering_maximum_angle, oversteer_minimum_angle, oversteer_maximum_angle,
                                      rotation_emergency_amount, maximum_throttle,
                                      &actor->control.moving_towards_vector,
                                      &actor->control.desired_facing_vector, &actor->control.moving_facing_direction,
                                      &actor->output.throttle,
                                      &actor->control.movement_thwarted, &actor->control.movement_complete);
        if ( actor->control.movement_complete )
            actor->control.moving = 0;
    }

    unsigned __int8 committed = actor->control.moving;
    if ( actor->control.moving )
    {
        actor->control.optional_facing_vector = 0;
        actor->control.free_facing_vector = 0;
    }
    else if ( force_throttle_facing )
    {
        actor->control.optional_facing_vector = 0;
        actor->control.desired_facing_vector.n[2] = actor->input.facing_vector.n[2];
        actor->control.desired_facing_vector.n[1] = actor->input.facing_vector.n[1];
        *throttle = actor->input.facing_vector.n[0];
        actor->control.moving_facing_direction = 0;
        actor->control.free_facing_vector = 0;
    }
    else if ( actor->control.fixed_stationary_facing )
    {
        actor->control.desired_facing_vector.n[2] = actor->control.fixed_stationary_facing_vector.n[2];
        actor->control.desired_facing_vector.n[1] = actor->control.fixed_stationary_facing_vector.n[1];
        *throttle = actor->control.fixed_stationary_facing_vector.n[0];
        actor->control.moving_facing_direction = 0;
        actor->control.optional_facing_vector = 1;
        actor->control.free_facing_vector = 0;
    }

    if ( force_throttle_facing && !committed )
        actor_clear_discarded_firing_positions(actor_index, 1u);

    if ( actor->control.moving )
        crouch = (character_def->flags & (1u << _actor_definition_cannot_move_crouching_bit)) != 0 ? 0 : crouch;
    actor->control.desire_stationary_facing = 0;
    if ( crouch && (character_def->flags & (1u << _actor_definition_fixed_crouch_facing_bit)) != 0 )
        actor->control.desire_stationary_facing = 1;
    actor->control.crouching = crouch;
    actor_unit_control_crouch(actor_index, crouch);

    if ( actor_move_animation_busy(actor_index) || actor->input.vehicle_index != -1 || actor->input.in_midair )
        is_busy_anim = 1;

    if ( !is_busy_anim && actor->emotions.berserk && !actor->emotions.played_berserk_sound )   /* berserk leap entry */
    {
        int cause_unit = -1;
        int target_prop = actor->target.target_prop_index;
        real_vector3d alignment;
        *(long long *)alignment.n = *(long long *)&actor->input.facing_vector;
        if ( target_prop != -1 )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, target_prop);
            cause_unit = prop->unit_index;
            *(long long *)alignment.n = *(long long *)&prop->actor_to_prop;   /* actor_to_prop xy */
            if ( normalize2d((real_vector2d *)&alignment) == 0.0f )
                *(long long *)alignment.n = *(long long *)&actor->input.facing_vector;
        }
        actor_move_animation_impulse(actor_index, 0, (const real_vector2d *)&alignment);
        ai_communication_event(_ai_communication_berserk, actor->meta.unit_index, cause_unit, _comm_hostility_enemy, -1, -1, NULL);
        actor->emotions.played_berserk_sound = 1;
    }

    if ( allow_jump )
    {
        actor_unit_control_jump(actor_index);
    }
    else if ( actor->input.in_midair || actor->input.vehicle_index != -1 )
    {
        actor->control.jumping_targeted = 0;
    }
    else if ( !actor_move_animation_busy(actor_index) && actor->orders.move.jump )   /* leap desire */
    {
        unsigned __int8 leaped = 0;
        if ( actor->orders.move.jump_leap )
        {
            real_vector3d alignment;
            if ( actor->orders.move.jump_targeted )
            {
                *(long long *)alignment.n = *(long long *)&actor->orders.move.jump_alignment_vector;
            }
            else
            {
                *(long long *)alignment.n = *(long long *)&actor->input.facing_vector;
                if ( normalize2d((real_vector2d *)&alignment) == 0.0f )
                    *(real_vector2d *)alignment.n = *global_forward2d;
            }
            leaped = unit_leap_begin(actor->meta.unit_index, (const real_vector2d *)&alignment);
        }
        if ( leaped )
            ai_communication_event(_ai_communication_leap, actor->meta.unit_index, -1, -1, -1, -1, NULL);
        else
            actor_unit_control_jump(actor_index);
        if ( actor->orders.move.jump_targeted )
        {
            actor->control.jump_target_horizontal_vel = actor->orders.move.jump_target_horizontal_vel;
            actor->control.jumping_targeted = 1;
            actor->control.jump_target_vertical_vel = actor->orders.move.jump_target_vertical_vel;
            /* jump_alignment_vector is real_vector2d (i/j); decompiler wrote .x/.y for the two components. */
            actor->control.jump_alignment_vector.__s1.i = actor->orders.move.jump_alignment_vector.__s1.i;
            actor->control.jump_alignment_vector.__s1.j = actor->orders.move.jump_alignment_vector.__s1.j;
        }
    }

    /* publish the final throttle vector snapshot (actor+1772) */
    actor->output.animation = actor->orders.move.animation;
}