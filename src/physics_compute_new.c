/* physics_compute_new @0x837BE4E0 — accumulate per-mass-point force/torque for the new vehicle
   physics. Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md
   =========================================================================
   NOTE / CAVEAT: clean decompile, faithfully reconstructed, but one thing is best-effort:
   the object-datum velocity/position/angular-velocity vectors are read at the raw float
   offsets 23/26/35 (bytes 92/104/140) established by the update_alien_scout_physics sibling;
   object_datum.h models these ~4 bytes earlier, so raw offsets are kept here to stay
   consistent with the physics sibling. The force/torque math itself is transcribed verbatim.
   DEVIATION: friction_evaluate's last two args were mis-transcribed as (&mp->velocity,
   &mp-><phase>_friction.friction) — the guess the old caveat flagged. Disasm arbitrates: all
   three call sites load r7 = mp+0x10 (forward) and r8 = mp+0x28 (up), the decomposition axes.
   ========================================================================= */
#include <stdint.h>
#include <string.h>
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/mass_point_flags.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/powered_mass_point_definition.h"
#include "headers/powered_mass_point_definition_flags.h"
#include "headers/friction_datum.h"
#include "headers/material_definition.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
#include "headers/friction_datum.h"
extern float fabsf(float x);  /* DEVIATION: fabs @0x837BEB98/@0x837BECF4 feed fmuls with no frsp - single-precision abs, not the double __fabs */

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void compute_ground_plane(int object_index, mass_point_datum *mass_point, const struct mass_point_definition *mass_point_definition);
extern float scenario_location_water_depth(const location *location, const real_point3d *position);
extern material_definition *scenario_material_definition_get(int16_t material_type);
extern float pin_fraction(float value, float value0, float value1);
extern void friction_evaluate(int16_t type, float parallel_scale, float perpendicular_scale, friction_datum *components, real_vector3d *primary, real_vector3d *secondary);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

void physics_compute_new(const physics_instance *instance, const powered_mass_point_datum *powered_mass_points,
                         mass_point_datum *mass_points, real_vector3d *total_force, real_vector3d *total_torque)
{
    const physics_definition *physics = instance->physics;
    float g = (physics->gravity_scale * global_gravity);
    /* recovered: raw float offsets 23/26/35 (bytes 92/104/140) -> object_datum(+4 header) ->
     * _object_datum.position(0x58)/.translational_velocity(0x64)/.angular_velocity(0x88), DB-verified */
    object_datum *obj = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, instance->object_index)->datum;
    float *obj_position = obj->object.position.n;
    float *obj_velocity = obj->object.translational_velocity.n;
    float *obj_angular_velocity = obj->object.angular_velocity.n;
    int i;

    total_force->n[0] = 0.0f;
    total_force->n[1] = 0.0f;
    total_force->n[2] = -(physics->mass * g);
    total_torque->n[0] = 0.0f;
    total_torque->n[1] = 0.0f;
    total_torque->n[2] = 0.0f;

    memset(mass_points, 0, sizeof(mass_point_datum) * physics->mass_points.count);
    if (physics->mass_points.count <= 0)
        return;

    for (i = 0; i < physics->mass_points.count; ++i)
    {
        /* recovered: (char *)address + 128 * i -> [i] indexing (stride == sizeof(mass_point_definition)) */
        const mass_point_definition *def = &((const mass_point_definition *)physics->mass_points.address)[i];
        mass_point_datum *mp = &mass_points[i];
        powered_mass_point_definition *pmp_def = 0; /* powered mass point DEFINITION */
        const powered_mass_point_datum *pmp = 0;    /* powered mass point DATUM */

        if (def->powered_mass_point_index != -1 && powered_mass_points)
        {
            pmp_def = &((powered_mass_point_definition *)physics->powered_mass_points.address)[def->powered_mass_point_index];
            pmp = &powered_mass_points[def->powered_mass_point_index];
        }

        /* --- transform the mass point into world space --- */
        mp->flags = 0;
        matrix4x3_transform_point(&instance->world_matrix, &def->position, &mp->position);
        if (pmp)
        {
            real_matrix4x3 powered_matrix;
            matrix4x3_multiply(&instance->world_matrix, &pmp->rotation_matrix, &powered_matrix);
            matrix4x3_transform_normal(&powered_matrix, &def->forward, &mp->forward);
            matrix4x3_transform_normal(&powered_matrix, &def->up, &mp->up);
        }
        else
        {
            matrix4x3_transform_normal(&instance->world_matrix, &def->forward, &mp->forward);
            matrix4x3_transform_normal(&instance->world_matrix, &def->up, &mp->up);
        }
        scenario_location_from_point(&mp->location, &mp->position);

        /* radius from the centre of mass */
        mp->radius.n[0] = mp->position.n[0] - obj_position[0];
        mp->radius.n[1] = mp->position.n[1] - obj_position[1];
        mp->radius.n[2] = mp->position.n[2] - obj_position[2];

        /* point velocity = v_cm + omega x r */
        mp->velocity.n[0] = obj_velocity[0] + ((mp->radius.n[2] * obj_angular_velocity[1]) - (mp->radius.n[1] * obj_angular_velocity[2]));
        mp->velocity.n[1] = obj_velocity[1] + ((mp->radius.n[0] * obj_angular_velocity[2]) - (mp->radius.n[2] * obj_angular_velocity[0]));
        mp->velocity.n[2] = obj_velocity[2] + ((mp->radius.n[1] * obj_angular_velocity[0]) - (mp->radius.n[0] * obj_angular_velocity[1]));

        compute_ground_plane(instance->object_index, mp, def);
        mp->water_depth = scenario_location_water_depth(&mp->location, &mp->position);

        /* --- ground contact --- */
        if (mp->ground_depth > 0.0)
        {
            material_definition *material = scenario_material_definition_get(mp->ground_material_type);
            float ground_friction = (material->physics_ground_friction_scale <= 0.0 || physics->mass > 7500.0)
                                    ? physics->ground_friction
                                    : (physics->ground_friction * material->physics_ground_friction_scale);
            float ground_normal_k1 = (material->physics_ground_friction_normal_k1_scale <= 0.0)
                                     ? physics->ground_normal_k1
                                     : (physics->ground_normal_k1 * material->physics_ground_friction_normal_k1_scale);
            float ground_normal_k0 = (material->physics_ground_friction_normal_k0_scale <= 0.0)
                                     ? physics->ground_normal_k0
                                     : (physics->ground_normal_k0 * material->physics_ground_friction_normal_k0_scale);
            float ground_depth_scale = (material->physics_ground_depth_scale <= 0.0)
                                       ? physics->ground_depth
                                       : (physics->ground_depth * material->physics_ground_depth_scale);
            float ground_damp_fraction = physics->ground_damp_fraction;
            float *gn = mp->ground_plane.n.n;
            float damped_normal_velocity;
            float normal_force_magnitude;

            if (material->physics_ground_damp_fraction_scale > 0.0)
                ground_damp_fraction = (physics->ground_damp_fraction * material->physics_ground_damp_fraction_scale);

            damped_normal_velocity = -((mp->velocity.n[0] * gn[0])
                                            + ((gn[2] * mp->velocity.n[2]) + (gn[1] * mp->velocity.n[1])));
            normal_force_magnitude = ((((global_gravity / ground_depth_scale) * mp->ground_depth)
                                                   - (((mp->velocity.n[0] * gn[0])
                                                                   + ((gn[2] * mp->velocity.n[2]) + (gn[1] * mp->velocity.n[1])))
                                                           * ground_damp_fraction))
                                           * physics->mass);
            mp->normal_force_magnitude = normal_force_magnitude;
            mp->normal_force.n[0] = normal_force_magnitude * gn[0];
            mp->normal_force.n[1] = gn[1] * normal_force_magnitude;
            mp->normal_force.n[2] = gn[2] * normal_force_magnitude;

            /* velocity relative to ground (remove normal component) */
            mp->velocity_relative_to_ground.n[0] = (damped_normal_velocity * gn[0]) + mp->velocity.n[0];
            mp->velocity_relative_to_ground.n[1] = (gn[1] * damped_normal_velocity) + mp->velocity.n[1];
            mp->velocity_relative_to_ground.n[2] = (gn[2] * damped_normal_velocity) + mp->velocity.n[2];

            {
                float ground_scale = -(def->mass * ground_friction);
                mp->ground_friction.friction.n[0] = mp->velocity_relative_to_ground.n[0] * ground_scale;
                mp->ground_friction.friction.n[1] = mp->velocity_relative_to_ground.n[1] * ground_scale;
                mp->ground_friction.friction.n[2] = mp->velocity_relative_to_ground.n[2] * ground_scale;

                /* powered ground friction: drive toward the powered velocity along up x plane */
                if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_ground_friction_bit)) != 0 && pmp->ground_friction_velocity != 0.0)
                {
                    float pin = pin_fraction(gn[2], ground_normal_k0, ground_normal_k1);
                    float up_dot = ((gn[0] * mp->up.n[0]) + ((mp->up.n[1] * gn[1]) + (mp->up.n[2] * gn[2])));
                    float weight;
                    float *fwd = mp->forward.n;
                    float along;
                    float px, py, pz;

                    if (up_dot < 0.0f) up_dot = 0.0f;
                    else if (up_dot > 1.0f) up_dot = 1.0f;
                    weight = ((((up_dot * up_dot) * pin) * pin) * ground_scale);

                    along = ((fwd[0] * -pmp->ground_friction_velocity))
                          + ((fwd[1] * -pmp->ground_friction_velocity) + (fwd[2] * -pmp->ground_friction_velocity));
                    /* project the powered velocity into the ground plane */
                    px = (gn[0] * -((gn[0] * along)) ) + (fwd[0] * -pmp->ground_friction_velocity);
                    py = (gn[1] * -((gn[1] * along)) ) + (fwd[1] * -pmp->ground_friction_velocity);
                    pz = (gn[2] * -((gn[2] * along)) ) + (fwd[2] * -pmp->ground_friction_velocity);

                    mp->velocity_relative_to_ground.n[0] += px;
                    mp->velocity_relative_to_ground.n[1] += py;
                    mp->velocity_relative_to_ground.n[2] += pz;
                    mp->ground_friction.friction.n[0] += (px * weight);
                    mp->ground_friction.friction.n[1] += (py * weight);
                    mp->ground_friction.friction.n[2] += (pz * weight);
                }

                friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                                  &mp->ground_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837BEA0C */
            }
        }

        /* --- water contact --- */
        if (mp->water_depth > 0.0)
        {
            float depth_fraction = (mp->water_depth >= physics->water_depth)
                                   ? 1.0f : (mp->water_depth / physics->water_depth);

            if (def->density > 0.0 && physics->water_depth > 0.0)
            {
                float pressure = ((((def->mass / def->density) * physics->water_density) * depth_fraction) * g);
                mp->water_pressure_magnitude = pressure;
                mp->water_pressure.n[0] = 0.0f;
                mp->water_pressure.n[1] = 0.0f;
                mp->water_pressure.n[2] = pressure;
            }

            if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_water_friction_bit)) != 0 && pmp->water_friction_velocity != 0.0)
            {
                float scale = -(def->mass * physics->water_friction);
                float *fwd = mp->forward.n;
                mp->water_friction.friction.n[0] = ((fwd[0] * -pmp->water_friction_velocity) + obj_velocity[0]) * scale;
                mp->water_friction.friction.n[1] = ((fwd[1] * -pmp->water_friction_velocity) + mp->velocity.n[1]) * scale;
                mp->water_friction.friction.n[2] = ((fwd[2] * -pmp->water_friction_velocity) + mp->velocity.n[2]) * scale;
            }
            else
            {
                float scale = -(def->mass * physics->water_friction);
                mp->water_friction.friction.n[0] = scale * obj_velocity[0];
                mp->water_friction.friction.n[1] = scale * mp->velocity.n[1];
                mp->water_friction.friction.n[2] = scale * mp->velocity.n[2];
            }
            friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                              &mp->water_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837BEB3C */

            /* water lift (powered) */
            if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_water_lift_bit)) != 0 && pmp->water_lift_ratio != 0.0)
            {
                float *fwd = mp->forward.n;
                float speed_along = fabsf(((fwd[0] * mp->velocity.n[0])
                                                 + ((fwd[2] * mp->velocity.n[2]) + (fwd[1] * mp->velocity.n[1]))));
                float lift = (((speed_along * pmp->water_lift_ratio) * physics->mass) * depth_fraction);
                mp->powered_force.n[0] += lift * mp->up.n[0];
                mp->powered_force.n[1] += lift * mp->up.n[1];
                mp->powered_force.n[2] += lift * mp->up.n[2];
            }
        }

        /* --- air friction --- */
        if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_air_friction_bit)) != 0 && pmp->air_friction_velocity != 0.0)
        {
            float scale = -(def->mass * physics->air_friction);
            float *fwd = mp->forward.n;
            mp->air_friction.friction.n[0] = ((fwd[0] * -pmp->air_friction_velocity) + obj_velocity[0]) * scale;
            mp->air_friction.friction.n[1] = ((fwd[1] * -pmp->air_friction_velocity) + mp->velocity.n[1]) * scale;
            mp->air_friction.friction.n[2] = ((fwd[2] * -pmp->air_friction_velocity) + mp->velocity.n[2]) * scale;
        }
        else
        {
            float scale = -(def->mass * physics->air_friction);
            mp->air_friction.friction.n[0] = scale * obj_velocity[0];
            mp->air_friction.friction.n[1] = scale * mp->velocity.n[1];
            mp->air_friction.friction.n[2] = scale * mp->velocity.n[2];
        }
        friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                          &mp->air_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837BEC98 */

        /* air lift (powered) */
        if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_air_lift_bit)) != 0 && pmp->air_lift_ratio != 0.0)
        {
            float *fwd = mp->forward.n;
            float speed_along = fabsf(((fwd[0] * mp->velocity.n[0])
                                             + ((fwd[2] * mp->velocity.n[2]) + (fwd[1] * mp->velocity.n[1]))));
            float lift = ((speed_along * physics->mass) * pmp->air_lift_ratio);
            mp->powered_force.n[0] += lift * mp->up.n[0];
            mp->powered_force.n[1] += lift * mp->up.n[1];
            mp->powered_force.n[2] += lift * mp->up.n[2];
        }

        /* --- state flags: moving / on-ground / in-water --- */
        {
            float speed_sq = ((mp->velocity.n[2] * mp->velocity.n[2])
                                   + ((obj_velocity[0] * obj_velocity[0]) + (mp->velocity.n[1] * mp->velocity.n[1])));
            /* runtime mass-point state bits: enum mass_point_flags
             * (DB $35EE1D98A1945FD2941159B669ED5A99) */
            mp->flags = (speed_sq >= 0.0011111111) ? (mp->flags & ~0x1u) : (mp->flags | 1);
            mp->flags = (mp->ground_depth <= 0.0) ? (mp->flags & ~(1u << _point_on_ground_bit))
                                                  : (mp->flags | (1u << _point_on_ground_bit));
            mp->flags = (mp->water_depth <= 0.0) ? (mp->flags & ~(1u << _point_in_water_bit))
                                                 : (mp->flags | (1u << _point_in_water_bit));
        }

        /* --- powered thrust + antigravity --- */
        if (pmp_def)
        {
            float *fwd = mp->forward.n;
            if ((pmp_def->flags & (1u << _powered_mass_point_thrust_bit)) != 0)
            {
                float thrust = (pmp->thrust_fraction * physics->mass);
                mp->powered_force.n[0] += fwd[0] * thrust;
                mp->powered_force.n[1] += fwd[1] * thrust;
                mp->powered_force.n[2] += fwd[2] * thrust;
            }
            if ((pmp_def->flags & (1u << _powered_mass_point_antigrav_bit)) != 0)
            {
                real_point3d probe_origin;
                real_vector3d probe_delta;
                collision_result probe;
                float probe_length = (def->radius + pmp_def->antigrav_height);

                probe_origin.n[0] = mp->position.n[0];
                probe_origin.n[1] = mp->position.n[1];
                probe_origin.n[2] = mp->position.n[2];
                probe_delta.n[0] = probe_length * global_down3d->n[0];
                probe_delta.n[1] = global_down3d->n[1] * probe_length;
                probe_delta.n[2] = global_down3d->n[2] * probe_length;

                if (collision_test_vector(_collision_test_for_bipeds_dead_flags, &probe_origin, &probe_delta, instance->object_index, &probe))
                {
                    float lo = pmp_def->antigrav_normal_k1;
                    float hi = pmp_def->antigrav_normal_k0;
                    float up_z = mp->up.n[2];
                    float align;         /* v98 */
                    float ground_effect; /* v101 */
                    float magnitude;     /* v104 */

                    if (hi >= lo)
                    {
                        if (up_z <= lo) align = 1.0f;
                        else if (up_z >= hi) align = 0.0f;
                        else align = ((hi - up_z) / (hi - lo));
                    }
                    else
                    {
                        if (up_z <= hi) align = 0.0f;
                        else if (up_z >= lo) align = 1.0f;
                        else align = ((up_z - hi) / (lo - hi));
                    }

                    if (((probe_length * probe.t) - def->radius) <= 0.0f)
                        ground_effect = 1.0f;
                    else
                        ground_effect = (1.0f - (((probe_length * probe.t) - def->radius) / pmp_def->antigrav_height));

                    magnitude = ((((((((ground_effect * ground_effect) * global_gravity)
                                                                              - (((probe.plane.n.n[2] * mp->velocity.n[2])
                                                                                              + ((probe.plane.n.n[0] * mp->velocity.n[0])
                                                                                                      + (probe.plane.n.n[1] * mp->velocity.n[1])))
                                                                                      * pmp_def->antigrav_damp_fraction))
                                                                      * pmp->antigrav_fraction)
                                                              * pmp_def->antigrav_strength)
                                                      * physics->mass)
                                              * align)); /* balanced missing outer float-cast paren */
                    mp->powered_force.n[0] += probe.plane.n.n[0] * magnitude;
                    mp->powered_force.n[1] += probe.plane.n.n[1] * magnitude;
                    mp->powered_force.n[2] += probe.plane.n.n[2] * magnitude;
                    mp->flags |= (1u << _point_antigraving_bit);
                }
            }
        }

        /* --- accumulate: force = sum of all contributions; torque = r x force --- */
        mp->force.n[0] = mp->normal_force.n[0] + mp->ground_friction.friction.n[0] + mp->water_pressure.n[0]
                       + mp->water_friction.friction.n[0] + mp->air_friction.friction.n[0] + mp->powered_force.n[0];
        mp->force.n[1] = mp->normal_force.n[1] + mp->ground_friction.friction.n[1] + mp->water_pressure.n[1]
                       + mp->water_friction.friction.n[1] + mp->air_friction.friction.n[1] + mp->powered_force.n[1];
        mp->force.n[2] = mp->normal_force.n[2] + mp->ground_friction.friction.n[2] + mp->water_pressure.n[2]
                       + mp->water_friction.friction.n[2] + mp->air_friction.friction.n[2] + mp->powered_force.n[2];

        mp->torque.n[0] = (mp->force.n[2] * mp->radius.n[1]) - (mp->radius.n[2] * mp->force.n[1]);
        mp->torque.n[1] = (mp->radius.n[2] * mp->force.n[0]) - (mp->radius.n[0] * mp->force.n[2]);
        mp->torque.n[2] = (mp->radius.n[0] * mp->force.n[1]) - (mp->radius.n[1] * mp->force.n[0]);

        total_force->n[0] += mp->force.n[0];
        total_force->n[1] += mp->force.n[1];
        total_force->n[2] += mp->force.n[2];
        total_torque->n[0] += mp->torque.n[0];
        total_torque->n[1] += mp->torque.n[1];
        total_torque->n[2] += mp->torque.n[2];
    }
}
