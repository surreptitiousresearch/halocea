/* trigger_create_projectiles @0x836DB528 — fire one weapon trigger: for each firing marker resolve the
 * muzzle position/forward from the marker transform, let the controlling unit (player or actor) adjust and
 * aim the ray, then spawn the trigger's projectiles with per-shot error-cone spread and multi-projectile
 * distribution, inheriting the shooter's velocity when the projectile definition asks for it.
 *
 * Clean decompile (0x836DB528).
 * Notes / DEVIATIONS (disasm-verified 0x836DBAB4-0x836DBBF8):
 * - the FPR shadow on projectile_distribute's float distribution_angle scrambles the decompiler's last two
 *   args; the real call is projectile_distribute(&forward, &up, distribution_function, distribution_angle,
 *   marker_projectile_index, projectile_count).
 * - seed_random_vector_in_cone3d writes back into placement.forward in place (axis == result == &forward);
 *   inner cone angle = projectile_error_inner_cone_angle, outer = the per-shot error value.
 * - the player-aim path physically follows the marker loop in the binary (a break + goto LABEL_23); it is
 *   logically the "controlling player valid" branch of the aim step and is written as such here.
 * - charged primary fire (alternate_shots_loaded > 0) validates/looks up velocity flags against the
 *   SECONDARY trigger's projectile while still spawning THIS trigger's projectile — reproduced verbatim. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_placement_data.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/networked_datum_role.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_definition_flags.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/projectile_definition.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/projectile_definition_flags.h"
#include "headers/weapon_secondary_trigger_mode.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int weapon_get_owner_object_index(int weapon_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t actor_firing_blindly(int actor_index);
extern void unit_adjust_projectile_ray(int unit_index, real_point3d *origin, real_vector3d *forward, float *inherited_velocity, uint8_t offset_origin, uint8_t offset_aim);
extern int actor_aim_projectile(int actor_index, const real_point3d *origin, real_vector3d *vector, float *error_reference);
extern int player_aim_projectile(int player_index, const real_point3d *position, real_vector3d *direction);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern float __fsqrts(float x);
extern double __fabs(double x);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern void projectile_distribute(real_vector3d *forward, real_vector3d *up, int16_t distribution_function, float distribution_angle, int16_t index, int16_t count);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point);
extern void projectile_set_target_object_index(int projectile_index, int target_object_index);
extern void projectile_kill_tracer(int projectile_index);

void trigger_create_projectiles(int weapon_index, int16_t trigger_index, NetworkedDatumRole projectile_datum_role)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger_state = &weapon->weapon.triggers[trigger_index]; /* per-trigger runtime */
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *triggers_block =
        (weapon_trigger_definition *)definition->weapon.triggers.address;
    weapon_trigger_definition *trigger = &triggers_block[trigger_index]; /* this trigger's definition */
    int owner_object_index = weapon_get_owner_object_index(weapon_index);

    /* pick the marker set (parent object's when the weapon flags its markers as parent-relative) */
    const char *trigger_marker_names[2];
    trigger_marker_names[0] = "primary trigger";
    trigger_marker_names[1] = "secondary trigger";
    int marker_object_index = weapon_index;
    if ( weapon->object.flags & (1u << _object_invisible_bit) )
    {
        int parent = weapon->object.parent_object_index;
        if ( parent != -1 )
            marker_object_index = parent;
    }

    object_marker markers[28];
    int16_t marker_count = object_get_marker_by_name(marker_object_index,
            trigger_marker_names[trigger_index], markers, 64);
    if ( !marker_count )
        marker_count = 1;
    if ( (trigger->flags & (1u << _weapon_trigger_uses_weapon_origin_bit)) == 0 )    /* not "fire from all markers" */
        marker_count = 1;

    if ( marker_count <= 0 )
        return;

    for ( int marker = 0; marker < marker_count; ++marker )
    {
        float inherited_velocity = 0.0f;
        float aim_error = 0.0f;
        real_point3d position = markers[marker].matrix.___u1.__s1.position;
        real_vector3d forward = markers[marker].matrix.___u1.__s1.forward;

        unit_datum *owner_unit = object_try_and_get_and_verify_type(owner_object_index, object_mask_unit);
        int aim_target_index = -1;

        int controlling_player = -1;
        int controlling_actor = -1;
        uint8_t do_aim = (trigger->flags & (1u << _weapon_trigger_projectiles_cannot_be_aimed_bit)) == 0 && owner_unit
                && (owner_unit->object.damage_flags & (1u << _object_dead_bit)) == 0;
        if ( do_aim )
        {
            controlling_player = owner_unit->unit.player_index;
            controlling_actor = owner_unit->unit.actor_index;
            if ( owner_unit->unit.gunner_object_index != -1 )  /* gunning a vehicle: use its controller */
            {
                unit_datum *vehicle = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data,
                        object_header_datum, owner_unit->unit.gunner_object_index)->datum;
                controlling_player = vehicle->unit.player_index;
                controlling_actor = vehicle->unit.actor_index;
            }

            uint8_t not_firing_blindly = 1;
            uint8_t offset_origin =
                    (TAG_GET(unit_definition, owner_unit->definition_index)->unit.flags
                     >> _unit_definition_fires_from_camera_bit) & 1;
            if ( controlling_actor != -1 )
                not_firing_blindly = actor_firing_blindly(controlling_actor) == 0;
            unit_adjust_projectile_ray(owner_object_index, &position, &forward, &inherited_velocity,
                    offset_origin, owner_unit->unit.gunner_object_index == -1 && not_firing_blindly);

            if ( controlling_player != -1 )
            {
                /* build a right/up gun basis and offset the muzzle by the trigger's gun offset */
                real_vector3d right;
                right.n[0] = global_up3d->n[1] * forward.n[2] - global_up3d->n[2] * forward.n[1];
                right.n[1] = global_up3d->n[2] * forward.n[0] - global_up3d->n[0] * forward.n[2];
                right.n[2] = global_up3d->n[0] * forward.n[1] - global_up3d->n[1] * forward.n[0];
                if ( normalize3d(&right) == 0.0f )
                    right = *global_left3d;

                real_vector3d up;
                up.n[0] = right.n[2] * forward.n[1] - right.n[1] * forward.n[2];
                up.n[1] = forward.n[2] * right.n[0] - right.n[2] * forward.n[0];
                up.n[2] = right.n[1] * forward.n[0] - forward.n[1] * right.n[0];
                normalize3d(&up);

                float offset_forward = trigger->first_person_weapon_offset.n[0];
                float offset_right = trigger->first_person_weapon_offset.n[1];
                float offset_up = trigger->first_person_weapon_offset.n[2];
                position.n[0] = forward.n[0] * offset_forward + position.n[0];
                position.n[1] = forward.n[1] * offset_forward + position.n[1];
                position.n[2] = forward.n[2] * offset_forward + position.n[2];
                position.n[0] = offset_right * right.n[0] + position.n[0];
                position.n[1] = right.n[1] * offset_right + position.n[1];
                position.n[2] = right.n[2] * offset_right + position.n[2];
                position.n[0] = offset_up * up.n[0] + position.n[0];
                position.n[1] = up.n[1] * offset_up + position.n[1];
                position.n[2] = up.n[2] * offset_up + position.n[2];

                aim_target_index = player_aim_projectile(controlling_player, &position, &forward);
            }
            else if ( controlling_actor != -1 )
            {
                aim_target_index = actor_aim_projectile(controlling_actor, &position, &forward, &aim_error);
            }
        }

        /* "fire from all markers": muzzle stays at the raw marker position */
        if ( trigger->flags & (1u << _weapon_trigger_uses_weapon_origin_bit) )
            position = markers[marker].matrix.___u1.__s1.position;

        /* projectile count (and definition/velocity-flag source), boosted for a charged primary shot */
        int projectile_definition_index;
        int16_t projectile_count;
        int16_t charge_level = weapon->weapon.alternate_shots_loaded;
        if ( trigger_index || charge_level <= 0 )
        {
            projectile_definition_index = trigger->projectile.index;
            projectile_count = trigger->projectiles_per_shot;
        }
        else
        {
            projectile_definition_index = triggers_block[1].projectile.index; /* secondary trigger's */
            if ( definition->weapon.secondary_trigger_mode == _weapon_secondary_trigger_loads_multiple_primary_ammunition )
                ++charge_level;
            weapon->weapon.alternate_shots_loaded = 0;                        /* consume the charge */
            projectile_count = trigger->projectiles_per_shot * charge_level;
        }

        if ( projectile_definition_index == -1 )
            continue;

        /* resolve the projectile's target: the weapon's parent unit (or its vehicle) */
        int target_object_index = -1;
        int weapon_parent = weapon->object.parent_object_index;
        if ( weapon_parent != -1 )
        {
            unit_datum *parent_unit = object_try_and_get_and_verify_type(weapon_parent, object_mask_unit);
            if ( parent_unit )
            {
                target_object_index = weapon->object.parent_object_index;
                if ( parent_unit->unit.gunner_object_index != -1 )
                    target_object_index = parent_unit->unit.gunner_object_index;
            }
        }

        if ( projectile_count <= 0 )
            continue;

        projectile_definition *projectile_def =
                TAG_GET(projectile_definition, projectile_definition_index);

        real_vector3d first_marker_forward;
        for ( int shot = 0; shot < projectile_count; ++shot )
        {
            uint8_t spawn_tracer = 0;
            object_placement_data placement;
            object_placement_data_new(&placement, trigger->projectile.index, target_object_index);
            float tracer_frequency = trigger_state->rate_of_fire;
            placement.position = position;
            placement.forward = forward;

            /* tracer cadence: every trigger+38 rounds, or always when frequency is 0 */
            int16_t tracer_counter = trigger_state->sequential_non_tracer_rounds;
            trigger_state->sequential_non_tracer_rounds = tracer_counter + 1;
            if ( tracer_frequency == 0.0f || tracer_counter >= trigger->rounds_between_tracers )
            {
                spawn_tracer = 1;
                trigger_state->sequential_non_tracer_rounds = 0;
            }

            /* per-shot error cone half-angle: charge-interpolated when the aim step gave none */
            float error_angle = aim_error;
            if ( aim_error == 0.0f )
            {
                float charge = (trigger->flags & (1u << _weapon_trigger_analog_rate_of_fire_bit)) ? weapon->weapon.primary_trigger
                        : trigger_state->__noop;  /* DB member name kept */
                error_angle = (1.0f - charge) * trigger->projectile_error_angle_lower_bound
                        + trigger->projectile_error_angle_upper_bound * charge;
                aim_error = error_angle;
            }

            if ( (trigger->flags & (1u << _weapon_trigger_use_error_when_unzoomed_bit)) == 0 || (weapon->weapon.control_flags & 0x40) == 0 )
                seed_random_vector_in_cone3d(get_global_random_seed_address(), &placement.forward,
                        trigger->projectile_error_inner_cone_angle, error_angle, &placement.forward);

            /* parallel-fire: all shots reuse the first shot's jittered forward */
            if ( shot == 0 )
                first_marker_forward = placement.forward;
            if ( trigger->flags & (1u << _weapon_trigger_projectiles_have_identical_error_bit) )
                placement.forward = first_marker_forward;

            real_vector3d *up = perpendicular3d(&placement.forward, &placement.up);
            float up_magnitude = __fsqrts(up->n[2] * up->n[2]
                    + (up->n[0] * up->n[0] + up->n[1] * up->n[1]));
            if ( __fabs(up_magnitude) >= 0.000099999997f )
            {
                float inverse = 1.0f / up_magnitude;
                up->n[0] = inverse * up->n[0];
                up->n[1] = inverse * up->n[1];
                up->n[2] = inverse * up->n[2];
            }

            projectile_distribute(&placement.forward, &placement.up,
                    trigger->projectile_distribution_function,
                    trigger->projectile_distribution_angle, (int16_t)shot, projectile_count);

            /* inherit shooter velocity when the projectile definition requests it, else fixed muzzle speed */
            if ( projectile_def && (projectile_def->projectile.flags & (1u << _projectile_combine_initial_velocity_with_parent_velocity_bit)) )
            {
                object_get_velocities(owner_object_index, &placement.translational_velocity, 0);
            }
            else
            {
                placement.translational_velocity.n[0] = placement.forward.n[0] * inherited_velocity;
                placement.translational_velocity.n[1] = placement.forward.n[1] * inherited_velocity;
                placement.translational_velocity.n[2] = placement.forward.n[2] * inherited_velocity;
            }

            uint8_t force_inside_bsp = owner_unit && owner_unit->unit.player_index != -1;
            if ( force_inside_bsp )
                placement.flags |= 2u;

            int projectile = object_new_with_datum_role_control(&placement, projectile_datum_role);
            if ( projectile != -1 )
            {
                if ( force_inside_bsp )
                {
                    real_point3d camera_position;
                    unit_get_camera_position(owner_object_index, &camera_position);
                    object_force_inside_bsp(projectile, owner_object_index, &camera_position);
                }
                if ( aim_target_index != -1 )
                    projectile_set_target_object_index(projectile, aim_target_index);
                if ( !spawn_tracer )
                    projectile_kill_tracer(projectile);
            }
        }
    }
}
