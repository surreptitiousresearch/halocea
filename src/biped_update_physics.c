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

        movement_penalty_inv = 1.0f - physics->movement_penalty;
        desired_z = ((physics->movement_desired.__s1.k * flying_up.__s1.k)
                  + ((physics->forward.__s1.k * physics->movement_desired.__s1.i)
                          + (physics->movement_desired.__s1.j * flying_left.__s1.k)));
        desired_x = ((physics->movement_desired.__s1.j * flying_left.__s1.i)
                          + (physics->movement_desired.__s1.k * flying_up.__s1.i));

        accel.__s1.j = (movement_penalty_inv
                        * ((physics->movement_desired.__s1.k * flying_up.__s1.j)
                                + ((physics->forward.__s1.j * physics->movement_desired.__s1.i)
                                        + (physics->movement_desired.__s1.j * flying_left.__s1.j))))
                - physics->velocity.__s1.j;
        accel.__s1.k = (movement_penalty_inv * desired_z) - physics->velocity.__s1.k;
        accel.__s1.i = (movement_penalty_inv
                        * ((physics->movement_desired.__s1.i * physics->forward.__s1.i)
                                + desired_x))
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
        new_velocity->__s1.j = ((physics->forward.__s1.j * physics->movement_desired.__s1.i)
                                + (physics->movement_desired.__s1.j * physics->forward.__s1.i));
        new_velocity->__s1.i = ((physics->forward.__s1.i * physics->movement_desired.__s1.i)
                                - (physics->forward.__s1.j * physics->movement_desired.__s1.j));
        horizontal_y = new_velocity->__s1.j;
        horizontal_x = new_velocity->__s1.i;
    }
    else if ((in_flags & (1u << _biped_physics_in_airborne_bit)) != 0)
    {
        /* ---- airborne: horizontal accel clamped to the airborne budget, plus gravity ---- */
        real_vector2d horizontal_accel;
        float movement_penalty_inv = 1.0f - physics->movement_penalty;
        float clamped_x, clamped_y;

        horizontal_accel.__s1.i = ((movement_penalty_inv
                                   * ((physics->forward.__s1.i * physics->movement_desired.__s1.i)
                                           - (physics->forward.__s1.j * physics->movement_desired.__s1.j)))
                                   - physics->velocity.__s1.i);
        horizontal_accel.__s1.j = ((movement_penalty_inv
                                   * ((physics->forward.__s1.j * physics->movement_desired.__s1.i)
                                           + (physics->movement_desired.__s1.j * physics->forward.__s1.i)))
                                   - physics->velocity.__s1.j);
        if (normalize2d(&horizontal_accel) <= physics->airborne_acceleration_maximum)
        {
            clamped_x = horizontal_accel.__s1.i;
            clamped_y = horizontal_accel.__s1.j;
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

        speed_scale = __fsqrts(((physics->movement_desired.__s1.k * physics->movement_desired.__s1.k)
                             + ((physics->movement_desired.__s1.i * physics->movement_desired.__s1.i)
                                     + (physics->movement_desired.__s1.j * physics->movement_desired.__s1.j))));

        if ((in_flags & (1u << _biped_physics_in_climb_anything_bit)) != 0)
        {
            /* on a resolved ground plane: build a tangent basis from the aiming vector and the
             * ground normal, then express the desired movement in it */
            real_vector3d tangent, bitangent;

            tangent.__s1.i = (physics->ground_plane.n.__s1.j * physics->aiming.__s1.k)
                      - (physics->ground_plane.n.__s1.k * physics->aiming.__s1.j);
            tangent.__s1.j = (physics->ground_plane.n.__s1.k * physics->aiming.__s1.i)
                      - (physics->ground_plane.n.__s1.i * physics->aiming.__s1.k);
            tangent.__s1.k = (physics->ground_plane.n.__s1.i * physics->aiming.__s1.j)
                      - (physics->ground_plane.n.__s1.j * physics->aiming.__s1.i);
            if (normalize3d(&tangent) == 0.0f)
            {
                /* aiming parallel to the ground normal — fall back to global up, then forward */
                tangent.__s1.i = (global_up3d->n[2] * physics->ground_plane.n.__s1.j)
                          - (global_up3d->n[1] * physics->ground_plane.n.__s1.k);
                tangent.__s1.j = (global_up3d->n[0] * physics->ground_plane.n.__s1.k)
                          - (global_up3d->n[2] * physics->ground_plane.n.__s1.i);
                tangent.__s1.k = (global_up3d->n[1] * physics->ground_plane.n.__s1.i)
                          - (global_up3d->n[0] * physics->ground_plane.n.__s1.j);
                if (normalize3d(&tangent) == 0.0f)
                {
                    tangent.__s1.i = (global_forward3d->n[2] * physics->ground_plane.n.__s1.j)
                              - (global_forward3d->n[1] * physics->ground_plane.n.__s1.k);
                    tangent.__s1.j = (global_forward3d->n[0] * physics->ground_plane.n.__s1.k)
                              - (global_forward3d->n[2] * physics->ground_plane.n.__s1.i);
                    tangent.__s1.k = (global_forward3d->n[1] * physics->ground_plane.n.__s1.i)
                              - (global_forward3d->n[0] * physics->ground_plane.n.__s1.j);
                    normalize3d(&tangent);
                }
            }

            bitangent.__s1.i = (physics->ground_plane.n.__s1.j * tangent.__s1.k)
                        - (physics->ground_plane.n.__s1.k * tangent.__s1.j);
            bitangent.__s1.j = (physics->ground_plane.n.__s1.k * tangent.__s1.i)
                        - (physics->ground_plane.n.__s1.i * tangent.__s1.k);
            bitangent.__s1.k = (physics->ground_plane.n.__s1.i * tangent.__s1.j)
                        - (physics->ground_plane.n.__s1.j * tangent.__s1.i);
            normalize3d(&bitangent);

            move_direction.__s1.i = (physics->movement_desired.__s1.j * tangent.__s1.i)
                             + (physics->movement_desired.__s1.i * bitangent.__s1.i);
            move_direction.__s1.j = (physics->movement_desired.__s1.j * tangent.__s1.j)
                             + (physics->movement_desired.__s1.i * bitangent.__s1.j);
            move_direction.__s1.k = ((physics->movement_desired.__s1.j * tangent.__s1.k)
                                     + (physics->movement_desired.__s1.i * bitangent.__s1.k))
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

            tangent.__s1.k = (aiming_j * global_up3d->n[0]) - (aiming_i * global_up3d->n[1]);
            tangent.__s1.i = (aiming_k * global_up3d->n[1]) - (aiming_j * global_up3d->n[2]);
            tangent.__s1.j = (aiming_i * global_up3d->n[2]) - (aiming_k * global_up3d->n[0]);
            normalize3d(&tangent);
            ground_normal = &physics->ground_plane.n;

            horizontal_x = ((physics->forward.__s1.i * physics->movement_desired.__s1.i)
                                 - (physics->forward.__s1.j * physics->movement_desired.__s1.j));
            horizontal_y = ((physics->forward.__s1.j * physics->movement_desired.__s1.i)
                                 + (physics->movement_desired.__s1.j * physics->forward.__s1.i));

            /* project (aiming, tangent) rotation of the desired movement onto the ground-normal plane */
            bitangent_z = ((physics->forward.__s1.k
                        * ((-((physics->ground_plane.n.__s1.i * tangent.__s1.i)
                                                 + ((physics->ground_plane.n.__s1.j * tangent.__s1.j)
                                                         + (physics->ground_plane.n.__s1.k * tangent.__s1.k)))
                                        * physics->ground_plane.n.__s1.k)
                                + tangent.__s1.k))   /* was ))) — extra ) prematurely closed the outer (float)( wrapper */
                        + (physics->forward.__s1.i
                        * ((-((physics->ground_plane.n.__s1.i * aiming_i)
                                                 + ((physics->ground_plane.n.__s1.j * aiming_j)
                                                         + (physics->ground_plane.n.__s1.k * aiming_k)))
                                        * physics->ground_plane.n.__s1.k)
                                + aiming_k)));
            bitangent_y = ((physics->forward.__s1.k
                        * ((-((physics->ground_plane.n.__s1.i * tangent.__s1.i)
                                                 + ((physics->ground_plane.n.__s1.j * tangent.__s1.j)
                                                         + (physics->ground_plane.n.__s1.k * tangent.__s1.k)))
                                        * physics->ground_plane.n.__s1.j)
                                + tangent.__s1.j))   /* was ))) — extra ) prematurely closed the outer (float)( wrapper */
                        + (physics->forward.__s1.i
                        * ((-((physics->ground_plane.n.__s1.i * aiming_i)
                                                 + ((physics->ground_plane.n.__s1.j * aiming_j)
                                                         + (physics->ground_plane.n.__s1.k * aiming_k)))
                                        * physics->ground_plane.n.__s1.j)
                                + aiming_j)));
            bitangent_x = (physics->forward.__s1.k
                        * ((-((physics->ground_plane.n.__s1.i * tangent.__s1.i)
                                                 + ((physics->ground_plane.n.__s1.j * tangent.__s1.j)
                                                         + (physics->ground_plane.n.__s1.k * tangent.__s1.k)))
                                        * physics->ground_plane.n.__s1.i)
                                + tangent.__s1.i))
                        + (physics->forward.__s1.i
                        * ((-((physics->ground_plane.n.__s1.i * aiming_i)
                                                 + ((physics->ground_plane.n.__s1.j * aiming_j)
                                                         + (physics->ground_plane.n.__s1.k * aiming_k)))
                                        * physics->ground_plane.n.__s1.i)
                                + aiming_i));

            move_direction.__s1.i = bitangent_x;
            move_direction.__s1.j = bitangent_y;
            move_direction.__s1.k = bitangent_z + physics->forward.__s1.k;
            if (((in_flags >> _biped_physics_in_climb_anything_bit) & 1) == 0)
                move_direction.__s1.k = (bitangent_z + physics->forward.__s1.k) * 5.0f;
        }
        else
        {
            /* on a slope: rotate the desired movement by the facing yaw, then keep it in-plane */
            ground_normal = &physics->ground_plane.n;
            horizontal_x = ((physics->forward.__s1.i * physics->movement_desired.__s1.i)
                                 - (physics->forward.__s1.j * physics->movement_desired.__s1.j));
            horizontal_y = ((physics->forward.__s1.j * physics->movement_desired.__s1.i)
                                 + (physics->movement_desired.__s1.j * physics->forward.__s1.i));
            move_direction.__s1.j = horizontal_y;
            move_direction.__s1.i = horizontal_x;
            move_direction.__s1.k = physics->movement_desired.__s1.k
                - (((physics->ground_plane.n.__s1.j * horizontal_y)
                                + (physics->ground_plane.n.__s1.i * horizontal_x))
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
            ground_nudge = -((ground_normal->n[0] * 0.0078125f) - clamped_x);
            physics->out_flags = ((uint8_t)airborne == 0) ? 0 : (1u << _biped_physics_out_slipping_bit);
            physics->new_velocity.__s1.i = ground_nudge + physics->velocity.__s1.i;
            physics->new_velocity.__s1.j = physics->velocity.__s1.j
                - ((ground_normal->n[1] * 0.0078125f) - clamped_y);
            physics->new_velocity.__s1.k = physics->velocity.__s1.k
                - ((ground_normal->n[2] * 0.0078125f) - clamped_z);
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
            projection = -(((moved_position.__s1.x * support_plane.n.__s1.i)
                                + ((moved_position.__s1.z * support_plane.n.__s1.k)
                                        + (moved_position.__s1.y * support_plane.n.__s1.j)))
                                - support_plane.d);
            probe_x = ((support_plane.n.__s1.i * projection) + moved_position.__s1.x);
            probe_y = ((support_plane.n.__s1.j * projection) + moved_position.__s1.y);
            probe_z = ((support_plane.n.__s1.k * projection) + moved_position.__s1.z);

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
                        if (((neighbour_plane.n.__s1.i * moved_velocity.__s1.i)
                                  + ((neighbour_plane.n.__s1.j * moved_velocity.__s1.j)
                                          + (neighbour_plane.n.__s1.k * moved_velocity.__s1.k))) > 0.0f
                            && (((neighbour_plane.n.__s1.i * moved_position.__s1.x)
                                             + ((neighbour_plane.n.__s1.k * moved_position.__s1.z)
                                                     + (neighbour_plane.n.__s1.j * moved_position.__s1.y)))
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
                            float segment_fraction =
                                ((((probe_x - *edge_start) * (*edge_end - *edge_start))
                                       + (((probe_z - edge_start[2]) * (edge_end[2] - edge_start[2]))
                                               + ((probe_y - edge_start[1]) * (edge_end[1] - edge_start[1]))))
                                     / (((*edge_end - *edge_start) * (*edge_end - *edge_start))
                                             + ((edge_dz * edge_dz)
                                                     + (edge_dy * edge_dy))));
                            float closest_x, closest_y, closest_z;
                            float dist_sq;
                            if (segment_fraction >= 0.0f)
                            {
                                if (segment_fraction <= 1.0f)
                                {
                                    closest_z = (((edge_end[2] - edge_start[2]) * segment_fraction) + edge_start[2]);
                                    closest_y = ((edge_end[1] - edge_start[1]) * segment_fraction) + edge_start[1];
                                    closest_x = (edge_dx * segment_fraction) + start_x;
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
                            dist_sq = (((closest_x - probe_x) * (closest_x - probe_x))
                                     + (((closest_z - probe_z) * (closest_z - probe_z))
                                             + ((closest_y - probe_y) * (closest_y - probe_y))));
                            if (dist_sq < best_dist_sq)
                            {
                                best_dist_sq = dist_sq;
                                best_cos = ((neighbour_plane.n.__s1.i * moved_velocity.__s1.i)
                                         + ((neighbour_plane.n.__s1.j * moved_velocity.__s1.j)
                                                 + (neighbour_plane.n.__s1.k * moved_velocity.__s1.k)));
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
                float drop = (((surface_normal_i * moved_position.__s1.x)
                                   + ((surface_normal_k * moved_position.__s1.z)
                                           + (surface_normal_j * moved_position.__s1.y)))
                                   - (surface_distance + physics->width));
                if (__fabs(drop) <= (physics->width * 0.5f))
                {
                    /* snap the moved position onto the neighbour plane and synthesize one contact */
                    moved_position.__s1.x = (surface_normal_i * -drop) + moved_position.__s1.x;
                    moved_position.__s1.y = (surface_normal_j * -drop) + moved_position.__s1.y;
                    moved_position.__s1.z = (surface_normal_k * -drop) + moved_position.__s1.z;
                    if (((surface_normal_i * moved_velocity.__s1.i)
                              + ((surface_normal_j * moved_velocity.__s1.j)
                                      + (surface_normal_k * moved_velocity.__s1.k))) > -0.033333335f)
                    {
                        float bias = -(((surface_normal_i * moved_velocity.__s1.i)
                                            + ((surface_normal_j * moved_velocity.__s1.j)
                                                    + (surface_normal_k * moved_velocity.__s1.k)))
                                            + SECONDS_PER_TICK);
                        moved_velocity.__s1.i = (bias * surface_normal_i) + moved_velocity.__s1.i;
                        moved_velocity.__s1.j = (surface_normal_j * bias) + moved_velocity.__s1.j;
                        moved_velocity.__s1.k = (surface_normal_k * bias) + moved_velocity.__s1.k;
                    }
                    physics->stick_surface_index = best_neighbour;
                    collision_count = 1;
                    collisions[0].t = 0.0f;
                    collisions[0].point.__s1.x = (-physics->width * surface_normal_i) + moved_position.__s1.x;
                    collisions[0].point.__s1.y = (surface_normal_j * -physics->width) + moved_position.__s1.y;
                    collisions[0].point.__s1.z = (surface_normal_k * -physics->width) + moved_position.__s1.z;
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
            float facing_len_sq = ((horizontal_y * horizontal_y)
                                 + (horizontal_x * horizontal_x));
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
                    && ((unsigned char)grounded || (collisions[index].flags & 4) != 0);
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
                         && ((collisions[index].plane.n.__s1.j * horizontal_y)
                                  + (collisions[index].plane.n.__s1.i * horizontal_x)) > 0.5f)
                {
                    take = 0;
                }
                else if (have_walkable)
                {
                    if (is_stick)
                        take = 1;
                    else if (!have_facing)
                        take = -((contact_normal->__s1.i * physics->new_velocity.__s1.i)
                                             + ((contact_normal->__s1.k * physics->new_velocity.__s1.k)
                                                     + (contact_normal->__s1.j * physics->new_velocity.__s1.j)))
                             > best_normal_dot;
                }
                else
                {
                    take = 1;
                }

                if (take)
                {
                    best_normal_dot = -((contact_normal->__s1.i * physics->new_velocity.__s1.i)
                                    + ((contact_normal->__s1.k * physics->new_velocity.__s1.k)
                                            + (contact_normal->__s1.j * physics->new_velocity.__s1.j)));
                    chosen_surface_index = i;
                    have_walkable = walkable;
                    have_facing = (char)is_stick;
                    best_normal_k = collisions[index].plane.n.__s1.k;
                }

                if ((physics->out_flags & (1u << _biped_physics_out_volatile_collision_bit)) == 0
                    && ((collisions[index].flags & 8) != 0
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
                    float projection = -((normal_k * start_velocity.__s1.k)
                                     + ((start_velocity.__s1.j * normal_j)
                                             + (normal_i * start_velocity.__s1.i)));
                    float slide_z = ((normal_k * projection) + start_velocity.__s1.k);
                    float slide_x = (normal_i * projection) + start_velocity.__s1.i;
                    float slide_y = (normal_j * projection) + start_velocity.__s1.j;
                    if (((slide_x * slide_x)
                              + ((slide_z * slide_z)
                                      + (slide_y * slide_y)))
                        > (double)(physics->ground_tangential_velocity_max
                                        * physics->ground_tangential_velocity_max))
                    {
                        stuck_object_valid =
                            (projection
                                  / __fsqrts(((start_velocity.__s1.i * start_velocity.__s1.i)
                                                   + ((start_velocity.__s1.k * start_velocity.__s1.k)
                                                           + (start_velocity.__s1.j * start_velocity.__s1.j)))))
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
                    physics->landing_velocity =
                        -((start_velocity.__s1.i * physics->ground_plane.n.__s1.i)
                               + ((physics->ground_plane.n.__s1.j * start_velocity.__s1.j)
                                       + (physics->ground_plane.n.__s1.k * start_velocity.__s1.k)));
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
                    float bump_sq = ((bump_dz * bump_dz)
                                  + ((bump_dy * bump_dy)
                                          + (bump_dx * bump_dx)));
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
        physics->collision_velocity = __fsqrts(
              (((moved_velocity.__s1.i - start_velocity.__s1.i)
                            * (moved_velocity.__s1.i - start_velocity.__s1.i))
                    + (((moved_velocity.__s1.k - start_velocity.__s1.k)
                                    * (moved_velocity.__s1.k - start_velocity.__s1.k))
                            + ((moved_velocity.__s1.j - start_velocity.__s1.j)
                                    * (moved_velocity.__s1.j - start_velocity.__s1.j)))));

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
                            float probe_scale = -((sphere_width * 2.0f)
                                                       - definition->biped.collision_height_standing);
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
