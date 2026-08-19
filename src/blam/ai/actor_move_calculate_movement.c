/* actor_move_calculate_movement @0x837C8AE0 — the core per-tick AI locomotion solver: from a desired
 * movement vector and a bundle of steering parameters, compute the actor's desired facing direction, facing
 * direction code, throttle vector, and the "movement thwarted" / "movement complete" flags.
 *
 * The Hex-Rays output is flagged "local variable allocation has failed"; the body was reconstructed against
 * the disassembly. The reg-allocation failure produced a fabricated 50-parameter signature — the real
 * prototype (DB funcs.prototype, 17 args) is authoritative. The failure manifested in three ways, all fixed
 * here:
 *   1. The six pointer parameters passed on the stack (slots 11-16) were fabricated as phantom
 *      half-slot params plus duplicate `*_0` params. Resolved from usage + the epilogue stores:
 *        stack slot 11 -> desired_movement_vector  (input)
 *        stack slot 12 -> desired_facing_vector    (output)
 *        stack slot 13 -> desired_facing_direction (output)
 *        stack slot 14 -> desired_throttle         (output)
 *        stack slot 15 -> movement_thwarted        (output u8; decompiler wrote HIBYTE of n[0])
 *        stack slot 16 -> movement_complete        (output u8; same)
 *   2. allow_all_moving_turns was shown as a `(char)` cast of the movement-vector pointer, which is
 *      garbage; disasm (mr r23,r8) shows that byte is the caller's r8 — the real argument.
 *   3. The normalize-fallback register aliasing (two OVERLAPPED locals) is one cached direction,
 *      `fallback_direction` below: once the facing vector normalizes (falling back to the actor's
 *      forward vector when degenerate), it is reused for the later movement/aim normalizations.
 *
 * Actor-datum field provenance — recovered by offset, all now named members (DB types_members):
 *   +372 -> input.facing_vector; +300 -> input.position.body_position; +356 -> input.pathfinding_surface_index
 */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/structure_bsp.h"
#include "headers/path_collision_result.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_point2d.h"
#include "headers/ppc_intrinsics.h"
#include <math.h>
#include "headers/actor_movement_type.h"
#include "headers/actor_facing.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


#include "headers/structure_bsp.h"
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern void actor_move_calculate_free(uint8_t move_in_3d, const real_vector3d *movement_direction, const real_vector3d *facing_direction, real_vector3d *throttle);
extern void actor_move_calculate_controlled_by_aiming(uint8_t move_in_3d, const real_vector3d *movement_vector, const real_vector3d *forced_aim_direction, const real_vector3d *current_facing_vector, real_vector3d *desired_facing_vector, int16_t *desired_facing_direction);
extern void actor_find_pathfinding_location(int actor_index);
extern uint8_t structure_test_line2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *p0, int p0_surface_index, const real_point2d *p1, int p1_surface_index, path_collision_result *result);
extern float actor_destination_tolerance(int actor_index);
extern void actor_get_stopping_distances(int actor_index, float *current_stopping_distance_reference, float *maximum_stopping_distance_reference);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void actor_move_calculate_movement(int actor_index, uint8_t move_in_3d, int16_t override_facing,
    uint8_t free_movement, float free_movement_dist_sq, uint8_t allow_all_moving_turns,
    float steering_maximum_angle, float oversteer_minimum_angle, float oversteer_maximum_angle,
    float rotation_emergency_amount, float maximum_throttle, const real_vector3d *desired_movement_vector,
    real_vector3d *desired_facing_vector, int16_t *desired_facing_direction, real_vector3d *desired_throttle,
    uint8_t *movement_thwarted, uint8_t *movement_complete)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    /* tag pointer kept as a typed pointer (was `int` holding the pointer, which
     * truncates on the x64 host); removes the (actor_definition *) pun at its use. */
    const actor_definition *character_tag = TAG_GET(const actor_definition, actor->meta.definition_index);

    float facing_dot_threshold = 0.86602539f;   /* cos(30 deg); raised to the character tag value for long moves */
    int16_t aiming_facing_code = -1;             /* aiming-path output; decompiler fused this init into the high half of a 32-bit slot */

    if ( actor->orders.move.move_face_exactly )
        actor->control.face_exactly = 1;

    int16_t facing_code;                         /* facing direction code (0=fwd,1=back,2=left,3=right,4=free) */
    real_vector3d facing_direction;
    real_vector3d scratch;
    real_vector3d movement_direction;
    real_vector3d free_throttle;                 /* actor_move_calculate_free output */

    if ( (unsigned int)override_facing < 4 )
    {
        facing_code = override_facing;

        scratch.n[0] = desired_movement_vector->n[0];
        scratch.n[1] = desired_movement_vector->n[1];
        scratch.n[2] = desired_movement_vector->n[2];
        if ( !move_in_3d )
            scratch.n[2] = 0.0f;
        if ( normalize3d(&scratch) == 0.0f )
        {
            scratch.n[0] = actor->input.facing_vector.n[0];
            scratch.n[1] = actor->input.facing_vector.n[1];
            scratch.n[2] = actor->input.facing_vector.n[2];
        }

        /* actor_move_orders.override_movement_facing domain: actor_facing.h
         * (types_enum_values $53D606AE39E79059735B54401B32FAA6) */
        switch ( override_facing )
        {
            case _actor_facing_forward:
                facing_direction = scratch;
                goto facing_ready;
            case _actor_facing_backward:
                facing_direction.n[0] = -scratch.n[0];
                facing_direction.n[1] = -scratch.n[1];
                break;
            case _actor_facing_left:
                facing_direction.n[1] = scratch.n[0];
                facing_direction.n[0] = -scratch.n[1];
                break;
            default:   /* _actor_facing_right (3); the guard above bounds override_facing to 0..3 */
                facing_direction.n[0] = scratch.n[1];
                facing_direction.n[1] = -scratch.n[0];
                break;
        }
        facing_direction.n[2] = scratch.n[2];

facing_ready:
        if ( free_movement )
        {
            actor_move_calculate_free(move_in_3d, &scratch, &facing_direction, &free_throttle);
            facing_code = 4;
        }
    }
    else
    {
        float movement_magnitude_sq =
              (desired_movement_vector->n[2] * desired_movement_vector->n[2])
            + ((desired_movement_vector->n[0] * desired_movement_vector->n[0])
                    + (desired_movement_vector->n[1] * desired_movement_vector->n[1]));
        if ( movement_magnitude_sq > 0.64000005f )
            facing_dot_threshold = character_tag->moving.runtime_begin_movement_angle_cosine;

        if ( free_movement && movement_magnitude_sq < free_movement_dist_sq )
        {
            /* Near the destination while free-moving: blend the aim/forward facing against the movement. */
            char use_aim_facing = 0;
            movement_direction.n[0] = desired_movement_vector->n[0];
            movement_direction.n[1] = desired_movement_vector->n[1];
            movement_direction.n[2] = desired_movement_vector->n[2];
            if ( actor->control.moving_forced_by_aiming )
            {
                facing_direction.n[0] = actor->control.moving_forced_aim_direction.n[0];
                facing_direction.n[1] = actor->control.moving_forced_aim_direction.n[1];
                facing_direction.n[2] = actor->control.moving_forced_aim_direction.n[2];
                use_aim_facing = actor->input.vehicle_driver_type > 0;
            }
            else
            {
                facing_direction.n[0] = actor->input.facing_vector.n[0];
                facing_direction.n[1] = actor->input.facing_vector.n[1];
                facing_direction.n[2] = actor->input.facing_vector.n[2];
            }
            if ( !move_in_3d )
            {
                movement_direction.n[2] = 0.0f;
                facing_direction.n[2] = 0.0f;
            }

            if ( normalize3d(&facing_direction) == 0.0f )
            {
                facing_direction.n[0] = actor->input.facing_vector.n[0];
                facing_direction.n[1] = actor->input.facing_vector.n[1];
                facing_direction.n[2] = actor->input.facing_vector.n[2];
            }
            real_vector3d fallback_direction = facing_direction;

            if ( normalize3d(&movement_direction) == 0.0f )
                movement_direction = fallback_direction;

            const real_vector3d *free_facing;
            if ( use_aim_facing )
            {
                scratch.n[0] = actor->input.facing_vector.n[0];
                scratch.n[1] = actor->input.facing_vector.n[1];
                scratch.n[2] = actor->input.facing_vector.n[2];
                if ( !move_in_3d )
                    scratch.n[2] = 0.0f;
                if ( normalize3d(&scratch) == 0.0f )
                    scratch = fallback_direction;
                free_facing = &scratch;
            }
            else
            {
                free_facing = &facing_direction;
            }

            actor_move_calculate_free(move_in_3d, &movement_direction, free_facing, &free_throttle);
            facing_code = 4;
        }
        else if ( actor->control.moving_forced_by_aiming )
        {
            actor_move_calculate_controlled_by_aiming(move_in_3d, desired_movement_vector,
                &actor->control.moving_forced_aim_direction, &actor->input.facing_vector,
                &facing_direction, &aiming_facing_code);
            facing_code = aiming_facing_code;
        }
        else
        {
            facing_direction.n[0] = desired_movement_vector->n[0];
            facing_direction.n[1] = desired_movement_vector->n[1];
            facing_direction.n[2] = desired_movement_vector->n[2];
            if ( !move_in_3d )
                facing_direction.n[2] = 0.0f;
            if ( normalize3d(&facing_direction) == 0.0f )
            {
                facing_direction.n[0] = actor->input.facing_vector.n[0];
                facing_direction.n[1] = actor->input.facing_vector.n[1];
                facing_direction.n[2] = actor->input.facing_vector.n[2];
            }
            facing_code = 0;
        }
    }

    /* Dot of desired facing against the actor's current forward vector. */
    float facing_dot = (facing_direction.n[1] * actor->input.facing_vector.n[1])
        + ((actor->input.facing_vector.n[0] * facing_direction.n[0])
                + (actor->input.facing_vector.n[2] * facing_direction.n[2]));

    uint8_t can_move;
    if ( allow_all_moving_turns || actor->output.movement_type == actor_movement_type_flaming )
    {
        can_move = 1;
    }
    else
    {
        if ( !actor->state.flying )
        {
            actor_find_pathfinding_location(actor_index);
            if ( actor->input.pathfinding_surface_index != -1 )
            {
                char do_line_test = 1;
                if ( (unsigned int)facing_code > 3 )
                {
                    do_line_test = 0;
                }
                else if ( facing_code == 1 )
                {
                    scratch.n[0] = -actor->input.facing_vector.n[0];
                    scratch.n[1] = -actor->input.facing_vector.n[1];
                    scratch.n[2] = actor->input.facing_vector.n[2];
                }
                else if ( facing_code == 2 )
                {
                    scratch.n[0] = actor->input.facing_vector.n[1];
                    scratch.n[1] = -actor->input.facing_vector.n[0];
                    scratch.n[2] = actor->input.facing_vector.n[2];
                }
                else if ( facing_code )   /* 3 */
                {
                    scratch.n[0] = -actor->input.facing_vector.n[1];
                    scratch.n[1] = actor->input.facing_vector.n[0];
                    scratch.n[2] = actor->input.facing_vector.n[2];
                }
                else                      /* 0 */
                {
                    scratch.n[0] = actor->input.facing_vector.n[0];
                    scratch.n[1] = actor->input.facing_vector.n[1];
                    scratch.n[2] = actor->input.facing_vector.n[2];
                }

                if ( do_line_test && normalize2d((real_vector2d *)&scratch) > 0.0f )
                {
                    real_point2d target;
                    float target_z;
                    target.n[0] = (scratch.n[0] * 0.40000001f) + actor->input.position.body_position.x;
                    target.n[1] = (scratch.n[1] * 0.40000001f) + actor->input.position.body_position.y;
                    target_z = actor->input.position.body_position.z;
                    (void)target_z;
                    path_collision_result line_result[3];
                    if ( structure_test_line2d(global_structure_bsp, actor->emotions.ignorant_of_broken_surfaces,
                             (const real_point2d *)&actor->input.position.body_position, actor->input.pathfinding_surface_index, &target, -1, line_result) )
                    {
                        if ( facing_dot_threshold <= 0.94999999f )
                            facing_dot_threshold = 0.94999999f;
                    }
                }
            }
        }
        can_move = facing_dot > facing_dot_threshold;
    }

    float destination_tolerance = actor_destination_tolerance(actor_index);
    float movement_magnitude_sq =
          (desired_movement_vector->n[0] * desired_movement_vector->n[0])
        + ((desired_movement_vector->n[1] * desired_movement_vector->n[1])
                + (desired_movement_vector->n[2] * desired_movement_vector->n[2]));
    *movement_complete = movement_magnitude_sq < (destination_tolerance * destination_tolerance);

    float current_stopping_distance;
    float maximum_stopping_distance;
    actor_get_stopping_distances(actor_index, &current_stopping_distance, &maximum_stopping_distance);
    if ( !actor->control.path.destination_orders.keep_moving && movement_magnitude_sq < (current_stopping_distance * current_stopping_distance) )
    {
        float distance = __fsqrts(movement_magnitude_sq);
        if ( distance <= (maximum_stopping_distance + 0.050000001f)
          || current_stopping_distance <= (double)maximum_stopping_distance )
        {
            maximum_throttle = 0.0f;
        }
        else
        {
            float scaled = ((distance - maximum_stopping_distance)
                                 / (current_stopping_distance - maximum_stopping_distance));
            if ( maximum_throttle > scaled )
                maximum_throttle = scaled;
        }
    }

    /* Throttle vector, oriented by the facing code and scaled by the (possibly reduced) throttle. */
    scratch.n[0] = global_zero_vector3d->n[0];
    scratch.n[1] = global_zero_vector3d->n[1];
    scratch.n[2] = global_zero_vector3d->n[2];
    if ( can_move )
    {
        if ( (unsigned int)facing_code <= 4 )
        {
            if ( facing_code == 1 )
                scratch.n[0] = -1.0f;
            else if ( facing_code == 2 )
                scratch.n[1] = -1.0f;
            else if ( facing_code == 3 )
                scratch.n[1] = 1.0f;
            else if ( facing_code )   /* 4 */
                scratch = free_throttle;
            else                      /* 0 */
                scratch.n[0] = 1.0f;
        }
        scratch.n[0] = scratch.n[0] * maximum_throttle;
        scratch.n[1] = scratch.n[1] * maximum_throttle;
        scratch.n[2] = scratch.n[2] * maximum_throttle;
        *movement_thwarted = 0;
    }
    else
    {
        actor->control.face_exactly = 1;
        *movement_thwarted = 1;
    }

    /* Steering / oversteer: rotate the facing direction toward the target by a clamped angle step. */
    if ( steering_maximum_angle > 0.0f || oversteer_maximum_angle > 0.0f )
    {
        float angle;
        if ( facing_dot < 1.0f )
        {
            if ( facing_dot > -1.0f )
                angle = (float)acos(facing_dot);
            else
                angle = PI;
        }
        else
        {
            angle = 0.0f;
        }

        float clamped = angle;
        if ( steering_maximum_angle > 0.0f )
        {
            float emergency = (steering_maximum_angle * rotation_emergency_amount);
            float limit = steering_maximum_angle;
            if ( rotation_emergency_amount > 1.0f )
            {
                float factor = 1.5f;
                if ( rotation_emergency_amount <= 1.5f )
                    factor = rotation_emergency_amount;
                limit = (factor * steering_maximum_angle);
            }
            if ( (angle * 3.0f) <= emergency )
                emergency = (angle * 3.0f);
            if ( angle >= emergency )
            {
                if ( angle > limit )
                    clamped = limit;
            }
            else
            {
                clamped = emergency;
            }
        }

        float oversteer_accumulator = actor->control.face_exactly_oversteer_angle;
        if ( clamped <= oversteer_accumulator )
        {
            if ( oversteer_accumulator > 0.0f )
            {
                if ( clamped >= oversteer_minimum_angle )
                    clamped = actor->control.face_exactly_oversteer_angle;
                else
                    actor->control.face_exactly_oversteer_angle = 0.0f;
            }
        }
        else if ( actor->control.face_exactly && clamped > oversteer_minimum_angle )
        {
            if ( clamped <= oversteer_maximum_angle )
                actor->control.face_exactly_oversteer_angle = clamped;
            else
                actor->control.face_exactly_oversteer_angle = oversteer_maximum_angle;
        }

        float delta = (clamped - angle);
        if ( __fabs(delta) > 0.000099999997 )
        {
            real_vector3d axis;   /* cross(actor_forward, facing_direction) */
            axis.n[1] = (actor->input.facing_vector.n[2] * facing_direction.n[0])
                      - (actor->input.facing_vector.n[0] * facing_direction.n[2]);
            axis.n[2] = (facing_direction.n[1] * actor->input.facing_vector.n[0])
                      - (actor->input.facing_vector.n[1] * facing_direction.n[0]);
            axis.n[0] = (actor->input.facing_vector.n[1] * facing_direction.n[2])
                      - (facing_direction.n[1] * actor->input.facing_vector.n[2]);
            if ( normalize3d(&axis) > 0.0f )
                rotate_vector_about_axis(&facing_direction, &axis, sin(delta), cos(delta));
        }
    }

    *desired_facing_direction = facing_code;
    desired_facing_vector->n[0] = facing_direction.n[0];
    desired_facing_vector->n[1] = facing_direction.n[1];
    desired_facing_vector->n[2] = facing_direction.n[2];
    desired_throttle->n[0] = scratch.n[0];
    desired_throttle->n[1] = scratch.n[1];
    desired_throttle->n[2] = scratch.n[2];
}