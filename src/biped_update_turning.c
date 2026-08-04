/* biped_update_turning @0x837B0FC0 — per-tick facing/turning update for a biped. Two entirely different
 * modes:
 *
 *  - Flying-camera mode (definition flags & 4, and the biped isn't taking damage): a free-look "noclip"
 *    style turn that snaps straight to the desired facing when the biped is nearly stationary (velocity/
 *    angular-velocity/throttle all below tiny thresholds and the facing is already close to desired), else
 *    blends toward a throttle-tilted facing; separately updates a banking angle from the throttle-driven
 *    turn rate and applies it via unit_euler_aiming_update (or a direct facing snap when the flying
 *    angular acceleration is zero); finishes with biped_snap_facing.
 *
 *  - Normal ground/vehicle mode: computes a signed "turn direction" (which side the desired facing is on)
 *    from either the up-axis cross product (airborne-capable bipeds) or a flattened 2D cross product
 *    (grounded bipeds), snapping it to the near side when almost fully turned around; if actively moving
 *    or forced to turn (biped state 1 / definition flags & 1), rotates the facing (and banks around the
 *    facing axis if the biped can fly) by a fixed per-tick turning speed, stopping as soon as the facing
 *    crosses the desired direction; if idle and not seated/vehicle-controlled, only sets the animation's
 *    desired turning state without touching the actual facing.
 *
 * Clean decompile. object/unit sub-record field names come from the newly-typed _object_datum/_unit_datum
 * (object_datum.h/unit_datum.h) — previously opaque byte blobs in biped_datum.h, now reused as-is since
 * both are DB types_members-confirmed. `(_cntlzw(x)&0x20)==0` / `!=0` are the established "x != 0"/"x == 0"
 * branchless idioms used elsewhere in this codebase. */

#include <math.h>

#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_animation_update_data.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum_state.h"
#include "headers/base_seat.h"
#include "headers/unit_animation_state.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"
#include "headers/game_time_constants.h"


#include "headers/real_matrix4x3.h"
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void unit_euler_aiming_update(const real_matrix4x3 *orientation, real_vector3d *aiming_vector, const real_vector3d *desired_aiming_vector, real_vector3d *aiming_velocity, const real_rectangle2d *aiming_bounds, float angular_velocity_limit, float angular_acceleration_limit);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);
extern int _cntlzw(unsigned int);

void biped_update_turning(int biped_index, unit_animation_update_data *animation)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    const biped_definition *definition = TAG_GET(const biped_definition, biped->definition_index);
    unsigned int flags = definition->biped.flags;

    if ( (flags & 4) != 0 && (biped->object.damage_flags & (1u << _object_dead_bit)) == 0 )
    {
        /* flying-camera mode */
        int nearly_stationary = 0;
        if ( (biped->object.translational_velocity.n[0] * biped->object.translational_velocity.n[0]
                    + (biped->object.translational_velocity.n[2] * biped->object.translational_velocity.n[2]
                        + biped->object.translational_velocity.n[1] * biped->object.translational_velocity.n[1]))
                < 0.00027777778f
          && (biped->object.angular_velocity.n[0] * biped->object.angular_velocity.n[0]
                    + (biped->object.angular_velocity.n[2] * biped->object.angular_velocity.n[2]
                        + biped->object.angular_velocity.n[1] * biped->object.angular_velocity.n[1]))
                < 0.0000013538552f
          && (biped->unit.throttle.n[0] * biped->unit.throttle.n[0]
                    + (biped->unit.throttle.n[2] * biped->unit.throttle.n[2]
                        + biped->unit.throttle.n[1] * biped->unit.throttle.n[1]))
                < 0.010000001f )
        {
            float stationary_turning_threshold = (biped->unit.control_flags & (1u << _unit_control_exact_facing_bit)) != 0
                    ? 0.99000001f
                    : definition->biped.runtime_cosine_stationary_turning_threshold;
            nearly_stationary = (biped->object.forward.n[0] * biped->unit.desired_facing_vector.n[0]
                        + (biped->unit.desired_facing_vector.n[2] * biped->object.forward.n[2]
                            + biped->unit.desired_facing_vector.n[1] * biped->object.forward.n[1]))
                    > stationary_turning_threshold;
        }

        real_vector3d target_facing;
        if ( nearly_stationary )
        {
            target_facing.n[0] = biped->object.forward.n[0];
            target_facing.n[1] = biped->object.forward.n[1];
            target_facing.n[2] = biped->object.forward.n[2];
        }
        else
        {
            target_facing.n[0] = biped->unit.desired_facing_vector.n[0];
            target_facing.n[1] = biped->unit.desired_facing_vector.n[1];
            target_facing.n[2] = biped->unit.desired_facing_vector.n[2];
            float pitch = definition->biped.flying_pitch_amount * biped->unit.throttle.n[2];
            if ( pitch != 0.0f )
            {
                target_facing.n[2] += pitch;
                if ( normalize3d(&target_facing) == 0.0f )
                {
                    target_facing.n[0] = biped->unit.desired_facing_vector.n[0];
                    target_facing.n[1] = biped->unit.desired_facing_vector.n[1];
                    target_facing.n[2] = biped->unit.desired_facing_vector.n[2];
                }
            }
        }

        /* banking: turn-rate signal derived from the desired-facing cross-track error against up/forward */
        float turn_rate = ((biped->object.up.n[2] * biped->object.forward.n[1] - biped->object.up.n[1] * biped->object.forward.n[2])
                    * biped->unit.desired_facing_vector.n[0]
                + (biped->unit.desired_facing_vector.n[2]
                        * (biped->object.up.n[1] * biped->object.forward.n[0] - biped->object.up.n[0] * biped->object.forward.n[1])
                    + biped->unit.desired_facing_vector.n[1]
                        * (biped->object.up.n[0] * biped->object.forward.n[2] - biped->object.up.n[2] * biped->object.forward.n[0])))
                * 3.3333333f * biped->unit.throttle.n[0] - biped->unit.throttle.n[1];
        if ( turn_rate > 1.5f )
            turn_rate = 1.5f;

        float target_bank = definition->biped.flying_bank_amount * turn_rate;
        float bank_blend;
        if ( biped->biped.bank * target_bank > 0.0f )
        {
            float bank_ratio = biped->biped.bank / target_bank;
            if ( bank_ratio > 1.0f )
                bank_ratio = 1.0f;
            bank_blend = 1.0f - bank_ratio;
        }
        else
        {
            bank_blend = 1.0f;
        }

        float bank_time = (1.0f - bank_blend) * definition->biped.flying_bank_decay_time
                + definition->biped.flying_bank_apply_time * bank_blend;
        if ( bank_time <= 0.0f )
            biped->biped.bank = target_bank;
        else
            biped->biped.bank += (target_bank - biped->biped.bank) / (bank_time * 30.0f);

        float angular_acceleration_limit = definition->biped.flying_angular_acceleration * 0.0011111111f;
        float angular_velocity_limit = definition->biped.flying_angular_velocity * SECONDS_PER_TICK;
        real_rectangle2d aiming_bounds;
        aiming_bounds.n[0] = -3.1415927f;
        aiming_bounds.n[1] = PI;
        aiming_bounds.n[2] = -1.5707964f;
        aiming_bounds.n[3] = 1.5707964f;

        if ( angular_acceleration_limit == 0.0f )
        {
            biped->object.forward = target_facing;
        }
        else
        {
            unit_euler_aiming_update(nullptr, &biped->object.forward, &target_facing, &biped->object.angular_velocity,
                    &aiming_bounds, angular_velocity_limit, angular_acceleration_limit);
        }

        biped_snap_facing(biped, definition);
        return;
    }

    /* normal ground/vehicle mode */
    int not_seated = 0;
    int is_flaming_seat = 0;  /* was mislabelled "is_gunner_seat"; value 5 is _base_seat_flaming */
    if ( biped->unit.animation.base_seat_index )
        is_flaming_seat = biped->unit.animation.base_seat_index == _base_seat_flaming;
    else
        not_seated = 1;

    if ( not_seated )
        return;

    real_vector3d turn_axis;
    float dot_current_desired;
    if ( (flags & 0x40) != 0 )
    {
        /* airborne-capable: turn direction from the up-axis cross product of forward and desired facing */
        turn_axis.n[0] = biped->object.up.n[2]
                    * (biped->object.up.n[0] * biped->unit.desired_facing_vector.n[1]
                        - biped->object.up.n[1] * biped->unit.desired_facing_vector.n[0])
                - biped->object.up.n[0]
                    * (biped->unit.desired_facing_vector.n[2] * biped->object.up.n[1]
                        - biped->unit.desired_facing_vector.n[1] * biped->object.up.n[2]);
        turn_axis.n[1] = biped->object.up.n[0]
                    * (biped->unit.desired_facing_vector.n[1] * biped->object.up.n[0]
                        - biped->object.up.n[1] * biped->unit.desired_facing_vector.n[0])
                - biped->object.up.n[2]
                    * (biped->object.up.n[2] * biped->unit.desired_facing_vector.n[0]
                        - biped->unit.desired_facing_vector.n[2] * biped->object.up.n[0]);
        turn_axis.n[2] = biped->object.up.n[1]
                    * (biped->unit.desired_facing_vector.n[2] * biped->object.up.n[1]
                        - biped->unit.desired_facing_vector.n[1] * biped->object.up.n[2])
                - biped->object.up.n[0]
                    * (biped->object.up.n[2] * biped->unit.desired_facing_vector.n[0]
                        - biped->unit.desired_facing_vector.n[2] * biped->object.up.n[0]);

        if ( normalize3d(&turn_axis) == 0.0f )
        {
            turn_axis.n[0] = biped->object.forward.n[0];
            turn_axis.n[1] = biped->object.forward.n[1];
            turn_axis.n[2] = biped->object.forward.n[2];
        }

        dot_current_desired = biped->object.forward.n[0] * turn_axis.n[2] - biped->object.forward.n[2] * turn_axis.n[0];
        dot_current_desired = biped->object.up.n[0]
                    * (biped->object.forward.n[2] * turn_axis.n[1] - biped->object.forward.n[1] * turn_axis.n[2])
                + (dot_current_desired * biped->object.up.n[1]
                    + biped->object.up.n[2]
                        * (biped->object.forward.n[1] * turn_axis.n[0] - biped->object.forward.n[0] * turn_axis.n[1]));
    }
    else
    {
        /* grounded: turn direction from the flattened 2D cross product */
        turn_axis.n[0] = biped->unit.desired_facing_vector.n[0];
        turn_axis.n[1] = biped->unit.desired_facing_vector.n[1];
        turn_axis.n[2] = 0.0f;
        if ( normalize2d((real_vector2d *)&turn_axis) == 0.0f )
        {
            turn_axis.n[0] = biped->object.forward.n[0];
            turn_axis.n[1] = biped->object.forward.n[1];
            turn_axis.n[2] = biped->object.forward.n[2];
        }
        dot_current_desired = biped->object.forward.n[1] * turn_axis.n[0] - biped->object.forward.n[0] * turn_axis.n[1];
    }

    float facing_alignment = biped->object.forward.n[0] * turn_axis.n[0] + biped->object.forward.n[1] * turn_axis.n[1];
    unsigned __int8 turn_right = dot_current_desired > 0.0f;

    if ( facing_alignment < -0.89999998f )
    {
        /* almost fully turned around: snap the turn direction based on the current animation state */
        char state = biped->unit.animation.state;
        if ( state == _unit_state_turn_right )
            turn_right = 1;
        else if ( state == _unit_state_turn_left )
            turn_right = 0;
    }

    if ( biped->biped.state == biped_state_moving || (flags & 1) != 0 )
    {
        if ( (biped->unit.control_flags & (1u << _unit_control_look_dont_turn_bit)) != 0 )
            return;

        float turn_step = definition->biped.moving_turning_speed * SECONDS_PER_TICK;
        float cos_step = (float)cos(turn_step);
        float sin_step = (float)sin(turn_step);
        if ( turn_right )
            sin_step = -sin_step;

        float progress;
        if ( (flags & 0x40) != 0 )
        {
            rotate_vector_about_axis(&biped->object.forward, &biped->object.up, sin_step, cos_step);
            progress = biped->object.up.n[0] * (turn_axis.n[2] * biped->object.forward.n[1] - turn_axis.n[1] * biped->object.forward.n[2])
                    + (biped->object.up.n[2] * (turn_axis.n[1] * biped->object.forward.n[0] - turn_axis.n[0] * biped->object.forward.n[1])
                        + biped->object.up.n[1] * (turn_axis.n[0] * biped->object.forward.n[2] - turn_axis.n[2] * biped->object.forward.n[0]));
        }
        else
        {
            float old_forward_x = biped->object.forward.n[0];
            float old_forward_y = biped->object.forward.n[1];
            biped->object.forward.n[0] = old_forward_x * cos_step - old_forward_y * sin_step;
            biped->object.forward.n[1] = old_forward_x * sin_step + old_forward_y * cos_step;
            progress = turn_axis.n[0] * biped->object.forward.n[1] - turn_axis.n[1] * biped->object.forward.n[0];
        }

        if ( turn_right )
        {
            if ( progress >= 0.0f )
                return;
        }
        else if ( progress <= 0.0f )
        {
            return;
        }

        if ( (flags & 0x40) != 0 )
        {
            real_vector3d rotation_axis;
            rotation_axis.n[0] = biped->object.up.n[1] * turn_axis.n[2] - biped->object.up.n[2] * turn_axis.n[1];
            rotation_axis.n[1] = biped->object.up.n[2] * turn_axis.n[0] - biped->object.up.n[0] * turn_axis.n[2];
            rotation_axis.n[2] = biped->object.up.n[0] * turn_axis.n[1] - biped->object.up.n[1] * turn_axis.n[0];
            if ( normalize3d(&rotation_axis) > 0.0f )
            {
                real_vector3d rotated;
                rotated.n[0] = biped->object.up.n[1] * rotation_axis.n[2] - biped->object.up.n[2] * rotation_axis.n[1];
                rotated.n[1] = biped->object.up.n[2] * rotation_axis.n[0] - biped->object.up.n[0] * rotation_axis.n[2];
                rotated.n[2] = biped->object.up.n[0] * rotation_axis.n[1] - biped->object.up.n[1] * rotation_axis.n[0];
                biped->object.forward = rotated;
            }
        }
        else
        {
            biped->object.forward.n[0] = turn_axis.n[0];
            biped->object.forward.n[1] = turn_axis.n[1];
            biped->object.forward.n[2] = 0.0f;
            biped->object.up = *global_up3d;
        }

        normalize3d(&biped->object.forward);
    }
    else if ( !biped->biped.state && !is_flaming_seat && (biped->unit.flags & (1u << _unit_aim_without_turning_bit)) == 0 )
    {
        unsigned int control_flags = biped->unit.control_flags;
        if ( (control_flags & (1u << _unit_control_look_dont_turn_bit)) == 0 )
        {
            float stationary_turning_threshold = (control_flags & (1u << _unit_control_exact_facing_bit)) != 0
                    ? 0.99000001f
                    : definition->biped.runtime_cosine_stationary_turning_threshold;
            if ( facing_alignment < stationary_turning_threshold && (definition->unit.flags & (1u << _unit_is_special)) == 0 )
                animation->state_desired = ((_cntlzw(turn_right) & 0x20) == 0) + 2;
        }
    }
}
