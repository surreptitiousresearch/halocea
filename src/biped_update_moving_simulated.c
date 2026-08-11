/* biped_update_moving_simulated @0x837B1770 — per-frame movement/physics update for a biped (character) unit,
 * simulated variant used by client-side prediction (biped_predicted_movement_localplayer). It fills a
 * biped_physics scratch block from the biped's current state and the biped tag definition, resolves the
 * desired movement vector (from the throttle-driven analog input, a locomotion-animation frame delta, or
 * the flying model), runs biped_update_physics, then writes the integrated position/velocity, ground
 * plane, support/elevator surfaces and landing back onto the biped datum.
 *
 * CAVEAT: the decompiler flagged "local variable allocation has failed" and packed several vector copies
 * into overlapped 64-bit register slots (v91/v92/v93 near the epilogue). Those were untangled into the
 * plain real_point3d/real_vector3d component copies they represent, verified against the store offsets.
 * The desired-facing overshoot test at 0x837B1C1C was a product of two scalar triple products
 * (up . (desired x rotated_forward)) * (up . (desired x forward)); reconstructed as such.
 *
 * DEVIATION: this takes THREE arguments, not four — the call site at 0x837B2788 sets only r3/r4/r5, and
 * r6 is never written. r5 is a unit_animation_update_data*, not a second biped_datum*: every access
 * through it is `stb 0(r25)` / `lbz 1(r25)` / `stb 1(r25)`, bytes 0 and 1 only. The old reconstruction
 * spelled that register as `biped_faux` and reached both outputs as byte [1] of its definition_index,
 * which put the movement-direction state (stores @0x837B19B4 and @0x837B1A68, both to byte 0) on top of
 * the crouch flag (byte 1, @0x837B2158/0x837B237C/0x837B2398). The outputs are the same pair the
 * non-simulated sibling biped_update_moving writes: animation_update->state_desired / ->crouching. */

#include <stdint.h>
#include <string.h>
#include "headers/biped_datum.h"
#include "headers/unit_definition_flags.h"
#include "headers/biped_physics_in_flags.h"
#include "headers/biped_physics_out_flags.h"
#include "headers/biped_definition.h"
#include "headers/biped_physics.h"
#include "headers/unit_animation_update_data.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_player_information.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/player_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/animation_frame_info_type.h"
#include "headers/animation_frame_info_xy_translation.h"
#include "headers/animation_frame_info_xy_translation_yaw_rotation.h"
#include "headers/animation_frame_info_xyz_translation_yaw_rotation.h"
#include "headers/base_seat.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"


/* boundary helpers — not reversed here */
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern float game_difficulty_get_value(int16_t value_type);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern uint8_t cinematic_in_progress(void);
extern uint8_t game_players_are_double_speed(void);
extern uint8_t actor_is_leaping(int actor_index);
extern void biped_update_physics(biped_physics *physics);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);
extern void biped_start_landing(biped_datum *biped, const biped_definition *biped_definition, float landing_velocity);
extern double        __fabs(double x);
extern float         __fsqrts(float x);
extern double        sin(double x);
extern double        cos(double x);

#define SECONDS_PER_TICK 0.033333335f  /* 1/30 */

void biped_update_moving_simulated(
    unsigned int real_biped_index,
    biped_datum *biped,
    unit_animation_update_data *animation_update)  /* named ..._update to not shadow the animation type */
{
    biped_physics physics;
    real_vector3d rotated_forward;
    const biped_definition *definition;
    float movement_scale;
    uint16_t in_flags;

    if ( (biped->object.flags & (1u << _object_at_rest_bit)) != 0
      && (biped->object.damage_flags & (1u << _object_dead_bit)) != 0
      && (biped->unit.animation.flags & (1u << _unit_animation_ignore_translation_bit)) != 0 )
    {
        return;
    }

    physics.biped_index = real_biped_index;
    physics.in_flags = 0;
    physics.forward = biped->object.forward;

    definition = TAG_GET(const biped_definition, biped->definition_index);

    if ( (definition->unit.flags & (1u << _unit_definition_simple_creature_bit)) != 0 )
        physics.aiming = biped->object.forward;
    else
        unit_get_aiming_vector(real_biped_index, &physics.aiming);

    physics.velocity = biped->object.translational_velocity;
    physics.crouch_velocity = 0.0f;
    physics.acceleration_maximum = 0.0053333333f;
    physics.airborne_acceleration_maximum = 0.0f;
    biped_get_physics_pill(real_biped_index, &physics.position, &physics.height, &physics.width);

    physics.minimum_normal_k = definition->biped.runtime_minimum_normal_k;
    physics.downhill_k0 = definition->biped.runtime_downhill_k0;
    physics.downhill_k1 = definition->biped.runtime_downhill_k1;
    physics.downhill_velocity_scale = definition->biped.downhill_velocity_scale;
    physics.uphill_k0 = definition->biped.runtime_uphill_k0;
    physics.uphill_k1 = definition->biped.runtime_uphill_k1;
    physics.uphill_velocity_scale = definition->biped.uphill_velocity_scale;
    physics.ground_plane = biped->biped.ground_plane;
    physics.ground_tangential_velocity_max = 3.4028235e38f;  /* FLT_MAX */
    physics.ground_tangential_angle = 0.0f;
    physics.existing_support_surface_index = biped->biped.support_surface_index;

    if ( (uint16_t)biped->biped.landing == 1 )
    {
        physics.movement_desired.n[0] = 0.0f;
        physics.movement_desired.n[1] = 0.0f;
        physics.movement_desired.n[2] = 0.0f;
        physics.movement_penalty = 1.0f;
    }
    else
    {
        movement_scale = 1.0f;
        if ( (definition->biped.flags & (1u << _biped_random_speed_increase_bit)) != 0 && !biped->unit.aiming_speed )
        {
            /* stagger movement speed per-biped by difficulty (index modulo 137, 1/137 ~= 0.00729927) */
            float difficulty_value = game_difficulty_get_value(_game_difficulty_infection_form_toughness);
            int biped_stagger_ordinal = real_biped_index % 0x89;
            movement_scale = difficulty_value * ((float)biped_stagger_ordinal * 0.00729927f) + 1.0f;
        }

        /* classify the movement-direction animation (forward/back/left/right, +4 while stunned) into the
         * caller's animation-update record (state_desired = _unit_state_move_front .. _stunned_move_right) */
        if ( (definition->biped.flags & (1u << _biped_flying_bit)) == 0 || (biped->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        {
            float throttle_x = biped->unit.throttle.n[0];
            if ( throttle_x != 0.0f
              || biped->unit.throttle.n[1] != 0.0f
              || biped->unit.throttle.n[2] != 0.0f )
            {
                int stunned = biped->unit.body_stun > 0.2f;
                float stunned_movement_threshold = definition->unit.stunned_movement_threshold;
                float throttle_y;
                char movement_base;
                char movement_state;

                if ( stunned_movement_threshold > 0.0f
                  && biped->object.recent_body_damage > stunned_movement_threshold )
                {
                    stunned = 1;
                }
                throttle_y = biped->unit.throttle.n[1];
                movement_base = stunned ? 4 : 0;

                if ( __fabs(throttle_x) >= __fabs(throttle_y) )
                {
                    movement_state = movement_base + 5;
                    if ( throttle_x >= 0.0f )
                        movement_state = movement_base + 4;
                }
                else if ( throttle_y >= 0.0f )
                {
                    movement_state = movement_base + 6;
                }
                else
                {
                    movement_state = movement_base + 7;
                }
                animation_update->state_desired = movement_state;
            }
        }
        else
        {
            animation_update->state_desired = 0;
        }

        /* pull the frame-relative movement delta from the currently-playing locomotion animation */
        {
            int animation_index = biped->object.animation.state.index;

            physics.movement_desired = *global_zero_vector3d;

            if ( animation_index != -1
              && ((biped->object.damage_flags & (1u << _object_dead_bit)) != 0 || (definition->biped.flags & (1u << _biped_flying_bit)) == 0)
              && (biped->unit.animation.flags & (1u << _unit_animation_ignore_translation_bit)) == 0 )
            {
                float frame_yaw_delta = 0.0f;
                float desired_y = physics.movement_desired.n[1];
                animation_graph *graph = TAG_GET(animation_graph, biped->object.animation.animation_graph_index);
                animation *animation_def = (animation *)graph->animations.address + animation_index;
                int frame_component_count = (uint16_t)animation_def->frame_info_type;
                int frame_index = biped->object.animation.state.frame_index;
                /* frame_info is a variable-format stream: the record type — and therefore the
                 * stride — is chosen by animation.frame_info_type, and the DB carries one struct
                 * per arm (8 / 12 / 16 bytes). Indexing each arm as its own type is what makes
                 * the stride implicit instead of a magic multiplier. */
                void *frame_base = animation_def->frame_info.address;

                switch ( frame_component_count )
                {
                    case _animation_frame_info_xy_translation:
                    {
                        const animation_frame_info_xy_translation *frame_data =
                            &((const animation_frame_info_xy_translation *)frame_base)[frame_index];
                        physics.movement_desired.n[0] = frame_data->offset.n[0];
                        desired_y = frame_data->offset.n[1];
                        break;
                    }
                    case _animation_frame_info_xy_translation_yaw_rotation:
                    {
                        const animation_frame_info_xy_translation_yaw_rotation *frame_data =
                            &((const animation_frame_info_xy_translation_yaw_rotation *)
                              frame_base)[frame_index];
                        physics.movement_desired.n[0] = frame_data->offset.n[0];
                        desired_y = frame_data->offset.n[1];
                        physics.movement_desired.n[1] = frame_data->offset.n[1];
                        frame_yaw_delta = frame_data->yaw;
                        break;
                    }
                    case _animation_frame_info_xyz_translation_yaw_rotation:
                    {
                        const animation_frame_info_xyz_translation_yaw_rotation *frame_data =
                            &((const animation_frame_info_xyz_translation_yaw_rotation *)
                              frame_base)[frame_index];
                        physics.movement_desired.n[0] = frame_data->offset.n[0];
                        desired_y = frame_data->offset.n[1];
                        physics.movement_desired.n[1] = frame_data->offset.n[1];
                        physics.movement_desired.n[2] = frame_data->offset.n[2];
                        frame_yaw_delta = frame_data->yaw;
                        break;
                    }
                    default:
                        desired_y = physics.movement_desired.n[1];
                        break;
                }

                physics.movement_desired.n[0] = physics.movement_desired.n[0] * movement_scale;
                physics.movement_desired.n[1] = desired_y * movement_scale;
                physics.movement_desired.n[2] = physics.movement_desired.n[2] * movement_scale;

                if ( __fabs(frame_yaw_delta) >= 0.000099999997 )
                {
                    float yaw_cos = (float)cos(frame_yaw_delta);
                    float yaw_sin = (float)sin(frame_yaw_delta);

                    rotated_forward = biped->object.forward;
                    rotate_vector_about_axis(&rotated_forward, &biped->object.up, yaw_sin, yaw_cos);

                    if ( (biped->unit.control_flags & (1u << _unit_control_exact_facing_bit)) != 0 )
                    {
                        char animation_state = biped->unit.animation.state;
                        if ( animation_state == _unit_state_turn_left || animation_state == _unit_state_turn_right )
                        {
                            real_vector3d *desired_facing = &biped->unit.desired_facing_vector;
                            real_vector3d *forward = &biped->object.forward;
                            float dot_forward_desired =
                                  (forward->n[0] * desired_facing->n[0])
                                + (desired_facing->n[2] * forward->n[2])
                                + (desired_facing->n[1] * forward->n[1]);

                            if ( dot_forward_desired > 0.5f )
                            {
                                real_vector3d *object_up = &biped->object.up;
                                /* scalar triple products: up . (desired x rotated_forward) and
                                 * up . (desired x forward). Opposite signs => the rotation overshot the
                                 * desired facing, so snap straight to the desired vector. */
                                float det_rotated =
                                      object_up->n[0] * (desired_facing->n[1] * rotated_forward.n[2] - desired_facing->n[2] * rotated_forward.n[1])
                                    + object_up->n[2] * (rotated_forward.n[1] * desired_facing->n[0] - desired_facing->n[1] * rotated_forward.n[0])
                                    + object_up->n[1] * (desired_facing->n[2] * rotated_forward.n[0] - rotated_forward.n[2] * desired_facing->n[0]);
                                float det_forward =
                                      object_up->n[1] * (desired_facing->n[2] * forward->n[0] - forward->n[2] * desired_facing->n[0])
                                    + object_up->n[2] * (forward->n[1] * desired_facing->n[0] - desired_facing->n[1] * forward->n[0])
                                    + object_up->n[0] * (forward->n[2] * desired_facing->n[1] - forward->n[1] * desired_facing->n[2]);

                                if ( det_rotated * det_forward <= 0.0f )
                                    rotated_forward = *desired_facing;
                            }
                        }
                    }

                    biped->object.forward.n[0] = rotated_forward.n[0];
                    biped->object.forward.n[1] = rotated_forward.n[1];
                    biped->object.forward.n[2] = rotated_forward.n[2];
                    biped_snap_facing(biped, definition);
                }
            }
        }

        {
            unsigned int def_flags = definition->biped.flags;
            if ( (def_flags & (1u << _biped_flying_bit)) == 0 || (biped->object.damage_flags & (1u << _object_dead_bit)) != 0 )
            {
                if ( (def_flags & (1u << _biped_uses_player_physics_bit)) == 0 || biped->unit.animation.state == _unit_state_user_animation )
                {
                    /* AI/scripted ground movement: adopt the animation-driven velocity directly */
                    unsigned int biped_flags = biped->biped.flags;
                    if ( (biped_flags & (1u << _biped_slipping_bit)) == 0 && (biped_flags & (1u << _biped_airborne_bit)) == 0 )
                    {
                        biped->object.translational_velocity.n[0] = physics.movement_desired.n[0];
                        biped->object.translational_velocity.n[1] = physics.movement_desired.n[1];
                        physics.acceleration_maximum = 3.4028235e38f;  /* FLT_MAX */
                    }
                }
                else
                {
                    /* player-controlled ground movement */
                    float body_stun_scale = 1.0f;
                    game_globals_player_information *player_movement = (game_globals_player_information *)global_game_globals->player_information.address;
                    game_globals_player_information movement_params_copy;
                    int player_index;
                    float throttle_x;
                    float crouch;
                    float uncrouch;
                    float stun_factor;
                    float forward_speed;
                    float side_speed;
                    int base_seat_index;
                    float forward_velocity;
                    float side_velocity;
                    float acceleration;

                    if ( cinematic_in_progress() || (definition->biped.flags & (1u << _biped_uses_old_player_physics_bit)) != 0 )
                    {
                        memcpy(&movement_params_copy, player_movement, sizeof(movement_params_copy));
                        player_movement = &movement_params_copy;
                        movement_params_copy.walking_speed = 0.51200002f;
                        movement_params_copy.run_forward_speed = 2.25f;
                        movement_params_copy.run_backward_speed = 2.0f;
                        movement_params_copy.run_sideways_speed = 2.0f;
                        movement_params_copy.run_acceleration = 0.31999999f;
                    }

                    player_index = biped->unit.player_index;
                    if ( player_index != -1 )
                    {
                        body_stun_scale = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)
                                            ->multiplayer.speed_multiplier;
                    }

                    throttle_x = biped->unit.throttle.n[0];
                    crouch = biped->biped.crouch;
                    uncrouch = 1.0f - biped->biped.crouch;
                    stun_factor = -((player_movement->stun_movement_penalty * biped->unit.body_stun) - 1.0f)
                                    * body_stun_scale * movement_scale;

                    if ( throttle_x <= 0.0f )
                    {
                        forward_speed = player_movement->sneak_backward_speed;
                        side_speed = player_movement->run_backward_speed;
                    }
                    else
                    {
                        forward_speed = player_movement->sneak_forward_speed;
                        side_speed = player_movement->run_forward_speed;
                    }

                    base_seat_index = (uint8_t)biped->unit.animation.base_seat_index;
                    forward_velocity = (side_speed * uncrouch) + (forward_speed * crouch);
                    physics.movement_desired.n[0] = forward_velocity;
                    side_velocity = (player_movement->run_sideways_speed * uncrouch) + (player_movement->sneak_sideways_speed * crouch);
                    physics.movement_desired.n[1] = side_velocity;
                    physics.movement_desired.n[2] = 0.0f;
                    acceleration = (player_movement->run_acceleration * uncrouch) + (player_movement->sneak_acceleration * crouch);
                    physics.acceleration_maximum = acceleration;

                    if ( base_seat_index == _base_seat_alert )
                    {
                        if ( throttle_x <= 0.0f )
                            forward_velocity = 0.0f;
                        else
                            forward_velocity = player_movement->walking_speed;
                        side_velocity = 0.0f;
                    }

                    {
                        unsigned int no_facing = biped->unit.control_flags & (1u << _unit_control_look_dont_turn_bit);
                        float side_desired = (biped->unit.throttle.n[1] * stun_factor) * side_velocity;

                        physics.acceleration_maximum = acceleration * SECONDS_PER_TICK;
                        physics.movement_desired.n[0] = ((throttle_x * stun_factor) * forward_velocity) * SECONDS_PER_TICK;
                        physics.movement_desired.n[1] = side_desired * SECONDS_PER_TICK;
                        physics.airborne_acceleration_maximum = player_movement->airborne_acceleration * SECONDS_PER_TICK;

                        if ( !no_facing )
                        {
                            physics.forward = biped->unit.desired_facing_vector;
                        }
                    }

                    if ( game_players_are_double_speed() )
                    {
                        physics.movement_desired.n[0] = player_movement->double_speed_multiplier * physics.movement_desired.n[0];
                        physics.movement_desired.n[1] = player_movement->double_speed_multiplier * physics.movement_desired.n[1];
                        physics.movement_desired.n[2] = player_movement->double_speed_multiplier * physics.movement_desired.n[2];
                    }
                }
            }
            else
            {
                /* flying movement model */
                float throttle_magnitude = __fsqrts(
                      (biped->unit.throttle.n[1] * biped->unit.throttle.n[1])
                    + (biped->unit.throttle.n[2] * biped->unit.throttle.n[2])
                    + (biped->unit.throttle.n[0] * biped->unit.throttle.n[0]));
                float clamped_throttle = throttle_magnitude >= 1.0f ? 1.0f : throttle_magnitude;
                float inv_throttle = 1.0f - clamped_throttle;
                float crouch_modifier = 1.0f;
                float throttle_x;
                float throttle_y;
                float throttle_z;
                float flying_acceleration;

                if ( definition->biped.flying_crouch_velocity_modifier > 0.0f )
                {
                    float crouch = biped->biped.crouch;
                    if ( crouch == 1.0f )
                    {
                        crouch_modifier = definition->biped.flying_crouch_velocity_modifier;
                    }
                    else if ( crouch > 0.0f )
                    {
                        crouch_modifier = ((definition->biped.flying_crouch_velocity_modifier - 1.0f)
                                            * biped->biped.crouch) + 1.0f;
                    }
                }

                throttle_x = biped->unit.throttle.n[0];
                throttle_y = biped->unit.throttle.n[1];
                throttle_z = biped->unit.throttle.n[2];

                physics.movement_desired.n[0] = (definition->biped.flying_velocity * crouch_modifier) * movement_scale;
                physics.movement_desired.n[1] = (crouch_modifier * movement_scale) * definition->biped.flying_sidestep_velocity;
                physics.movement_desired.n[2] = (crouch_modifier * movement_scale) * definition->biped.flying_sidestep_velocity;
                flying_acceleration = ((1.0f - inv_throttle) * definition->biped.flying_acceleration)
                                        + (definition->biped.flying_deceleration * inv_throttle);
                physics.movement_desired.n[0] = (throttle_x * physics.movement_desired.n[0]) * SECONDS_PER_TICK;
                physics.movement_desired.n[1] = (throttle_y * physics.movement_desired.n[1]) * SECONDS_PER_TICK;
                physics.movement_desired.n[2] = (throttle_z * physics.movement_desired.n[2]) * SECONDS_PER_TICK;
                physics.acceleration_maximum = ((flying_acceleration * crouch_modifier) * movement_scale) * SECONDS_PER_TICK;
                physics.airborne_acceleration_maximum = physics.acceleration_maximum;
            }

            if ( (biped->biped.flags & (1u << _biped_airborne_bit)) != 0 )
            {
                int leaping = 0;
                if ( biped->biped.airborne_ticks < 22 )
                {
                    int actor_index = biped->unit.actor_index;
                    if ( actor_index != -1 )
                        leaping = actor_is_leaping(actor_index) != 0;
                }
                if ( leaping )
                {
                    physics.ground_tangential_angle = 0.5f;
                    physics.ground_tangential_velocity_max = 0.1f;
                }
            }
            physics.movement_penalty = 0.0f;
        }
    }

    /* advance the crouch transition toward standing (0.0) or crouched (1.0) */
    if ( biped->unit.animation.base_seat_index == _base_seat_crouch )
    {
        if ( (1.0f - biped->biped.crouch) <= (double)definition->biped.runtime_crouch_transition_velocity )
            biped->biped.crouch = 1.0f;
        else
            biped->biped.crouch = biped->biped.crouch + definition->biped.runtime_crouch_transition_velocity;
    }
    else if ( -biped->biped.crouch >= -definition->biped.runtime_crouch_transition_velocity )
    {
        biped->biped.crouch = 0.0f;
    }
    else
    {
        biped->biped.crouch = biped->biped.crouch - definition->biped.runtime_crouch_transition_velocity;
    }

    if ( biped->biped.crouch == 0.0f )
    {
        in_flags = physics.in_flags;
    }
    else
    {
        uint8_t crouch_was_active = animation_update->crouching;
        physics.in_flags |= (1u << _biped_physics_in_crouched_bit);
        in_flags = physics.in_flags;
        if ( !crouch_was_active )
        {
            in_flags |= (1u << _biped_physics_in_trying_to_stand_bit);
            physics.in_flags = in_flags;
        }
    }

    {
        unsigned int biped_flags = biped->biped.flags;
        if ( (biped_flags & (1u << _biped_airborne_bit)) != 0 )          { in_flags |= (1u << _biped_physics_in_airborne_bit);          physics.in_flags = in_flags; }
        if ( (biped_flags & (1u << _biped_slipping_bit)) != 0 )          { in_flags |= (1u << _biped_physics_in_slipping_bit);          physics.in_flags = in_flags; }
        if ( (biped_flags & (1u << _biped_absolute_movement_bit)) != 0 ) { in_flags |= (1u << _biped_physics_in_absolute_movement_bit); physics.in_flags = in_flags; }
        if ( (biped_flags & (1u << _biped_no_collision_bit)) != 0 )      { in_flags |= (1u << _biped_physics_in_no_collision_bit);      physics.in_flags = in_flags; }
    }

    {
        int is_dead = biped->object.damage_flags & (1u << _object_dead_bit);
        if ( is_dead )                                             { in_flags |= (1u << _biped_physics_in_dead_bit);  physics.in_flags = in_flags; }
        if ( (definition->biped.flags & (1u << _biped_flying_bit)) != 0 && !is_dead ) { in_flags |= (1u << _biped_physics_in_flying_bit);  physics.in_flags = in_flags; }
        if ( (definition->biped.flags & (1u << _biped_passes_through_bipeds_bit)) != 0 )          { in_flags |= (1u << _biped_physics_in_pass_through_bipeds_bit); physics.in_flags = in_flags; }
        if ( (definition->biped.flags & (1u << _biped_climbs_anything_bit)) != 0 && !is_dead ) physics.in_flags = in_flags | (1u << _biped_physics_in_climb_anything_bit);
    }

    biped_update_physics(&physics);

    if ( physics.elevator_object_index == -1 )
    {
        int elevator_ticks = biped->biped.elevator_ticks;
        if ( elevator_ticks <= 0 )
            biped->biped.elevator_object_index = -1;
        else
            biped->biped.elevator_ticks = elevator_ticks - 1;
    }
    else
    {
        biped->biped.elevator_object_index = physics.elevator_object_index;
        biped->biped.elevator_ticks = 60;
    }

    {
        real_point3d  final_position;
        real_vector3d final_velocity;
        unsigned char out_flags;
        float adjusted_z;

        if ( (biped->unit.flags & (1u << _unit_suspended_bit)) != 0 )
        {
            /* attached to an elevator/mover: keep the resolved position, zero the velocity */
            out_flags = physics.out_flags & ~(1u << _biped_physics_out_airborne_bit);
            physics.out_flags &= ~(1u << _biped_physics_out_airborne_bit);
            final_position = physics.position;
            final_velocity = *global_zero_vector3d;
        }
        else
        {
            final_position = physics.new_position;
            final_velocity = physics.new_velocity;
            out_flags = (unsigned char)physics.out_flags;
        }

        adjusted_z = final_position.n[2];
        if ( (definition->biped.flags & (1u << _biped_pill_centered_at_origin_bit)) == 0 )
            adjusted_z = final_position.n[2] - physics.width;

        biped->object.translational_velocity.n[0] = final_velocity.n[0];
        biped->object.translational_velocity.n[1] = final_velocity.n[1];
        biped->object.translational_velocity.n[2] = final_velocity.n[2];
        biped->object.position.n[0] = final_position.n[0];
        biped->object.position.n[1] = final_position.n[1];
        biped->object.position.n[2] = adjusted_z;
        biped->biped.pathfinding_surface_index = -1;
        biped->biped.pathfinding_point.n[0] = final_position.n[0];
        biped->biped.pathfinding_point.n[1] = final_position.n[1];
        biped->biped.pathfinding_point.n[2] = adjusted_z;
        biped->biped.support_surface_index = physics.support_surface_index;

        if ( !animation_update->crouching && (out_flags & (1u << _biped_physics_out_cannot_stand_bit)) != 0 )
            animation_update->crouching = 1;

        {
            unsigned int biped_flags = biped->biped.flags;
            biped_flags = (out_flags & (1u << _biped_physics_out_airborne_bit)) != 0 ? (biped_flags | (1u << _biped_airborne_bit)) : (biped_flags & ~(1u << _biped_airborne_bit));
            biped->biped.flags = biped_flags;
            biped_flags = (out_flags & (1u << _biped_physics_out_slipping_bit)) != 0 ? (biped_flags | (1u << _biped_slipping_bit)) : (biped_flags & ~(1u << _biped_slipping_bit));
            biped->biped.flags = biped_flags;
            biped_flags = biped->biped.flags;
            biped_flags = (definition->biped.flags & (1u << _biped_passes_through_bipeds_bit)) != 0 ? (biped_flags | (1u << _biped_movement_passes_through_bipeds_bit)) : (biped_flags & ~(1u << _biped_movement_passes_through_bipeds_bit));
            biped->biped.flags = biped_flags;
        }

        {
            float landing_velocity = physics.landing_velocity;
            int do_landing = physics.landing_velocity > 0.0f;
            biped->biped.ground_plane.n.n[0] = physics.ground_plane.n.n[0];
            biped->biped.ground_plane.n.n[1] = physics.ground_plane.n.n[1];
            biped->biped.ground_plane.n.n[2] = physics.ground_plane.n.n[2];
            biped->biped.ground_plane.d = physics.ground_plane.d;
            if ( do_landing )
                biped_start_landing(biped, definition, landing_velocity);
        }

        {
            int airborne_input = physics.in_flags & (1u << _biped_physics_in_flying_bit);
            unsigned int object_flags;

            if ( (physics.in_flags & (1u << _biped_physics_in_flying_bit)) != 0 || (biped->biped.flags & (1u << _biped_airborne_bit)) != 0 )
                object_flags = biped->object.flags & ~(1u << _object_on_ground_bit);
            else
                object_flags = biped->object.flags | (1u << _object_on_ground_bit);
            biped->object.flags = object_flags;

            if ( airborne_input
              || (biped->biped.flags & (1u << _biped_airborne_bit)) != 0
              || (out_flags & (1u << _biped_physics_out_volatile_collision_bit)) != 0
              || ((biped->object.translational_velocity.n[2] * biped->object.translational_velocity.n[2])
                    + ((biped->object.translational_velocity.n[0] * biped->object.translational_velocity.n[0])
                     + (biped->object.translational_velocity.n[1] * biped->object.translational_velocity.n[1]))) >= 0.000099999997 )
            {
                object_flags = biped->object.flags & ~(1u << _object_at_rest_bit);
            }
            else
            {
                object_flags |= (1u << _object_at_rest_bit);
            }
            biped->object.flags = object_flags;

            if ( (object_flags & (1u << _object_on_ground_bit)) != 0 )
                biped->object.angular_velocity = *global_zero_vector3d;
        }
    }
}
