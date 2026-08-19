/* biped_update_turning @0x837B0F80 — per-tick facing/turning update for a biped. Two entirely different
 * modes:
 *
 *  - Flying-camera mode (_biped_flying_bit set and the biped isn't dead): a free-look "noclip"
 *    style turn that snaps straight to the desired facing when the biped is nearly stationary (velocity/
 *    angular-velocity/throttle all below tiny thresholds and the facing is already close to desired), else
 *    blends toward a throttle-tilted facing; separately updates a banking angle from the throttle-driven
 *    turn rate and applies it via unit_euler_aiming_update (or a direct facing snap when the flying
 *    angular acceleration is zero); finishes with biped_snap_facing.
 *
 *  - Normal ground/vehicle mode: computes a signed "turn direction" (which side the desired facing is on)
 *    from either the up-axis cross product (_biped_climbs_anything_bit bipeds, whose up axis need not be
 *    the world up) or a flattened 2D cross product (ordinary bipeds), snapping it to the near side when
 *    almost fully turned around; if actively moving or forced to turn (biped state 1 /
 *    _biped_turns_without_animating_bit), rotates the facing by a fixed per-tick turning speed —
 *    re-orthogonalizing it against the biped's own up axis for climbs-anything bipeds, else snapping it
 *    into the world XY plane — and stops as soon as the facing crosses the desired direction; if idle and
 *    not seated/vehicle-controlled, only sets the animation's desired turning state without touching the
 *    actual facing.
 *
 * Clean decompile. object/unit sub-record field names come from the newly-typed _object_datum/_unit_datum
 * (object_datum.h/unit_datum.h) — previously opaque byte blobs in biped_datum.h, now reused as-is since
 * both are DB types_members-confirmed. */

#include <stdint.h>
#include <math.h>

#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
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
#include "headers/fused_math.h"


#include "headers/real_matrix4x3.h"
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void unit_euler_aiming_update(const real_matrix4x3 *orientation, real_vector3d *aiming_vector, const real_vector3d *desired_aiming_vector, real_vector3d *aiming_velocity, const real_rectangle2d *aiming_bounds, float angular_velocity_limit, float angular_acceleration_limit);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);

void biped_update_turning(int biped_index, unit_animation_update_data *animation)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    const biped_definition *definition = TAG_GET(const biped_definition, biped->definition_index);
    unsigned int flags = definition->biped.flags;

    if ( (flags & (1u << _biped_flying_bit)) != 0 && (biped->object.damage_flags & (1u << _object_dead_bit)) == 0 )
    {
        /* flying-camera mode */
        int nearly_stationary = 0;
        /* DEVIATION: each magnitude accumulates z*z + (x*x + y*y) through fmadds pairs
         * (@0x837B1010/0x837B1014, @0x837B1038/0x837B103C, @0x837B1060/0x837B1064; the y*y seed
         * is the plain fmuls) — previous x + (z + y) grouping was mis-ordered and unfused. */
        if ( fused_madd(biped->object.translational_velocity.n[2], biped->object.translational_velocity.n[2],
                    fused_madd(biped->object.translational_velocity.n[0], biped->object.translational_velocity.n[0],
                        biped->object.translational_velocity.n[1] * biped->object.translational_velocity.n[1]))
                < 0.00027777778f
          && fused_madd(biped->object.angular_velocity.n[2], biped->object.angular_velocity.n[2],
                    fused_madd(biped->object.angular_velocity.n[0], biped->object.angular_velocity.n[0],
                        biped->object.angular_velocity.n[1] * biped->object.angular_velocity.n[1]))
                < 0.0000013538552f
          && fused_madd(biped->unit.throttle.n[2], biped->unit.throttle.n[2],
                    fused_madd(biped->unit.throttle.n[0], biped->unit.throttle.n[0],
                        biped->unit.throttle.n[1] * biped->unit.throttle.n[1]))
                < 0.010000001f )
        {
            float stationary_turning_threshold = (biped->unit.control_flags & (1u << _unit_control_exact_facing_bit)) != 0
                    ? 0.99000001f
                    : definition->biped.runtime_cosine_stationary_turning_threshold;
            /* DEVIATION: dot accumulates through fmadds @0x837B10B0/0x837B10B4 (dy*fy seed is the
             * plain fmuls @0x837B109C). */
            nearly_stationary = fused_madd(biped->object.forward.n[0], biped->unit.desired_facing_vector.n[0],
                        fused_madd(biped->unit.desired_facing_vector.n[2], biped->object.forward.n[2],
                            biped->unit.desired_facing_vector.n[1] * biped->object.forward.n[1]))
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
        /* DEVIATION: cross terms are fmsubs @0x837B1194/0x837B119C/0x837B11A0, the accumulation is
         * fmadds @0x837B11A8/0x837B11AC (dy*cross_y seed plain fmuls @0x837B11A4), and the final
         * *throttle.x - throttle.y step is the fused fmsubs @0x837B11B4. */
        float turn_rate = fused_msub(
                fused_madd(fused_msub(biped->object.up.n[2], biped->object.forward.n[1],
                                biped->object.up.n[1] * biped->object.forward.n[2]),
                        biped->unit.desired_facing_vector.n[0],
                    fused_madd(biped->unit.desired_facing_vector.n[2],
                            fused_msub(biped->object.up.n[1], biped->object.forward.n[0],
                                    biped->object.up.n[0] * biped->object.forward.n[1]),
                        biped->unit.desired_facing_vector.n[1]
                            * fused_msub(biped->object.up.n[0], biped->object.forward.n[2],
                                    biped->object.up.n[2] * biped->object.forward.n[0])))
                * 3.3333333f,
                biped->unit.throttle.n[0], biped->unit.throttle.n[1]);
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

        /* DEVIATION: fmadds @0x837B1214 fuses (1-blend)*decay onto the plain apply*blend fmuls
         * @0x837B120C. */
        float bank_time = fused_madd(1.0f - bank_blend, definition->biped.flying_bank_decay_time,
                definition->biped.flying_bank_apply_time * bank_blend);
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
    float facing_alignment;
    if ( (flags & (1u << _biped_climbs_anything_bit)) != 0 )
    {
        /* climbs-anything: turn direction from the up-axis cross product of forward and desired facing */
        /* DEVIATION: whole triple-product transcribed from the fused sequence @0x837B1378-0x837B13A0 —
         * inner crosses are fmsubs @0x837B1378/0x837B137C/0x837B1380 and each component is a fused
         * fmsubs over a plain fmuls partner (@0x837B1390/0x837B1398/0x837B13A0); the previous x/y
         * components paired the wrong inner terms (e.g. x used a.z/a.x where the binary uses a.y/a.z). */
        {
            float cross_ax = fused_msub(biped->unit.desired_facing_vector.n[2], biped->object.up.n[1],
                    biped->unit.desired_facing_vector.n[1] * biped->object.up.n[2]);
            float cross_ay = fused_msub(biped->object.up.n[2], biped->unit.desired_facing_vector.n[0],
                    biped->unit.desired_facing_vector.n[2] * biped->object.up.n[0]);
            float cross_az = fused_msub(biped->unit.desired_facing_vector.n[1], biped->object.up.n[0],
                    biped->object.up.n[1] * biped->unit.desired_facing_vector.n[0]);
            turn_axis.n[0] = fused_msub(biped->object.up.n[2], cross_ay, biped->object.up.n[1] * cross_az);
            turn_axis.n[1] = fused_msub(biped->object.up.n[0], cross_az, biped->object.up.n[2] * cross_ax);
            turn_axis.n[2] = fused_msub(biped->object.up.n[1], cross_ax, biped->object.up.n[0] * cross_ay);
        }

        if ( normalize3d(&turn_axis) == 0.0f )
        {
            turn_axis.n[0] = biped->object.forward.n[0];
            turn_axis.n[1] = biped->object.forward.n[1];
            turn_axis.n[2] = biped->object.forward.n[2];
        }

        /* DEVIATION: error crosses are fmsubs @0x837B1410/0x837B1414/0x837B1418 and the accumulation
         * fmadds @0x837B1428/0x837B142C (up.z partner plain fmuls @0x837B1420); facing_alignment in
         * this branch is the fused 3D dot fmadds @0x837B141C/0x837B1424 — the previously shared 2D
         * dot dropped the axis.z*forward.z term here. */
        {
            float error_x = fused_msub(biped->object.forward.n[2], turn_axis.n[1],
                    biped->object.forward.n[1] * turn_axis.n[2]);
            float error_y = fused_msub(biped->object.forward.n[0], turn_axis.n[2],
                    biped->object.forward.n[2] * turn_axis.n[0]);
            float error_z = fused_msub(biped->object.forward.n[1], turn_axis.n[0],
                    biped->object.forward.n[0] * turn_axis.n[1]);
            dot_current_desired = fused_madd(biped->object.up.n[0], error_x,
                    fused_madd(error_y, biped->object.up.n[1], biped->object.up.n[2] * error_z));
            facing_alignment = fused_madd(turn_axis.n[2], biped->object.forward.n[2],
                    fused_madd(biped->object.forward.n[0], turn_axis.n[0],
                            turn_axis.n[1] * biped->object.forward.n[1]));
        }
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
        /* DEVIATION: fmsubs @0x837B149C (fwd.x*axis.y minuend plain fmuls @0x837B148C) and the 2D
         * alignment dot fmadds @0x837B14A0 (fwd.y*axis.y plain fmuls @0x837B1490). */
        dot_current_desired = fused_msub(biped->object.forward.n[1], turn_axis.n[0],
                biped->object.forward.n[0] * turn_axis.n[1]);
        facing_alignment = fused_madd(biped->object.forward.n[0], turn_axis.n[0],
                biped->object.forward.n[1] * turn_axis.n[1]);
    }

    uint8_t turn_right = dot_current_desired > 0.0f;

    if ( facing_alignment < -0.89999998f )
    {
        /* almost fully turned around: snap the turn direction based on the current animation state */
        char state = biped->unit.animation.state;
        if ( state == _unit_state_turn_right )
            turn_right = 1;
        else if ( state == _unit_state_turn_left )
            turn_right = 0;
    }

    if ( biped->biped.state == biped_state_moving || (flags & (1u << _biped_turns_without_animating_bit)) != 0 )
    {
        if ( (biped->unit.control_flags & (1u << _unit_control_look_dont_turn_bit)) != 0 )
            return;

        float turn_step = definition->biped.moving_turning_speed * SECONDS_PER_TICK;
        float cos_step = (float)cos(turn_step);
        float sin_step = (float)sin(turn_step);
        if ( turn_right )
            sin_step = -sin_step;

        float progress;
        if ( (flags & (1u << _biped_climbs_anything_bit)) != 0 )
        {
            rotate_vector_about_axis(&biped->object.forward, &biped->object.up, sin_step, cos_step);
            /* DEVIATION: transcribed from fmsubs @0x837B162C/0x837B1630/0x837B1634 + fmadds
             * @0x837B163C/0x837B1640 — each inner term is forward-cross-axis (fwd.z*axis.y - fwd.y*axis.z,
             * etc.); the previous version had every term negated (axis-cross-forward), inverting the
             * turn-completion sign test. */
            progress = fused_madd(biped->object.up.n[0],
                    fused_msub(biped->object.forward.n[2], turn_axis.n[1], biped->object.forward.n[1] * turn_axis.n[2]),
                    fused_madd(biped->object.up.n[2],
                        fused_msub(biped->object.forward.n[1], turn_axis.n[0], biped->object.forward.n[0] * turn_axis.n[1]),
                        biped->object.up.n[1]
                            * fused_msub(biped->object.forward.n[0], turn_axis.n[2], biped->object.forward.n[2] * turn_axis.n[0])));
        }
        else
        {
            float old_forward_x = biped->object.forward.n[0];
            float old_forward_y = biped->object.forward.n[1];
            /* DEVIATION: the 2D rotation is fmsubs @0x837B1658 / fmadds @0x837B1660 (fwd.y*sin and
             * fwd.y*cos partners are plain fmuls @0x837B164C/0x837B1654), and progress is the fused
             * fmsubs @0x837B1674 over the plain new_x*axis.y fmuls @0x837B1668. */
            biped->object.forward.n[0] = fused_msub(old_forward_x, cos_step, old_forward_y * sin_step);
            biped->object.forward.n[1] = fused_madd(old_forward_x, sin_step, old_forward_y * cos_step);
            progress = fused_msub(turn_axis.n[0], biped->object.forward.n[1],
                    biped->object.forward.n[0] * turn_axis.n[1]);
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

        if ( (flags & (1u << _biped_climbs_anything_bit)) != 0 )
        {
            real_vector3d rotation_axis;
            /* DEVIATION: cross is fmsubs @0x837B16C8/0x837B16D0/0x837B16D8 with plain fmuls minuends
             * (@0x837B16B8/0x837B16C0/0x837B16C4). */
            rotation_axis.n[0] = fused_msub(biped->object.up.n[1], turn_axis.n[2], biped->object.up.n[2] * turn_axis.n[1]);
            rotation_axis.n[1] = fused_msub(biped->object.up.n[2], turn_axis.n[0], biped->object.up.n[0] * turn_axis.n[2]);
            rotation_axis.n[2] = fused_msub(biped->object.up.n[0], turn_axis.n[1], biped->object.up.n[1] * turn_axis.n[0]);
            if ( normalize3d(&rotation_axis) > 0.0f )
            {
                real_vector3d rotated;
                /* DEVIATION: fmsubs @0x837B170C/0x837B1714/0x837B171C compute rotation_axis x up
                 * (fwd.x = up.z*axis.y - up.y*axis.z, etc.) — the previous up x rotation_axis order
                 * negated all three components. */
                rotated.n[0] = fused_msub(biped->object.up.n[2], rotation_axis.n[1], biped->object.up.n[1] * rotation_axis.n[2]);
                rotated.n[1] = fused_msub(biped->object.up.n[0], rotation_axis.n[2], biped->object.up.n[2] * rotation_axis.n[0]);
                rotated.n[2] = fused_msub(biped->object.up.n[1], rotation_axis.n[0], biped->object.up.n[0] * rotation_axis.n[1]);
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
                animation->state_desired = turn_right ? _unit_state_turn_right : _unit_state_turn_left;
        }
    }
}
