/* biped_update_physics @0x837AEF08 — per-frame movement/physics integration for one biped.
 *
 * Consumes the input half of a biped_physics scratch block (current state, desired movement,
 * terrain-response constants, locomotion-mode flags) that biped_update_moving[_simulated] filled,
 * and writes the output half: the new position/velocity, the resolved support/elevator/bumped
 * surfaces, and the landing velocity.
 *
 * Structure:
 *   1. Compute the frame's desired velocity into physics->new_velocity, branching on the
 *      locomotion mode in in_flags: 0x10 flying, 0x20 facing-relative (scripted) velocity,
 *      0x01 airborne (gravity), else grounded walking (with a slope/ground-plane sub-path).
 *   2. Sweep a collision "pill" (capsule) from the current position along that velocity via
 *      collision_move_pill, collecting up to 16 contacts.
 *   3. If the sweep hit nothing but the biped had a support surface, walk that surface's edge
 *      loop looking for an adjacent walkable surface to adhere to (step-down / ground adhesion),
 *      synthesizing a single contact when one is found.
 *   4. Scan the contacts to pick the support surface, the "stuck" surface, the elevator object,
 *      and the bumped object, then write the resolved output fields.
 *
 * CAVEAT: the decompiler badly mis-shifted every call that passes float args (height/width/radius
 * occupy FPRs f1.. and *reserve* the matching GPR slots on this ABI), fabricating a 60-wide float
 * parameter list and misassigning the trailing integer/pointer args of collision_move_pill and
 * collision_get_features_in_sphere. The call arguments below were re-derived from the disassembly
 * (0x837AF6EC-0x837AF758, 0x837B0288-0x837B02B4, 0x837B02C4-0x837B030C) and the callee prototypes,
 * not from the pseudocode. The dense single-precision (float) round chains are reproduced verbatim.
 */

#include <stdint.h>
#include "headers/biped_physics.h"
#include "headers/biped_physics_in_flags.h"
#include "headers/biped_physics_out_flags.h"
#include "headers/collision_surface_flags.h"
#include "headers/collision_plane.h"
#include "headers/collision_bsp.h"
#include "headers/collision_feature_list.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_plane3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/machine_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/machine_datum_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include "headers/fused_math.h"

/* ---- extern boundaries (siblings / engine primitives — declared, not reversed here) ---- */
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern void biped_build_flying_axes(const real_vector3d *forward_vector, real_vector3d *left_vector, real_vector3d *up_vector);
extern int16_t collision_move_pill(unsigned int flags, const real_point3d *old_position, const real_vector3d *old_velocity, float height, float width, int ignore_object_index, real_point3d *new_position, real_vector3d *new_velocity, int16_t maximum_collision_count, collision_plane *collisions);
extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern uint8_t collision_features_test_vector(const collision_feature_list *features, const real_point3d *point, const real_vector3d *vector, collision_plane *collision);
extern real_plane3d *bsp3d_get_plane_from_designator(const bsp3d *bsp, int plane_designator, real_plane3d *result);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern float  __fsqrts(float x);
extern double __fabs(double x);


void biped_update_physics(biped_physics *physics)
{
    uint16_t in_flags = physics->in_flags;
    int              grounded = (in_flags >> _biped_physics_in_climb_anything_bit) & 1;   /* bit 9 */
    float            horizontal_x = 0.0f;              /* facing direction the pick-loop later uses */
    float            horizontal_y = 0.0f;

    real_vector3d    accel;                            /* velocity change, clamped to accel max */
    real_vector3d    move_direction;                   /* desired unit movement direction */
    real_vector3d   *new_velocity = &physics->new_velocity;
    float            speed_scale;

    physics->out_flags = 0;

    if ((in_flags & (1u << _biped_physics_in_flying_bit)) != 0)
    {
        /* ---- flying: rotate desired movement into a flying (left, up, forward) basis ---- */
        real_vector3d flying_left, flying_up;
        float movement_penalty_inv;
        float desired_x, desired_y, desired_z;
        float clamped_x, clamped_y, clamped_z;
        float accel_magnitude;

        biped_build_flying_axes(&physics->forward, &flying_left, &flying_up);

        /* DEVIATION: each basis dot accumulates through fmadds pairs (@0x837AEFC0/0x837AEFD4,
         * @0x837AEFC8/0x837AEFE4, @0x837AEFEC/0x837AEFFC) over a plain fmuls seed; the penalty
         * scale and velocity subtraction stay plain (fmuls/fsubs @0x837AEFE8-0x837AF014). */
        movement_penalty_inv = 1.0f - physics->movement_penalty;
        desired_z = fused_madd(physics->movement_desired.__s1.k, flying_up.__s1.k,
                  fused_madd(physics->forward.__s1.k, physics->movement_desired.__s1.i,
                          physics->movement_desired.__s1.j * flying_left.__s1.k));
        desired_x = fused_madd(physics->movement_desired.__s1.j, flying_left.__s1.i,
                          physics->movement_desired.__s1.k * flying_up.__s1.i);

        accel.__s1.j = (movement_penalty_inv
                        * fused_madd(physics->movement_desired.__s1.k, flying_up.__s1.j,
                                fused_madd(physics->forward.__s1.j, physics->movement_desired.__s1.i,
                                        physics->movement_desired.__s1.j * flying_left.__s1.j)))
                - physics->velocity.__s1.j;
        accel.__s1.k = (movement_penalty_inv * desired_z) - physics->velocity.__s1.k;
        accel.__s1.i = (movement_penalty_inv
                        * fused_madd(physics->movement_desired.__s1.i, physics->forward.__s1.i,
                                desired_x))
                - physics->velocity.__s1.i;

        clamped_y = accel.__s1.j;
        clamped_x = accel.__s1.i;
        clamped_z = accel.__s1.k;
        accel_magnitude = normalize3d(&accel);
        if (accel_magnitude <= physics->acceleration_maximum)
        {
            /* magnitude within budget — normalize3d already left the unit vector, so restore raw */
            accel.__s1.i = clamped_x;
            accel.__s1.j = clamped_y;
            accel.__s1.k = clamped_z;
        }
        else
        {
            clamped_x = (accel.__s1.i * physics->acceleration_maximum);
            clamped_y = (accel.__s1.j * physics->acceleration_maximum);
            clamped_z = (accel.__s1.k * physics->acceleration_maximum);
        }

        new_velocity->__s1.i = clamped_x + physics->velocity.__s1.i;
        new_velocity->__s1.j = clamped_y + physics->velocity.__s1.j;
        new_velocity->__s1.k = clamped_z + physics->velocity.__s1.k;
        physics->out_flags = (physics->out_flags & ~((1u << _biped_physics_out_airborne_bit) | (1u << _biped_physics_out_slipping_bit))) | (1u << _biped_physics_out_airborne_bit);
    }
    else if ((in_flags & (1u << _biped_physics_in_absolute_movement_bit)) != 0)
    {
        /* ---- facing-relative (scripted) velocity: rotate movement_desired by the facing yaw ---- */
        new_velocity->__s1.k = physics->movement_desired.__s1.k;
        /* DEVIATION: yaw rotation is fmadds @0x837AF0D0 / fmsubs @0x837AF0D8 over plain fmuls
         * partners (@0x837AF0BC/0x837AF0C0). */
        new_velocity->__s1.j = fused_madd(physics->forward.__s1.j, physics->movement_desired.__s1.i,
                                physics->movement_desired.__s1.j * physics->forward.__s1.i);
        new_velocity->__s1.i = fused_msub(physics->forward.__s1.i, physics->movement_desired.__s1.i,
                                physics->forward.__s1.j * physics->movement_desired.__s1.j);
        horizontal_y = new_velocity->__s1.j;
        horizontal_x = new_velocity->__s1.i;
    }
    else if ((in_flags & (1u << _biped_physics_in_airborne_bit)) != 0)
    {
        /* ---- airborne: horizontal accel clamped to the airborne budget, plus gravity ---- */
        real_vector2d horizontal_accel;
        float movement_penalty_inv = 1.0f - physics->movement_penalty;
        float clamped_x, clamped_y;

        /* DEVIATION: yaw rotation is fmsubs @0x837AF12C / fmadds @0x837AF128 over plain fmuls
         * partners (@0x837AF108/0x837AF10C); penalty scale and velocity subtraction stay plain. */
        horizontal_accel.__s1.i = ((movement_penalty_inv
                                   * fused_msub(physics->forward.__s1.i, physics->movement_desired.__s1.i,
                                           physics->forward.__s1.j * physics->movement_desired.__s1.j))
                                   - physics->velocity.__s1.i);
        horizontal_accel.__s1.j = ((movement_penalty_inv
                                   * fused_madd(physics->forward.__s1.j, physics->movement_desired.__s1.i,
                                           physics->movement_desired.__s1.j * physics->forward.__s1.i))
                                   - physics->velocity.__s1.j);
        clamped_x = horizontal_accel.__s1.i;
        clamped_y = horizontal_accel.__s1.j;
        if (normalize2d(&horizontal_accel) <= physics->airborne_acceleration_maximum)
        {
            /* magnitude within budget — normalize2d already left the unit vector, so restore raw.
             * DEVIATION: the binary saves the (i,j) pair as one fused 64-bit slot (ld r29 @0x837AF148)
             * and writes it back on this arm (std r29 @0x837AF170); the decompiler dropped the fused
             * save/restore, so this arm was reading the normalized vector. The flying and grounded
             * branches save the same way with separate lwz/stw (@0x837AF054, @0x837AF604). */
            horizontal_accel.__s1.i = clamped_x;
            horizontal_accel.__s1.j = clamped_y;
        }
        else
        {
            clamped_x = (horizontal_accel.__s1.i * physics->airborne_acceleration_maximum);
            clamped_y = (horizontal_accel.__s1.j * physics->airborne_acceleration_maximum);
        }
        new_velocity->__s1.j = physics->velocity.__s1.j + clamped_y;
        new_velocity->__s1.i = physics->velocity.__s1.i + clamped_x;
        new_velocity->__s1.k = physics->velocity.__s1.k - global_gravity;
        physics->out_flags = in_flags & (1u << _biped_physics_in_slipping_bit);  /* in slipping (bit 1) maps onto out slipping (bit 1) */
    }
    else
    {
        /* ---- grounded walking ---- */
        const real_vector3d *ground_normal = &physics->ground_plane.n;
        int airborne = 0;

        /* DEVIATION: magnitude accumulates through fmadds @0x837AF1C8/0x837AF1CC (j*j seed plain
         * fmuls @0x837AF1B8). */
        speed_scale = __fsqrts(fused_madd(physics->movement_desired.__s1.k, physics->movement_desired.__s1.k,
                             fused_madd(physics->movement_desired.__s1.i, physics->movement_desired.__s1.i,
                                     physics->movement_desired.__s1.j * physics->movement_desired.__s1.j)));

        if ((in_flags & (1u << _biped_physics_in_climb_anything_bit)) != 0)
        {
            /* on a resolved ground plane: build a tangent basis from the aiming vector and the
             * ground normal, then express the desired movement in it */
            real_vector3d tangent, bitangent;

            /* DEVIATION: cross is fmsubs @0x837AF218/0x837AF224/0x837AF22C with plain fmuls
             * minuends (@0x837AF208/0x837AF210/0x837AF220). */
            tangent.__s1.i = fused_msub(physics->ground_plane.n.__s1.j, physics->aiming.__s1.k,
                      physics->ground_plane.n.__s1.k * physics->aiming.__s1.j);
            tangent.__s1.j = fused_msub(physics->ground_plane.n.__s1.k, physics->aiming.__s1.i,
                      physics->ground_plane.n.__s1.i * physics->aiming.__s1.k);
            tangent.__s1.k = fused_msub(physics->ground_plane.n.__s1.i, physics->aiming.__s1.j,
                      physics->ground_plane.n.__s1.j * physics->aiming.__s1.i);
            if (normalize3d(&tangent) == 0.0f)
            {
                /* aiming parallel to the ground normal — fall back to global up, then forward */
                /* DEVIATION: cross is fmsubs @0x837AF270/0x837AF268/0x837AF278 with plain fmuls
                 * minuends (@0x837AF25C/0x837AF258/0x837AF264). */
                tangent.__s1.i = fused_msub(global_up3d->n[2], physics->ground_plane.n.__s1.j,
                          global_up3d->n[1] * physics->ground_plane.n.__s1.k);
                tangent.__s1.j = fused_msub(global_up3d->n[0], physics->ground_plane.n.__s1.k,
                          global_up3d->n[2] * physics->ground_plane.n.__s1.i);
                tangent.__s1.k = fused_msub(global_up3d->n[1], physics->ground_plane.n.__s1.i,
                          global_up3d->n[0] * physics->ground_plane.n.__s1.j);
                if (normalize3d(&tangent) == 0.0f)
                {
                    /* DEVIATION: cross is fmsubs @0x837AF2C0/0x837AF2B8/0x837AF2C8 with plain fmuls
                     * minuends (@0x837AF2AC/0x837AF2A8/0x837AF2B4). */
                    tangent.__s1.i = fused_msub(global_forward3d->n[2], physics->ground_plane.n.__s1.j,
                              global_forward3d->n[1] * physics->ground_plane.n.__s1.k);
                    tangent.__s1.j = fused_msub(global_forward3d->n[0], physics->ground_plane.n.__s1.k,
                              global_forward3d->n[2] * physics->ground_plane.n.__s1.i);
                    tangent.__s1.k = fused_msub(global_forward3d->n[1], physics->ground_plane.n.__s1.i,
                              global_forward3d->n[0] * physics->ground_plane.n.__s1.j);
                    normalize3d(&tangent);
                }
            }

            /* DEVIATION: fmsubs @0x837AF2F8/0x837AF300/0x837AF308 compute tangent x ground-normal
             * (bitangent.i = gp.k*t.j - gp.j*t.k, etc.) — the previous ground-normal x tangent order
             * negated all three components; minuends are the plain fmuls @0x837AF2E0-0x837AF2F4. */
            bitangent.__s1.i = fused_msub(physics->ground_plane.n.__s1.k, tangent.__s1.j,
                        physics->ground_plane.n.__s1.j * tangent.__s1.k);
            bitangent.__s1.j = fused_msub(physics->ground_plane.n.__s1.i, tangent.__s1.k,
                        physics->ground_plane.n.__s1.k * tangent.__s1.i);
            bitangent.__s1.k = fused_msub(physics->ground_plane.n.__s1.j, tangent.__s1.i,
                        physics->ground_plane.n.__s1.i * tangent.__s1.j);
            normalize3d(&bitangent);

            /* DEVIATION: fmadds @0x837AF33C/0x837AF344/0x837AF338 over plain md.i*bitangent fmuls
             * seeds (@0x837AF328/0x837AF330/0x837AF31C); the trailing +md.k is the plain fadds
             * @0x837AF34C. */
            move_direction.__s1.i = fused_madd(physics->movement_desired.__s1.j, tangent.__s1.i,
                             physics->movement_desired.__s1.i * bitangent.__s1.i);
            move_direction.__s1.j = fused_madd(physics->movement_desired.__s1.j, tangent.__s1.j,
                             physics->movement_desired.__s1.i * bitangent.__s1.j);
            move_direction.__s1.k = fused_madd(physics->movement_desired.__s1.j, tangent.__s1.k,
                                     physics->movement_desired.__s1.i * bitangent.__s1.k)
                             + physics->movement_desired.__s1.k;
        }
        else if (physics->ground_plane.n.__s1.k <= 0.000099999997f)
        {
            /* no usable ground plane — build a tangent from aiming x global-up */
            real_vector3d tangent;
            float bitangent_x, bitangent_y, bitangent_z;
            float aiming_i = physics->aiming.__s1.i;
            float aiming_j = physics->aiming.__s1.j;
            float aiming_k = physics->aiming.__s1.k;

            /* DEVIATION: cross is fmsubs @0x837AF408/0x837AF410/0x837AF418 with plain fmuls
             * minuends (@0x837AF3EC/0x837AF3F0/0x837AF3FC). */
            tangent.__s1.k = fused_msub(aiming_j, global_up3d->n[0], aiming_i * global_up3d->n[1]);
            tangent.__s1.i = fused_msub(aiming_k, global_up3d->n[1], aiming_j * global_up3d->n[2]);
            tangent.__s1.j = fused_msub(aiming_i, global_up3d->n[2], aiming_k * global_up3d->n[0]);
            normalize3d(&tangent);
            ground_normal = &physics->ground_plane.n;

            /* DEVIATION: fmsubs @0x837AF480 / fmadds @0x837AF48C over plain fmuls partners
             * (@0x837AF45C/0x837AF47C). */
            horizontal_x = fused_msub(physics->forward.__s1.i, physics->movement_desired.__s1.i,
                                 physics->forward.__s1.j * physics->movement_desired.__s1.j);
            horizontal_y = fused_madd(physics->forward.__s1.j, physics->movement_desired.__s1.i,
                                 physics->movement_desired.__s1.j * physics->forward.__s1.i);

            /* project the tangent and the aiming vector onto the ground-normal plane, then blend
             * them by the desired movement.
             * DEVIATION: transcribed from the fused sequence @0x837AF430-0x837AF4C8 — the plane
             * dots finish in fnmadds (@0x837AF488/0x837AF484, spelled fused_nmsub with a negated
             * addend), every projection/blend step is fmadds, and the blend weights are
             * movement_desired.j/.i with a trailing +movement_desired.k (fadds @0x837AF4C8); the
             * previous version weighted by forward.k/forward.i and added forward.k. */
            {
                float neg_dot_tangent = fused_nmsub(physics->ground_plane.n.__s1.i, tangent.__s1.i,
                        -fused_madd(physics->ground_plane.n.__s1.j, tangent.__s1.j,
                                physics->ground_plane.n.__s1.k * tangent.__s1.k));
                float neg_dot_aiming = fused_nmsub(physics->ground_plane.n.__s1.i, aiming_i,
                        -fused_madd(physics->ground_plane.n.__s1.j, aiming_j,
                                physics->ground_plane.n.__s1.k * aiming_k));
                float tangent_proj_i = fused_madd(neg_dot_tangent, physics->ground_plane.n.__s1.i, tangent.__s1.i);
                float tangent_proj_j = fused_madd(neg_dot_tangent, physics->ground_plane.n.__s1.j, tangent.__s1.j);
                float tangent_proj_k = fused_madd(neg_dot_tangent, physics->ground_plane.n.__s1.k, tangent.__s1.k);
                float aiming_proj_i = fused_madd(neg_dot_aiming, physics->ground_plane.n.__s1.i, aiming_i);
                float aiming_proj_j = fused_madd(neg_dot_aiming, physics->ground_plane.n.__s1.j, aiming_j);
                float aiming_proj_k = fused_madd(neg_dot_aiming, physics->ground_plane.n.__s1.k, aiming_k);
                bitangent_x = fused_madd(physics->movement_desired.__s1.j, tangent_proj_i,
                        physics->movement_desired.__s1.i * aiming_proj_i);
                bitangent_y = fused_madd(physics->movement_desired.__s1.j, tangent_proj_j,
                        physics->movement_desired.__s1.i * aiming_proj_j);
                bitangent_z = fused_madd(physics->movement_desired.__s1.j, tangent_proj_k,
                        physics->movement_desired.__s1.i * aiming_proj_k);
            }

            move_direction.__s1.i = bitangent_x;
            move_direction.__s1.j = bitangent_y;
            move_direction.__s1.k = bitangent_z + physics->movement_desired.__s1.k;
            if (((in_flags >> _biped_physics_in_climb_anything_bit) & 1) == 0)
                move_direction.__s1.k = (bitangent_z + physics->movement_desired.__s1.k) * 5.0f;
        }
        else
        {
            /* on a slope: rotate the desired movement by the facing yaw, then keep it in-plane */
            ground_normal = &physics->ground_plane.n;
            /* DEVIATION: fmsubs @0x837AF394 / fmadds @0x837AF39C over plain fmuls partners
             * (@0x837AF378/0x837AF384); the in-plane correction dot is fmadds @0x837AF3B0 over
             * the plain gp.i*hx fmuls @0x837AF3A4. */
            horizontal_x = fused_msub(physics->forward.__s1.i, physics->movement_desired.__s1.i,
                                 physics->forward.__s1.j * physics->movement_desired.__s1.j);
            horizontal_y = fused_madd(physics->forward.__s1.j, physics->movement_desired.__s1.i,
                                 physics->movement_desired.__s1.j * physics->forward.__s1.i);
            move_direction.__s1.j = horizontal_y;
            move_direction.__s1.i = horizontal_x;
            move_direction.__s1.k = physics->movement_desired.__s1.k
                - (fused_madd(physics->ground_plane.n.__s1.j, horizontal_y,
                                physics->ground_plane.n.__s1.i * horizontal_x)
                        / physics->ground_plane.n.__s1.k);
        }

        normalize3d(&move_direction);

        /* slope-dependent speed scaling from the downhill/uphill response curve */
        if ((in_flags & (1u << _biped_physics_in_climb_anything_bit)) == 0)
        {
            float slope = move_direction.__s1.k;
            if (slope > physics->downhill_k1)
            {
                if (slope >= physics->downhill_k0)
                {
                    float curve_hi, curve_lo, curve_scale;
                    if (slope >= physics->uphill_k1)
                    {
                        speed_scale = (physics->uphill_velocity_scale * speed_scale);
                        goto integrate_velocity;
                    }
                    curve_hi = physics->uphill_k1;
                    if (slope <= physics->uphill_k0)
                        goto integrate_velocity;
                    curve_lo = physics->uphill_k0;
                    curve_scale = physics->uphill_velocity_scale;
                    speed_scale = (((((move_direction.__s1.k - curve_lo)
                                                   * (curve_scale - 1.0f))
                                           / (curve_hi - curve_lo))
                                   + 1.0f)
                           * speed_scale);
                    goto integrate_velocity;
                }
                else
                {
                    float curve_hi = physics->downhill_k0;
                    float curve_lo = physics->downhill_k1;
                    float curve_scale = physics->downhill_velocity_scale;
                    speed_scale = (((((move_direction.__s1.k - curve_lo)
                                                   * (curve_scale - 1.0f))
                                           / (curve_hi - curve_lo))
                                   + 1.0f)
                           * speed_scale);
                    goto integrate_velocity;
                }
            }
            speed_scale = (physics->downhill_velocity_scale * speed_scale);
        }

    integrate_velocity:
        {
            float target_scale = ((1.0f - physics->movement_penalty) * speed_scale);
            float desired_x = (move_direction.__s1.i * target_scale);
            float clamped_x, clamped_y, clamped_z;
            float ground_nudge;

            accel.__s1.j = (move_direction.__s1.j * target_scale) - physics->velocity.__s1.j;
            accel.__s1.k = (move_direction.__s1.k * target_scale) - physics->velocity.__s1.k;
            accel.__s1.i = desired_x - physics->velocity.__s1.i;

            clamped_y = accel.__s1.j;
            clamped_x = accel.__s1.i;
            clamped_z = accel.__s1.k;
            if (normalize3d(&accel) <= physics->acceleration_maximum)
            {
                accel.__s1.i = clamped_x;
                accel.__s1.j = clamped_y;
                accel.__s1.k = clamped_z;
            }
            else
            {
                if ((in_flags & (1u << _biped_physics_in_climb_anything_bit)) == 0)
                    airborne = (in_flags >> _biped_physics_in_slipping_bit) & 1;
                clamped_y = (accel.__s1.j * physics->acceleration_maximum);
                clamped_x = (accel.__s1.i * physics->acceleration_maximum);
                clamped_z = (accel.__s1.k * physics->acceleration_maximum);
            }

            /* small (1/128) push along the ground normal keeps the pill seated on the surface */
            /* DEVIATION: each nudge is a fused fnmsubs @0x837AF648/0x837AF650/0x837AF654
             * (clamped - normal/128 in one rounding); the +velocity is the plain fadds. */
            ground_nudge = fused_nmsub(ground_normal->n[0], 0.0078125f, clamped_x);
            physics->out_flags = ((uint8_t)airborne == 0) ? 0 : (1u << _biped_physics_out_slipping_bit);
            physics->new_velocity.__s1.i = ground_nudge + physics->velocity.__s1.i;
            physics->new_velocity.__s1.j = physics->velocity.__s1.j
                + fused_nmsub(ground_normal->n[1], 0.0078125f, clamped_y);
            physics->new_velocity.__s1.k = physics->velocity.__s1.k
                + fused_nmsub(ground_normal->n[2], 0.0078125f, clamped_z);
            if ((physics->out_flags & (1u << _biped_physics_out_slipping_bit)) != 0)
                physics->new_velocity.__s1.k = physics->new_velocity.__s1.k - global_gravity;
        }
    }

    /* ===================== common tail: sweep the pill and resolve contacts ===================== */
    {
        unsigned int    collision_flags;
        real_point3d    start_position;
        real_vector3d   start_velocity;
        real_point3d    moved_position;    /* collision_move_pill output -> becomes new_position */
        real_vector3d   moved_velocity;    /* collision_move_pill output -> becomes new_velocity */
        collision_plane collisions[16];
        int             collision_count;
        int             support_surface_index;
        int             chosen_surface_index = -1;
        int             stuck_object_valid = 0;
        int             bump_object = -1;
        int             have_walkable = 0;
        char            have_facing = 0;
        float           best_normal_dot = -3.4028235e38f;
        float           best_normal_k = -3.4028235e38f;

        /* DB carries these composite query words verbatim ($7278357605E8DB656D8B016C81954930);
         * the previous comment mislabeled 0xC2A0 as "adds vehicles" — it is the passthrough-living mask. */
        uint16_t flags = physics->in_flags;
        if ((flags & (1u << _biped_physics_in_no_collision_bit)) != 0)
            collision_flags = 0;
        else if ((flags & (1u << _biped_physics_in_dead_bit)) != 0)
            collision_flags = _collision_test_for_bipeds_dead_flags;
        else if ((physics->in_flags & (1u << _biped_physics_in_pass_through_bipeds_bit)) != 0)
            collision_flags = _collision_test_for_bipeds_passthrough_living_flags;
        else
            collision_flags = _collision_test_for_bipeds_living_flags;

        start_position = physics->position;
        start_velocity = physics->new_velocity;
        start_velocity.__s1.k = start_velocity.__s1.k + physics->crouch_velocity;

        collision_count = collision_move_pill(collision_flags, &start_position, &start_velocity,
                                              physics->height, physics->width, physics->biped_index,
                                              &moved_position, &moved_velocity, 16, collisions);

        if (collision_count < 16)
            physics->out_flags &= ~(1u << _biped_physics_out_splatter_bit);
        else
            physics->out_flags |= (1u << _biped_physics_out_splatter_bit);
        physics->stick_surface_index = -1;

        support_surface_index = physics->existing_support_surface_index;
        if (collision_count
            || support_surface_index == -1
            || support_surface_index < 0
            || support_surface_index >= global_collision_bsp->surfaces.count)
        {
            /* the pill hit something (or there is no prior support) - no adhesion search */
        }
        else
        {
            /* ---- ground adhesion: walk the support surface edge loop for a walkable neighbour ---- */
            collision_bsp     *bsp = global_collision_bsp;
            char              *surface_table = (char *)bsp->surfaces.address;
            collision_surface *support_surface =
                &((collision_surface *)surface_table)[support_surface_index]; /* recovered: &surface_table[12*i] -> typed index */
            int   first_edge = support_surface->first_edge_index;
            int   current_edge = first_edge;
            char *edge_table = (char *)bsp->edges.address;
            real_plane3d support_plane;
            real_plane3d neighbour_plane;
            float best_dist_sq = 3.4028235e38f;
            unsigned int best_neighbour = (unsigned int)-1;
            float surface_normal_i, surface_normal_j, surface_normal_k, surface_distance;
            float probe_x, probe_y, probe_z;
            float best_cos = 2.0f;
            float projection;

            bsp3d_get_plane_from_designator(&bsp->bsp3d, support_surface->plane_designator, &support_plane);
            surface_normal_i = support_plane.n.__s1.i;
            surface_normal_j = support_plane.n.__s1.j;
            surface_normal_k = support_plane.n.__s1.k;
            surface_distance = support_plane.d;

            /* project the moved position onto the support plane to seed the probe point */
            /* DEVIATION: plane dot is fmadds @0x837AF84C/0x837AF850 (y*n.j seed plain fmuls
             * @0x837AF820; the -d and negate are the plain fsubs/fneg @0x837AF854/0x837AF858);
             * each probe component is a fused fmadds @0x837AF85C/0x837AF860/0x837AF864. */
            projection = -(fused_madd(moved_position.__s1.x, support_plane.n.__s1.i,
                                fused_madd(moved_position.__s1.z, support_plane.n.__s1.k,
                                        moved_position.__s1.y * support_plane.n.__s1.j))
                                - support_plane.d);
            probe_x = fused_madd(support_plane.n.__s1.i, projection, moved_position.__s1.x);
            probe_y = fused_madd(support_plane.n.__s1.j, projection, moved_position.__s1.y);
            probe_z = fused_madd(support_plane.n.__s1.k, projection, moved_position.__s1.z);

            do
            {
                collision_edge *edge = &((collision_edge *)edge_table)[current_edge]; /* recovered: &edge_table[24*i] -> typed index */
                int          is_left = (edge->surface_indices[1] == support_surface_index);
                unsigned int neighbour = is_left ? edge->surface_indices[0] : edge->surface_indices[1];
                if (neighbour != (unsigned int)-1)
                {
                    collision_surface *neighbour_surface =
                        &((collision_surface *)surface_table)[neighbour]; /* recovered: &surface_table[12*i] -> typed index */
                    if ((physics->in_flags & (1u << _biped_physics_in_climb_anything_bit)) != 0 || (neighbour_surface->flags & (1u << _collision_surface_climbable_bit)) != 0)
                    {
                        bsp3d_get_plane_from_designator(&bsp->bsp3d, neighbour_surface->plane_designator,
                                                        &neighbour_plane);
                        /* DEVIATION: both plane dots accumulate through fmadds pairs
                         * (@0x837AF8F8/0x837AF8FC and @0x837AF918/0x837AF91C) over plain fmuls
                         * seeds (@0x837AF8EC/0x837AF908). */
                        if (fused_madd(neighbour_plane.n.__s1.i, moved_velocity.__s1.i,
                                  fused_madd(neighbour_plane.n.__s1.j, moved_velocity.__s1.j,
                                          neighbour_plane.n.__s1.k * moved_velocity.__s1.k)) > 0.0f
                            && (fused_madd(neighbour_plane.n.__s1.i, moved_position.__s1.x,
                                             fused_madd(neighbour_plane.n.__s1.k, moved_position.__s1.z,
                                                     neighbour_plane.n.__s1.j * moved_position.__s1.y))
                                     - neighbour_plane.d)
                               > (double)(physics->width * -0.5f))
                        {
                            /* clamp the probe onto the neighbour edge segment, keep the closest surface */
                            /* the folded 16 was sizeof(collision_vertex) */
                            const collision_vertex *vertex_table =
                                (const collision_vertex *)bsp->vertices.address;
                            const float *edge_start = vertex_table[edge->vertex_indices[0]].point.n;
                            const float *edge_end   = vertex_table[edge->vertex_indices[1]].point.n;
                            float edge_dy = (edge_end[1] - edge_start[1]);
                            float edge_dz = (edge_end[2] - edge_start[2]);
                            float start_x = *edge_start;
                            float edge_dx = (*edge_end - *edge_start);
                            /* DEVIATION: numerator/denominator accumulate through fmadds
                             * @0x837AF980/0x837AF988 and @0x837AF984/0x837AF98C over the plain
                             * y-term fmuls seeds (@0x837AF978/0x837AF97C); the divide stays plain. */
                            float segment_fraction =
                                (fused_madd(probe_x - *edge_start, *edge_end - *edge_start,
                                       fused_madd(probe_z - edge_start[2], edge_end[2] - edge_start[2],
                                               (probe_y - edge_start[1]) * (edge_end[1] - edge_start[1])))
                                     / fused_madd(*edge_end - *edge_start, *edge_end - *edge_start,
                                             fused_madd(edge_dz, edge_dz,
                                                     edge_dy * edge_dy)));
                            float closest_x, closest_y, closest_z;
                            float dist_sq;
                            if (segment_fraction >= 0.0f)
                            {
                                if (segment_fraction <= 1.0f)
                                {
                                    /* DEVIATION: each lerp is a fused fmadds @0x837AF9F0/0x837AF9E8/0x837AF9E0. */
                                    closest_z = fused_madd(edge_end[2] - edge_start[2], segment_fraction, edge_start[2]);
                                    closest_y = fused_madd(edge_end[1] - edge_start[1], segment_fraction, edge_start[1]);
                                    closest_x = fused_madd(edge_dx, segment_fraction, start_x);
                                }
                                else
                                {
                                    closest_x = *edge_end;
                                    closest_y = edge_end[1];
                                    closest_z = edge_end[2];
                                }
                            }
                            else
                            {
                                closest_x = *edge_start;
                                closest_y = edge_start[1];
                                closest_z = edge_start[2];
                            }
                            /* DEVIATION: distance accumulates through fmadds @0x837AFA18/0x837AFA1C
                             * over the plain y-delta fmuls seed @0x837AFA14. */
                            dist_sq = fused_madd(closest_x - probe_x, closest_x - probe_x,
                                     fused_madd(closest_z - probe_z, closest_z - probe_z,
                                             (closest_y - probe_y) * (closest_y - probe_y)));
                            if (dist_sq < best_dist_sq)
                            {
                                best_dist_sq = dist_sq;
                                best_cos = fused_madd(neighbour_plane.n.__s1.i, moved_velocity.__s1.i,
                                         fused_madd(neighbour_plane.n.__s1.j, moved_velocity.__s1.j,
                                                 neighbour_plane.n.__s1.k * moved_velocity.__s1.k));
                                best_neighbour = neighbour;
                                surface_normal_i = neighbour_plane.n.__s1.i;
                                surface_normal_j = neighbour_plane.n.__s1.j;
                                surface_normal_k = neighbour_plane.n.__s1.k;
                                surface_distance = neighbour_plane.d;
                            }
                        }
                    }
                }
                current_edge = is_left ? edge->edge_indices[1] : edge->edge_indices[0];
            }
            while (current_edge != first_edge);

            if (best_neighbour != (unsigned int)-1
                && best_dist_sq <= ((physics->width * 2.0f) * (physics->width * 2.0f))
                && best_cos <= 0.053333335f)
            {
                /* DEVIATION: plane dot is fmadds @0x837AFABC/0x837AFAC0 over the plain
                 * y*n.j fmuls seed @0x837AFAA8; the -(d+width) is the plain fadds/fsubs. */
                float drop = (fused_madd(surface_normal_i, moved_position.__s1.x,
                                   fused_madd(surface_normal_k, moved_position.__s1.z,
                                           surface_normal_j * moved_position.__s1.y))
                                   - (surface_distance + physics->width));
                if (__fabs(drop) <= (physics->width * 0.5f))
                {
                    /* snap the moved position onto the neighbour plane and synthesize one contact */
                    /* DEVIATION: the snap and bias applications are fused fmadds
                     * (@0x837AFAE8/0x837AFAF0/0x837AFAF8 and @0x837AFB1C/0x837AFB24/0x837AFB2C);
                     * the velocity dot accumulates through fmadds @0x837AFAE4/0x837AFB00 over the
                     * plain n.k*v.k fmuls seed @0x837AFAD4. */
                    moved_position.__s1.x = fused_madd(surface_normal_i, -drop, moved_position.__s1.x);
                    moved_position.__s1.y = fused_madd(surface_normal_j, -drop, moved_position.__s1.y);
                    moved_position.__s1.z = fused_madd(surface_normal_k, -drop, moved_position.__s1.z);
                    if (fused_madd(surface_normal_i, moved_velocity.__s1.i,
                              fused_madd(surface_normal_j, moved_velocity.__s1.j,
                                      surface_normal_k * moved_velocity.__s1.k)) > -0.033333335f)
                    {
                        float bias = -(fused_madd(surface_normal_i, moved_velocity.__s1.i,
                                            fused_madd(surface_normal_j, moved_velocity.__s1.j,
                                                    surface_normal_k * moved_velocity.__s1.k))
                                            + SECONDS_PER_TICK);
                        moved_velocity.__s1.i = fused_madd(bias, surface_normal_i, moved_velocity.__s1.i);
                        moved_velocity.__s1.j = fused_madd(surface_normal_j, bias, moved_velocity.__s1.j);
                        moved_velocity.__s1.k = fused_madd(surface_normal_k, bias, moved_velocity.__s1.k);
                    }
                    physics->stick_surface_index = best_neighbour;
                    collision_count = 1;
                    collisions[0].t = 0.0f;
                    /* DEVIATION: contact point offsets are fused fmadds @0x837AFB5C/0x837AFB64/0x837AFB6C. */
                    collisions[0].point.__s1.x = fused_madd(-physics->width, surface_normal_i, moved_position.__s1.x);
                    collisions[0].point.__s1.y = fused_madd(surface_normal_j, -physics->width, moved_position.__s1.y);
                    collisions[0].point.__s1.z = fused_madd(surface_normal_k, -physics->width, moved_position.__s1.z);
                    collisions[0].plane.n.__s1.i = surface_normal_i;
                    collisions[0].plane.n.__s1.j = surface_normal_j;
                    collisions[0].plane.n.__s1.k = surface_normal_k;
                    collisions[0].plane.d = surface_distance;
                    collisions[0].object_index = -1;
                    collisions[0].surface_index = best_neighbour;
                    collisions[0].flags = 0;
                    collisions[0].breakable_surface_index = 0;
                    collisions[0].material_index = -1;
                }
            }
        }

        /* re-normalize the facing used to bias support-surface selection */
        {
            /* DEVIATION: fmadds @0x837AFBB8 over the plain hx*hx fmuls seed @0x837AFBAC. */
            float facing_len_sq = fused_madd(horizontal_y, horizontal_y,
                                 horizontal_x * horizontal_x);
            if (facing_len_sq > 0.0000000099999991f)
            {
                float facing_len = __fsqrts(facing_len_sq);
                horizontal_x = ((1.0f / facing_len) * horizontal_x);
                horizontal_y = ((1.0f / facing_len) * horizontal_y);
            }
        }

        /* ---- pick the best support surface among the contacts ---- */
        if ((physics->in_flags & (1u << _biped_physics_in_flying_bit)) == 0 && collision_count > 0)
        {
            int   stick_surface = physics->stick_surface_index;
            short i = 0;
            int   index = 0;
            do
            {
                real_vector3d *contact_normal = &collisions[index].plane.n;
                int walkable = (physics->in_flags & (1u << _biped_physics_in_dead_bit)) == 0
                    && ((unsigned char)grounded || (collisions[index].flags & (1u << _collision_surface_climbable_bit)) != 0);
                int is_stick = (stick_surface != -1
                    && chosen_surface_index != -1
                    && collisions[chosen_surface_index].surface_index == stick_surface);
                int take = 0;

                if (!walkable)
                {
                    if (!have_walkable)
                        take = (collisions[index].plane.n.__s1.k > best_normal_k);
                }
                else if (!grounded
                         /* DEVIATION: fmadds @0x837AFCEC over the plain n.i*hx fmuls seed @0x837AFCDC. */
                         && fused_madd(collisions[index].plane.n.__s1.j, horizontal_y,
                                  collisions[index].plane.n.__s1.i * horizontal_x) > 0.5f)
                {
                    take = 0;
                }
                else if (have_walkable)
                {
                    if (is_stick)
                        take = 1;
                    else if (!have_facing)
                        /* DEVIATION: the negated dot is fnmadds @0x837AFCC8 (spelled fused_nmsub with
                         * a negated addend) over fmadds @0x837AFCC4 and the plain n.j*v.j fmuls seed
                         * @0x837AFCB8. */
                        take = fused_nmsub(contact_normal->__s1.i, physics->new_velocity.__s1.i,
                                             -fused_madd(contact_normal->__s1.k, physics->new_velocity.__s1.k,
                                                     contact_normal->__s1.j * physics->new_velocity.__s1.j))
                             > best_normal_dot;
                }
                else
                {
                    take = 1;
                }

                if (take)
                {
                    best_normal_dot = fused_nmsub(contact_normal->__s1.i, physics->new_velocity.__s1.i,
                                    -fused_madd(contact_normal->__s1.k, physics->new_velocity.__s1.k,
                                            contact_normal->__s1.j * physics->new_velocity.__s1.j));
                    chosen_surface_index = i;
                    have_walkable = walkable;
                    have_facing = (char)is_stick;
                    best_normal_k = collisions[index].plane.n.__s1.k;
                }

                if ((physics->out_flags & (1u << _biped_physics_out_volatile_collision_bit)) == 0
                    && ((collisions[index].flags & (1u << _collision_surface_breakable_bit)) != 0
                     || (collisions[index].object_index != -1
                      && ((1 << DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                collisions[index].object_index)->type)
                         & 0x40) == 0)))
                {
                    physics->out_flags |= (1u << _biped_physics_out_volatile_collision_bit);
                }

                i = (short)(index + 1);
                index = i;
            }
            while (i < collision_count);
        }

        /* ---- resolve the support plane from the chosen contact ---- */
        if (chosen_surface_index != -1)
        {
            real_vector3d *chosen_normal = &collisions[chosen_surface_index].plane.n;
            float normal_i = collisions[chosen_surface_index].plane.n.__s1.i;
            float normal_j = chosen_normal->__s1.j;
            float normal_k = chosen_normal->__s1.k;

            if (have_walkable || have_facing)
            {
                stuck_object_valid = 1;
            }
            else if (best_normal_k >= physics->minimum_normal_k)
            {
                stuck_object_valid = 1;
                if ((physics->in_flags & (1u << _biped_physics_in_airborne_bit)) != 0 && physics->ground_tangential_velocity_max < 3.4028235e38f)
                {
                    /* DEVIATION: the negated dot is fnmadds @0x837AFE80 (spelled fused_nmsub with a
                     * negated addend) over fmadds @0x837AFE78 and the plain n.i*v.i fmuls seed
                     * @0x837AFE74; the slide components are fused fmadds @0x837AFEBC-0x837AFEC4,
                     * and both magnitudes accumulate through fmadds pairs (@0x837AFED0/0x837AFED4,
                     * @0x837AFEE8/0x837AFEEC) over plain fmuls seeds. */
                    float projection = fused_nmsub(normal_k, start_velocity.__s1.k,
                                     -fused_madd(start_velocity.__s1.j, normal_j,
                                             normal_i * start_velocity.__s1.i));
                    float slide_z = fused_madd(normal_k, projection, start_velocity.__s1.k);
                    float slide_x = fused_madd(normal_i, projection, start_velocity.__s1.i);
                    float slide_y = fused_madd(normal_j, projection, start_velocity.__s1.j);
                    if (fused_madd(slide_x, slide_x,
                              fused_madd(slide_z, slide_z,
                                      slide_y * slide_y))
                        > (double)(physics->ground_tangential_velocity_max
                                        * physics->ground_tangential_velocity_max))
                    {
                        stuck_object_valid =
                            (projection
                                  / __fsqrts(fused_madd(start_velocity.__s1.i, start_velocity.__s1.i,
                                                   fused_madd(start_velocity.__s1.k, start_velocity.__s1.k,
                                                           start_velocity.__s1.j * start_velocity.__s1.j))))
                            >= (double)physics->ground_tangential_angle;
                    }
                }
            }

            if (stuck_object_valid)
            {
                physics->out_flags &= ~(1u << _biped_physics_out_airborne_bit);
                physics->support_surface_index = collisions[chosen_surface_index].surface_index;
                physics->ground_plane.n.__s1.i = collisions[chosen_surface_index].plane.n.__s1.i;
                physics->ground_plane.n.__s1.j = collisions[chosen_surface_index].plane.n.__s1.j;
                physics->ground_plane.n.__s1.k = collisions[chosen_surface_index].plane.n.__s1.k;
                physics->ground_plane.d = collisions[chosen_surface_index].plane.d;
                if (physics->support_surface_index == -1
                    || physics->support_surface_index != physics->stick_surface_index)
                {
                    /* DEVIATION: fnmadds @0x837AFF88 (spelled fused_nmsub with a negated addend)
                     * over fmadds @0x837AFF84 and the plain n.k*v.k fmuls seed @0x837AFF78. */
                    physics->landing_velocity =
                        fused_nmsub(start_velocity.__s1.i, physics->ground_plane.n.__s1.i,
                               -fused_madd(physics->ground_plane.n.__s1.j, start_velocity.__s1.j,
                                       physics->ground_plane.n.__s1.k * start_velocity.__s1.k));
                    goto support_done;
                }
            }
        }

        if (!stuck_object_valid)
        {
            physics->out_flags |= (1u << _biped_physics_out_airborne_bit);
            physics->ground_plane = depths_of_hell;
            physics->support_surface_index = -1;
        }
        physics->landing_velocity = 0.0f;
    support_done:;

        /* ---- pick the bumped object (farthest relative velocity, preferring vehicles: object.type == object_type_vehicle) ---- */
        if (collision_count > 0)
        {
            float   best_bump_sq = 2.0f;
            int16_t best_bump_type = 0;  /* +0xB4 is object.type (DB rel 176), not owner_team_index: vehicles preferred */
            short   i = 0;
            int     index = 0;
            for (;;)
            {
                int object_index = collisions[index].object_index;
                if (object_index != -1)
                {
                    object_datum *contact_object =
                        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                    float bump_dy = (contact_object->object.translational_velocity.n[1] - moved_velocity.__s1.j);
                    float bump_dz = (contact_object->object.translational_velocity.n[2] - moved_velocity.__s1.k);
                    float bump_dx = (contact_object->object.translational_velocity.n[0] - moved_velocity.__s1.i);
                    /* DEVIATION: magnitude accumulates dx^2 + (dz^2 + dy^2) through fmadds
                     * @0x837B0048/0x837B0044 over the plain dy^2 fmuls seed @0x837B0040 — the
                     * previous dz-outer grouping was mis-ordered. */
                    float bump_sq = fused_madd(bump_dx, bump_dx,
                                  fused_madd(bump_dz, bump_dz,
                                          bump_dy * bump_dy));
                    int take;
                    if (bump_object == -1)
                        take = 1;
                    else if (best_bump_type == 1)
                        take = (contact_object->object.type == object_type_vehicle);
                    else if (contact_object->object.type != object_type_vehicle)
                        take = (bump_sq > best_bump_sq);
                    else
                        take = 1;
                    if (take)
                    {
                        best_bump_type = contact_object->object.type;
                        bump_object = object_index;
                        best_bump_sq = bump_sq;
                    }
                }
                i = (short)(index + 1);
                index = i;
                if (i >= collision_count)
                    break;
            }
        }
        physics->bumped_object_index = bump_object;

        /* ---- pick the elevator object (a mover carrying the biped) ---- */
        physics->elevator_object_index = -1;
        if (collision_count > 0)
        {
            short i = 0;
            int   index = 0;
            do
            {
                int object_index = collisions[index].object_index;
                if (object_index != -1)
                {
                    object_datum *elevator_datum = object_try_and_get_and_verify_type(object_index, object_mask_machine);
                    if (elevator_datum)
                    {
                        machine_definition *elevator_definition =
                            TAG_GET(machine_definition, elevator_datum->definition_index);
                        if ((elevator_definition->machine.flags & (1u << _machine_never_appears_locked_bit)) != 0
                            && (uint16_t)elevator_definition->machine.elevator_node_index != 0xFFFF)
                            physics->elevator_object_index = object_index;
                    }
                }
                i = (short)(index + 1);
                index = i;
            }
            while (i < collision_count);
        }

        /* ---- write the resolved position/velocity and the total travel distance ---- */
        physics->new_position.__s1.x = moved_position.__s1.x;
        physics->new_position.__s1.y = moved_position.__s1.y;
        physics->new_position.__s1.z = moved_position.__s1.z;
        physics->new_velocity.__s1.k = moved_velocity.__s1.k;
        physics->new_velocity.__s1.i = moved_velocity.__s1.i;
        physics->new_velocity.__s1.j = moved_velocity.__s1.j;
        physics->new_velocity.__s1.k = physics->new_velocity.__s1.k - physics->crouch_velocity;
        /* DEVIATION: magnitude accumulates through fmadds @0x837B01EC/0x837B01DC over the plain
         * dj^2 fmuls seed @0x837B01CC. */
        physics->collision_velocity = __fsqrts(
              fused_madd(moved_velocity.__s1.i - start_velocity.__s1.i,
                            moved_velocity.__s1.i - start_velocity.__s1.i,
                    fused_madd(moved_velocity.__s1.k - start_velocity.__s1.k,
                                    moved_velocity.__s1.k - start_velocity.__s1.k,
                            (moved_velocity.__s1.j - start_velocity.__s1.j)
                                    * (moved_velocity.__s1.j - start_velocity.__s1.j))));

        /* ---- final ledge/step feature test for climbable geometry ---- */
        {
            uint16_t flags3 = physics->in_flags;
            if ((flags3 & (1u << _biped_physics_in_crouched_bit)) != 0 && (flags3 & (1u << _biped_physics_in_trying_to_stand_bit)) != 0)
            {
                biped_datum *biped_object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data,
                    object_header_datum, physics->biped_index)->datum;
                if (biped_object->unit.player_index != -1)
                {
                    biped_definition *definition =
                        TAG_GET(biped_definition, biped_object->definition_index);
                    int definition_flags = definition->biped.flags;
                    if ((definition_flags & (1u << _biped_pill_centered_at_origin_bit)) == 0 && (definition_flags & (1u << _biped_spherical_bit)) == 0)
                    {
                        unsigned int feature_flags = (flags3 & (1u << _biped_physics_in_dead_bit)) != 0
                            ? _collision_test_for_bipeds_dead_flags : _collision_test_for_bipeds_living_flags;
                        float sphere_radius = (definition->biped.collision_height_standing * 0.5f);
                        float sphere_width  = definition->biped.collision_radius;
                        real_point3d     sphere_center;
                        collision_feature_list features;
                        real_vector3d    probe_vector;
                        collision_plane  feature_hit;

                        sphere_center.__s1.x = moved_position.__s1.x;
                        sphere_center.__s1.y = moved_position.__s1.y;
                        sphere_center.__s1.z = moved_position.__s1.z + sphere_radius;
                        /* ignore_object re-derived: the biped ignores itself (decompiler mis-shifted this arg) */
                        if (collision_get_features_in_sphere(feature_flags, &sphere_center, sphere_radius,
                                                             0.0f, sphere_width, physics->biped_index,
                                                             &features))
                        {
                            /* DEVIATION: fused fnmsubs @0x837B02DC (height - width*2 in one rounding). */
                            float probe_scale = fused_nmsub(sphere_width, 2.0f,
                                                       definition->biped.collision_height_standing);
                            probe_vector.__s1.i = probe_scale * global_up3d->n[0];
                            probe_vector.__s1.j = global_up3d->n[1] * probe_scale;
                            probe_vector.__s1.k = global_up3d->n[2] * probe_scale;
                            if (collision_features_test_vector(&features,
                                                               &physics->new_position,
                                                               &probe_vector, &feature_hit))
                            {
                                physics->out_flags |= (1u << _biped_physics_out_cannot_stand_bit);
                            }
                        }
                    }
                }
            }
        }
    }
}
