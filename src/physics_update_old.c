/* =========================================================================
   physics_update_old @0x837C0268 — legacy single-step rigid-body integrator for a Blam
   object. Rebuilds the object world matrix from its current basis, accumulates
   the per-mass-point forces/torques exactly as physics_compute_new does (ground
   contact, water pressure/friction, air friction, powered thrust/lift/antigrav),
   then INTEGRATES: force/mass -> linear velocity delta, torque about the torque
   axis / moment of inertia -> angular velocity delta. Finally it translates the
   object, rotates+re-orthonormalizes its forward/up basis, and updates the
   object "at rest" state flags.

   NOTE / CAVEAT (best-effort, mirrors the physics_compute_new sibling):
   (1) The object datum runtime payload is reached as (_object_datum *)(object + 4)
       (the 4-byte definition-index wrapper); every field used here is DB-verified
       against object_datum.h at that +4 base.
   (2) The powered-mass-point DEFINITION is a raw tag block with no reconstructed
       struct in the corpus, so its fields are read at raw byte offsets (flags@32,
       radius@44, antigrav_scale@36, damp@48, k0@52/k1@56) exactly as the sibling.
   (3) The decompiler spilled the running torque accumulator into the int16 words
       of the stack `location` slot that is later reused as the translation
       location output; those dead spill writes are dropped and the torque is kept
       in plain float accumulators.
   DEVIATION: friction_evaluate's last two args were mis-transcribed as (&mp->velocity,
       &mp-><phase>_friction.friction) — the guess the old caveat (2) flagged. Disasm
       arbitrates: all three binary call sites set r7 = mp+0x10 (forward) and r8 =
       mp+0x28 (up), the decomposition axes, with r6 = the phase's friction_datum
       (mp+0x90 / +0xC4 / +0xE8). The callee reads r4/r5 nowhere — six args exactly.
   ========================================================================= */
#include <stdint.h>
#include <string.h>
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/powered_mass_point_definition.h"
#include "headers/powered_mass_point_definition_flags.h"
#include "headers/friction_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/collision_result.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/_object_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
#include "headers/material_type.h"

#include "headers/real_quaternion.h"
#include "headers/friction_datum.h"
extern double sin(double x);
extern double cos(double x);
extern float __fsqrts(float x);
extern double __fabs(double x);
extern float fabsf(float x);  /* DEVIATION: fabs @0x837C0A24/@0x837C0B80 feed fmuls with no frsp - single-precision abs, not the double __fabs */

extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_matrix4x3 *matrix4x3_rotation_from_quaternion(real_matrix4x3 *matrix, const real_quaternion *quaternion);
extern void matrix4x3_transpose(real_matrix4x3 *matrix);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void scenario_location_from_line(location *out_location, const location *start_location, const real_point3d *start_point, const real_point3d *end_point);
extern float scenario_location_water_depth(const location *location, const real_point3d *position);
extern void compute_ground_plane(int object_index, mass_point_datum *mass_point, const struct mass_point_definition *mass_point_definition);
extern void friction_evaluate(int16_t type, float parallel_scale, float perpendicular_scale, friction_datum *components, real_vector3d *primary, real_vector3d *secondary);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);

void physics_update_old(int object_index, powered_mass_point_datum *powered_mass_points,
                        mass_point_datum *mass_points, const real_vector3d *magic_force,
                        const real_vector3d *magic_torque)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_datum *od = &object->object;

    /* object -> object definition tag -> physics tag -> physics_definition */
    _object_definition *object_definition = TAG_GET(_object_definition, object->definition_index);
    const physics_definition *physics = TAG_GET(const physics_definition, object_definition->physics.index);

    float g = (physics->gravity_scale * global_gravity);
    float gravity_force = (physics->mass * g);
    real_matrix4x3 world_matrix[3];

    /* accumulated totals (linear force in x/y/z, torque in x/y/z) */
    float total_force_x = 0.0f;
    float total_force_y = 0.0f;
    float total_force_z = -gravity_force;
    float total_torque_x = 0.0f;
    float total_torque_y = 0.0f;
    float total_torque_z = 0.0f;

    /* integration outputs */
    float velocity_delta_x = 0.0f, velocity_delta_y = 0.0f, velocity_delta_z = 0.0f;
    float angular_velocity_delta_x = 0.0f, angular_velocity_delta_y = 0.0f, angular_velocity_delta_z = 0.0f;

    /* per-object contact tallies over the mass points */
    int16_t stopped_count = 0;
    int16_t ground_contact_count = 0;
    int16_t secondary_contact_count = 0;
    int16_t water_contact_count = 0;

    int mass_point_count;
    int i;

    matrix4x3_from_point_and_vectors(world_matrix, &od->position, &od->forward, &od->up);

    /* build each powered mass point's rotation matrix (transpose of the quaternion basis) */
    if (powered_mass_points && physics->powered_mass_points.count > 0)
    {
        int p = 0;
        do
        {
            powered_mass_point_datum *pmp = &powered_mass_points[p];
            matrix4x3_rotation_from_quaternion(&pmp->rotation_matrix, &pmp->rotation);
            matrix4x3_transpose(&pmp->rotation_matrix);
            p = (int16_t)(p + 1);
        } while (p < physics->powered_mass_points.count);
    }

    memset(mass_points, 0, sizeof(mass_point_datum) * physics->mass_points.count);

    /* external "magic" force / torque applied to the whole body */
    if (magic_force)
    {
        total_force_x = magic_force->n[0];
        total_force_z = (magic_force->n[2] + total_force_z);
        total_force_y = magic_force->n[1];
    }
    if (magic_torque)
    {
        total_torque_x = magic_torque->n[0];
        total_torque_y = magic_torque->n[1];
        total_torque_z = magic_torque->n[2];
    }

    mass_point_count = physics->mass_points.count;
    if (mass_point_count > 0)
    {
        i = 0;
        while (1)
        {
            /* recovered: (i << 7) + (char *)address -> [i] indexing (stride == sizeof(mass_point_definition)) */
            const mass_point_definition *def =
                &((const mass_point_definition *)physics->mass_points.address)[i];
            mass_point_datum *mp = &mass_points[i];
            int16_t powered_index = def->powered_mass_point_index;
            powered_mass_point_definition *pmp_def = 0; /* powered mass point DEFINITION */
            powered_mass_point_datum *pmp = 0;          /* powered mass point DATUM */
            const real_matrix4x3 *normal_matrix;
            float *mp_position = mp->position.n;
            float *mp_forward = mp->forward.n;
            float *mp_up = mp->up.n;
            float *mp_velocity = mp->velocity.n;
            real_point3d local_position;

            if (powered_index != -1 && powered_mass_points)
            {
                pmp_def = &((powered_mass_point_definition *)physics->powered_mass_points.address)[powered_index];
                if (pmp_def)
                {
                    pmp = &powered_mass_points[powered_index];
                }
            }

            /* --- transform the mass point into world space --- */
            mp->flags = 0;
            local_position.n[0] = def->position.n[0] - physics->center_of_mass.n[0];
            local_position.n[1] = def->position.n[1] - physics->center_of_mass.n[1];
            local_position.n[2] = def->position.n[2] - physics->center_of_mass.n[2];
            matrix4x3_transform_point(world_matrix, &local_position, &mp->position);

            if (pmp)
            {
                real_matrix4x3 powered_matrix;
                matrix4x3_multiply(world_matrix, &pmp->rotation_matrix, &powered_matrix);
                matrix4x3_transform_normal(&powered_matrix, &def->forward, &mp->forward);
                normal_matrix = &powered_matrix;
            }
            else
            {
                matrix4x3_transform_normal(world_matrix, &def->forward, &mp->forward);
                normal_matrix = world_matrix;
            }
            matrix4x3_transform_normal(normal_matrix, &def->up, &mp->up);
            scenario_location_from_point(&mp->location, &mp->position);

            /* radius from the centre of mass (object world position) */
            mp->radius.n[0] = mp_position[0] - od->position.n[0];
            mp->radius.n[1] = mp->position.n[1] - od->position.n[1];
            mp->radius.n[2] = mp->position.n[2] - od->position.n[2];

            /* point velocity = v_cm + omega x r */
            mp->velocity.n[1] = (od->angular_velocity.n[2] * mp->radius.n[0])
                              - (mp->radius.n[2] * od->angular_velocity.n[0]);
            mp->velocity.n[0] = (mp->radius.n[2] * od->angular_velocity.n[1])
                              - (od->angular_velocity.n[2] * mp->radius.n[1]);
            mp->velocity.n[2] = (od->angular_velocity.n[0] * mp->radius.n[1])
                              - (mp->radius.n[0] * od->angular_velocity.n[1]);
            mp->velocity.n[0] = od->translational_velocity.n[0] + mp->velocity.n[0];
            mp->velocity.n[1] = od->translational_velocity.n[1] + mp->velocity.n[1];
            mp->velocity.n[2] = od->translational_velocity.n[2] + mp->velocity.n[2];

            compute_ground_plane(object_index, mp, def);
            mp->water_depth = scenario_location_water_depth(&mp->location, &mp->position);

            /* --- ground contact --- */
            if (mp->ground_depth > 0.0 && physics->ground_depth > 0.0)
            {
                float *gn = mp->ground_plane.n.n;
                real_vector3d *ground_friction_out = &mp->ground_friction.friction;
                float damped_normal_velocity = -((gn[1] * mp->velocity.n[1])
                                                      + ((gn[0] * mp->velocity.n[0]) + (gn[2] * mp->velocity.n[2])));
                float normal_force_magnitude =
                    ((((mp->ground_depth / physics->ground_depth) * global_gravity)
                                  - (physics->ground_damp_fraction
                                          * ((gn[1] * mp->velocity.n[1])
                                                  + ((gn[0] * mp->velocity.n[0]) + (gn[2] * mp->velocity.n[2])))))
                          * physics->mass);
                float ground_scale = (def->mass * physics->ground_friction);

                mp->normal_force_magnitude = normal_force_magnitude;
                mp->normal_force.n[0] = gn[0] * normal_force_magnitude;
                mp->normal_force.n[1] = normal_force_magnitude * gn[1];
                mp->normal_force.n[2] = normal_force_magnitude * gn[2];

                mp->velocity_relative_to_ground.n[0] = (gn[0] * damped_normal_velocity) + mp_velocity[0];
                mp->velocity_relative_to_ground.n[1] = (damped_normal_velocity * gn[1]) + mp->velocity.n[1];
                mp->velocity_relative_to_ground.n[2] = (damped_normal_velocity * gn[2]) + mp->velocity.n[2];
                mp->ground_friction.friction.n[0] = mp->velocity_relative_to_ground.n[0] * -ground_scale;
                mp->ground_friction.friction.n[1] = -ground_scale * mp->velocity_relative_to_ground.n[1];
                mp->ground_friction.friction.n[2] = -ground_scale * mp->velocity_relative_to_ground.n[2];

                /* powered ground friction: drive the point toward the powered velocity in the ground plane */
                if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_ground_friction_bit)) != 0 && pmp->ground_friction_velocity != 0.0)
                {
                    float k1 = physics->ground_normal_k1;
                    float k0 = physics->ground_normal_k0;
                    float pin;
                    float up_dot;
                    float drive = -pmp->ground_friction_velocity;
                    float along;
                    float drive_x, drive_y, drive_z;
                    float weight;

                    if (k0 >= k1)
                    {
                        if (gn[2] > k1)
                        {
                            if (gn[2] < k0)
                                pin = ((k0 - gn[2]) / (k0 - k1));
                            else
                                pin = 0.0f;
                        }
                        else
                            pin = 1.0f;
                    }
                    else
                    {
                        if (gn[2] <= k0)
                            pin = 0.0f;
                        else if (gn[2] >= k1)
                            pin = 1.0f;
                        else
                            pin = ((gn[2] - k0) / (k1 - k0));
                    }

                    up_dot = ((gn[0] * mp_up[0]) + ((mp_up[1] * gn[1]) + (mp_up[2] * gn[2])));
                    if (up_dot >= 0.0f)
                    {
                        if (up_dot > 1.0f)
                            up_dot = 1.0f;
                    }
                    else
                        up_dot = 0.0f;

                    along = ((mp_forward[0] * drive) + ((mp_forward[1] * drive) + (mp_forward[2] * drive)));
                    drive_x = (-along * gn[0]) + (mp_forward[0] * drive);
                    drive_y = (-along * gn[1]) + (mp_forward[1] * drive);
                    drive_z = (-along * gn[2]) + (mp_forward[2] * drive);
                    weight = ((((up_dot * up_dot) * pin) * pin) * -ground_scale);

                    mp->velocity_relative_to_ground.n[0] = mp->velocity_relative_to_ground.n[0] + drive_x;
                    mp->velocity_relative_to_ground.n[1] = drive_y + mp->velocity_relative_to_ground.n[1];
                    mp->velocity_relative_to_ground.n[2] = drive_z + mp->velocity_relative_to_ground.n[2];
                    ground_friction_out->n[0] = (drive_x * weight) + ground_friction_out->n[0];
                    mp->ground_friction.friction.n[1] = (drive_y * weight) + mp->ground_friction.friction.n[1];
                    mp->ground_friction.friction.n[2] = (drive_z * weight) + mp->ground_friction.friction.n[2];
                }

                if (mp->ground_material_type == _material_ice)  /* ice: quarter friction */
                {
                    friction_evaluate(def->friction_type,
                                      (def->friction_parallel_scale * (float)0.125),
                                      (def->friction_perpendicular_scale * (float)0.125),
                                      &mp->ground_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837C08A0 */
                }
                else
                {
                    friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                                      &mp->ground_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837C08A0 */
                }
            }

            /* --- water contact --- */
            if (mp->water_depth > 0.0)
            {
                float water_depth_limit = physics->water_depth;
                float depth_fraction = (mp->water_depth >= water_depth_limit)
                                       ? 1.0f : (mp->water_depth / physics->water_depth);

                if (def->density > 0.0 && water_depth_limit > 0.0)
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
                    mp->water_friction.friction.n[0] = scale * ((-pmp->water_friction_velocity * mp_forward[0]) + mp_velocity[0]);
                    mp->water_friction.friction.n[1] = ((mp_forward[1] * -pmp->water_friction_velocity) + mp->velocity.n[1]) * scale;
                    mp->water_friction.friction.n[2] = ((mp_forward[2] * -pmp->water_friction_velocity) + mp->velocity.n[2]) * scale;
                }
                else
                {
                    float scale = -(def->mass * physics->water_friction);
                    mp->water_friction.friction.n[0] = scale * mp_velocity[0];
                    mp->water_friction.friction.n[1] = scale * mp->velocity.n[1];
                    mp->water_friction.friction.n[2] = scale * mp->velocity.n[2];
                }
                friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                                  &mp->water_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837C09C8 */

                /* water lift (powered) */
                if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_water_lift_bit)) != 0 && pmp->water_lift_ratio != 0.0)
                {
                    float speed_along = fabsf(((mp_forward[0] * mp->velocity.n[0])
                                                     + ((mp_forward[2] * mp->velocity.n[2]) + (mp_forward[1] * mp->velocity.n[1]))));
                    float lift = ((((speed_along * pmp->water_lift_ratio) * physics->mass) * depth_fraction));
                    mp->powered_force.n[0] = ((((speed_along * pmp->water_lift_ratio) * physics->mass) * depth_fraction) * mp_up[0])
                                           + mp->powered_force.n[0];
                    mp->powered_force.n[1] = (mp->up.n[1] * lift) + mp->powered_force.n[1];
                    mp->powered_force.n[2] = (mp->up.n[2] * lift) + mp->powered_force.n[2];
                }
            }

            /* --- air friction --- */
            if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_air_friction_bit)) != 0 && pmp->air_friction_velocity != 0.0)
            {
                float scale = -(def->mass * physics->air_friction);
                mp->air_friction.friction.n[0] = scale * ((-pmp->air_friction_velocity * mp_forward[0]) + mp_velocity[0]);
                mp->air_friction.friction.n[1] = ((mp_forward[1] * -pmp->air_friction_velocity) + mp->velocity.n[1]) * scale;
                mp->air_friction.friction.n[2] = ((mp_forward[2] * -pmp->air_friction_velocity) + mp->velocity.n[2]) * scale;
            }
            else
            {
                float scale = -(def->mass * physics->air_friction);
                mp->air_friction.friction.n[0] = scale * mp_velocity[0];
                mp->air_friction.friction.n[1] = scale * mp->velocity.n[1];
                mp->air_friction.friction.n[2] = scale * mp->velocity.n[2];
            }
            friction_evaluate(def->friction_type, def->friction_parallel_scale, def->friction_perpendicular_scale,
                              &mp->air_friction, &mp->forward, &mp->up); /* DEVIATION: r7=mp+0x10, r8=mp+0x28 @0x837C0B24 */

            /* air lift (powered) */
            if (pmp_def && (pmp_def->flags & (1u << _powered_mass_point_air_lift_bit)) != 0 && pmp->air_lift_ratio != 0.0)
            {
                float speed_along = fabsf(((mp_forward[0] * mp->velocity.n[0])
                                                 + ((mp_forward[2] * mp->velocity.n[2]) + (mp_forward[1] * mp->velocity.n[1]))));
                float lift = ((speed_along * pmp->air_lift_ratio) * physics->mass);
                mp->powered_force.n[0] = (((speed_along * pmp->air_lift_ratio) * physics->mass) * mp_up[0]) + mp->powered_force.n[0];
                mp->powered_force.n[1] = (mp->up.n[1] * lift) + mp->powered_force.n[1];
                mp->powered_force.n[2] = (mp->up.n[2] * lift) + mp->powered_force.n[2];
            }

            /* --- state flags: moving / on-ground / in-water --- */
            {
                unsigned int flags = mp->flags | 1;
                float speed_sq = ((mp_velocity[1] * mp_velocity[1])
                                       + ((mp_velocity[0] * mp_velocity[0]) + (mp_velocity[2] * mp_velocity[2])));
                if (speed_sq >= 0.0011111111)
                    flags = mp->flags & ~0x1u;   /* clear stopped bit */
                mp->flags = flags;
                flags |= 2;
                if (mp->ground_depth <= 0.0)
                    flags = mp->flags & ~0x2u;   /* clear ground-contact bit */
                mp->flags = flags;
                flags |= 8;
                if (mp->water_depth <= 0.0)
                    flags = mp->flags & ~0x8u;   /* clear in-water bit */
                mp->flags = flags;

                stopped_count += flags & 1;
                ground_contact_count += (flags >> 1) & 1;
                secondary_contact_count += (flags >> 2) & 1;
                water_contact_count += (flags >> 3) & 1;
            }

            /* --- powered thrust + antigravity --- */
            if (pmp_def)
            {
                if ((pmp_def->flags & (1u << _powered_mass_point_thrust_bit)) != 0)
                {
                    float thrust = (pmp->thrust_fraction * physics->mass);
                    mp->powered_force.n[0] = (mp_forward[0] * (pmp->thrust_fraction * physics->mass)) + mp->powered_force.n[0];
                    mp->powered_force.n[1] = (mp_forward[1] * thrust) + mp->powered_force.n[1];
                    mp->powered_force.n[2] = (mp_forward[2] * thrust) + mp->powered_force.n[2];
                }
                if ((pmp_def->flags & (1u << _powered_mass_point_antigrav_bit)) != 0)
                {
                    real_point3d probe_origin;
                    real_vector3d probe_delta;
                    collision_result probe;
                    float probe_length = (pmp_def->antigrav_height + def->radius);

                    probe_origin.n[0] = mp_position[0];
                    probe_origin.n[1] = mp->position.n[1];
                    probe_origin.n[2] = mp->position.n[2];
                    probe_delta.n[0] = probe_length * global_down3d->n[0];
                    probe_delta.n[1] = global_down3d->n[1] * probe_length;
                    probe_delta.n[2] = global_down3d->n[2] * probe_length;

                    if (collision_test_vector(_collision_test_for_bipeds_dead_flags, &probe_origin, &probe_delta, object_index, &probe))
                    {
                        float lo = pmp_def->antigrav_normal_k1;
                        float hi = pmp_def->antigrav_normal_k0;
                        float up_z = mp->up.n[2];
                        float align;
                        float ground_effect;
                        float magnitude;

                        if (hi >= lo)
                        {
                            if (up_z > lo)
                            {
                                if (up_z < hi)
                                    align = ((hi - up_z) / (hi - lo));
                                else
                                    align = 0.0f;
                            }
                            else
                                align = 1.0f;
                        }
                        else if (up_z > hi)
                        {
                            if (up_z < lo)
                                align = ((up_z - hi) / (lo - hi));
                            else
                                align = 1.0f;
                        }
                        else
                            align = 0.0f;

                        if ((((def->radius + pmp_def->antigrav_height) * probe.t) - def->radius) <= 0.0)
                            ground_effect = 1.0f;
                        else
                            ground_effect = ((float)1.0
                                                  - ((((def->radius + pmp_def->antigrav_height) * probe.t) - def->radius)
                                                          / pmp_def->antigrav_height));

                        magnitude = ((((((((ground_effect * ground_effect) * global_gravity)
                                                                                  - (((probe.plane.n.n[0] * mp->velocity.n[0])
                                                                                                  + ((probe.plane.n.n[1] * mp->velocity.n[1])
                                                                                                          + (probe.plane.n.n[2] * mp->velocity.n[2])))
                                                                                          * pmp_def->antigrav_damp_fraction))
                                                                          * pmp->antigrav_fraction)
                                                                  * pmp_def->antigrav_strength)
                                                          * physics->mass)
                                                  * align)); /* balanced missing outer float-cast paren */
                        mp->powered_force.n[0] = (probe.plane.n.n[0] * magnitude) + mp->powered_force.n[0];
                        mp->powered_force.n[1] = (probe.plane.n.n[1] * magnitude) + mp->powered_force.n[1];
                        mp->powered_force.n[2] = (probe.plane.n.n[2] * magnitude) + mp->powered_force.n[2];
                    }
                }
            }

            /* --- accumulate: force = sum of all contributions; torque = r x force --- */
            mp->force.n[0] = mp->force.n[0] + mp->normal_force.n[0];
            mp->force.n[1] = mp->normal_force.n[1] + mp->force.n[1];
            mp->force.n[2] = mp->normal_force.n[2] + mp->force.n[2];
            mp->force.n[0] = mp->force.n[0] + mp->ground_friction.friction.n[0];
            mp->force.n[1] = mp->ground_friction.friction.n[1] + mp->force.n[1];
            mp->force.n[2] = mp->ground_friction.friction.n[2] + mp->force.n[2];
            mp->force.n[0] = mp->force.n[0] + mp->water_pressure.n[0];
            mp->force.n[1] = mp->water_pressure.n[1] + mp->force.n[1];
            mp->force.n[2] = mp->water_pressure.n[2] + mp->force.n[2];
            mp->force.n[0] = mp->force.n[0] + mp->water_friction.friction.n[0];
            mp->force.n[1] = mp->water_friction.friction.n[1] + mp->force.n[1];
            mp->force.n[2] = mp->water_friction.friction.n[2] + mp->force.n[2];
            mp->force.n[0] = mp->force.n[0] + mp->air_friction.friction.n[0];
            mp->force.n[1] = mp->force.n[1] + mp->air_friction.friction.n[1];
            mp->force.n[2] = mp->force.n[2] + mp->air_friction.friction.n[2];
            mp->force.n[0] = mp->force.n[0] + mp->powered_force.n[0];
            mp->force.n[1] = mp->powered_force.n[1] + mp->force.n[1];
            mp->force.n[2] = mp->powered_force.n[2] + mp->force.n[2];

            mp->torque.n[0] = (mp->force.n[2] * mp->radius.n[1]) - (mp->radius.n[2] * mp->force.n[1]);
            i = (int16_t)(i + 1);
            mp->torque.n[2] = (mp->radius.n[0] * mp->force.n[1]) - (mp->radius.n[1] * mp->force.n[0]);
            mp->torque.n[1] = (mp->radius.n[2] * mp->force.n[0]) - (mp->radius.n[0] * mp->force.n[2]);

            total_torque_x = (total_torque_x + mp->torque.n[0]);
            mass_point_count = physics->mass_points.count;
            total_torque_y = (total_torque_y + mp->torque.n[1]);
            total_torque_z = (total_torque_z + mp->torque.n[2]);
            total_force_y = (total_force_y + mp->force.n[1]);
            total_force_z = (total_force_z + mp->force.n[2]);
            total_force_x = (mp->force.n[0] + total_force_x);

            if (i >= mass_point_count)
                break;
        }
    }

    /* --- integrate linear: velocity delta = force / mass --- */
    if (physics->mass != 0.0)
    {
        velocity_delta_x = (((float)1.0 / physics->mass) * total_force_x);
        velocity_delta_y = (total_force_y * ((float)1.0 / physics->mass));
        velocity_delta_z = (total_force_z * ((float)1.0 / physics->mass));
    }

    /* --- integrate angular: torque about its own axis / moment of inertia --- */
    {
        float torque_magnitude = __fsqrts(((total_torque_x * total_torque_x)
                                                 + ((total_torque_z * total_torque_z) + (total_torque_y * total_torque_y))));
        if (__fabs(torque_magnitude) >= 0.000099999997)
        {
            float axis_x = (((float)1.0 / torque_magnitude) * total_torque_x);
            float axis_y = (total_torque_y * ((float)1.0 / torque_magnitude));
            float axis_z = (total_torque_z * ((float)1.0 / torque_magnitude));
            if (torque_magnitude != 0.0)
            {
                float inertia = 0.0f;
                if (mass_point_count > 0)
                {
                    int j = 0;
                    do
                    {
                        mass_point_datum *mp = &mass_points[j];
                        /* recovered: (j << 7) + (char *)address -> [j] indexing (stride == sizeof(mass_point_definition)) */
                        const mass_point_definition *def =
                            &((const mass_point_definition *)physics->mass_points.address)[j];
                        float radius_dot_axis;
                        float perp_x, perp_y, perp_z;

                        j = (int16_t)(j + 1);
                        radius_dot_axis = ((mp->radius.n[0] * axis_x)
                                                + ((mp->radius.n[1] * axis_y) + (mp->radius.n[2] * axis_z)));
                        perp_x = (-radius_dot_axis * axis_x) + mp->radius.n[0];
                        perp_z = (axis_z * -radius_dot_axis) + mp->radius.n[2];
                        perp_y = (axis_y * -radius_dot_axis) + mp->radius.n[1];
                        inertia = (((((perp_y * perp_y)
                                                                + ((perp_z * perp_z)
                                                                        + ((perp_x * perp_x)
                                                                                + ((def->radius * def->radius) * (float)0.40000001))))
                                                        * def->mass)
                                                * physics->moment)
                                        + inertia);
                    } while (j < mass_point_count);

                    if (inertia != 0.0)
                    {
                        float inverse_inertia = ((float)1.0 / inertia);
                        angular_velocity_delta_x = (inverse_inertia * total_torque_x);
                        angular_velocity_delta_y = (total_torque_y * inverse_inertia);
                        angular_velocity_delta_z = (total_torque_z * inverse_inertia);
                    }
                }
            }
        }
    }

    /* --- apply the integrated deltas to the object --- */
    od->translational_velocity.n[0] = velocity_delta_x + od->translational_velocity.n[0];
    od->translational_velocity.n[1] = velocity_delta_y + od->translational_velocity.n[1];
    od->translational_velocity.n[2] = velocity_delta_z + od->translational_velocity.n[2];
    od->angular_velocity.n[0] = angular_velocity_delta_x + od->angular_velocity.n[0];
    od->angular_velocity.n[1] = angular_velocity_delta_y + od->angular_velocity.n[1];
    od->angular_velocity.n[2] = angular_velocity_delta_z + od->angular_velocity.n[2];

    /* translate the object by its (now updated) velocity */
    {
        real_point3d new_position;
        location new_location;
        real_vector3d rotation_axis;
        float angular_speed;

        new_position.n[0] = od->position.n[0] + od->translational_velocity.n[0];
        new_position.n[1] = od->position.n[1] + od->translational_velocity.n[1];
        new_position.n[2] = od->position.n[2] + od->translational_velocity.n[2];
        scenario_location_from_line(&new_location, (const location *)&od->location, &od->position, &new_position);
        object_translate(object_index, &new_position, &new_location);

        /* rotate the forward/up basis about the angular velocity axis by |omega| */
        rotation_axis.n[0] = od->angular_velocity.n[0];
        rotation_axis.n[1] = od->angular_velocity.n[1];
        rotation_axis.n[2] = od->angular_velocity.n[2];
        angular_speed = __fsqrts(((rotation_axis.n[0] * rotation_axis.n[0])
                                       + ((rotation_axis.n[1] * rotation_axis.n[1]) + (rotation_axis.n[2] * rotation_axis.n[2]))));
        if (__fabs(angular_speed) >= 0.000099999997)
        {
            rotation_axis.n[0] = ((float)1.0 / angular_speed) * rotation_axis.n[0];
            rotation_axis.n[1] = rotation_axis.n[1] * ((float)1.0 / angular_speed);
            rotation_axis.n[2] = rotation_axis.n[2] * ((float)1.0 / angular_speed);
            if (angular_speed != 0.0)
            {
                float sine = (float)sin(angular_speed);
                float cosine = (float)cos(angular_speed);
                float *fwd = od->forward.n;
                float *up = od->up.n;
                float up_dot_fwd;
                float fwd_len;
                float new_up_len;
                float new_up_x, new_up_y;

                rotate_vector_about_axis(&od->forward, &rotation_axis, sine, cosine);
                rotate_vector_about_axis(&od->up, &rotation_axis, sine, cosine);

                /* re-normalize forward */
                fwd_len = __fsqrts(((fwd[1] * fwd[1]) + ((fwd[0] * fwd[0]) + (fwd[2] * fwd[2]))));
                if (__fabs(fwd_len) >= 0.000099999997)
                {
                    float inverse_len = ((float)1.0 / fwd_len);
                    fwd[0] = inverse_len * fwd[0];
                    fwd[1] = inverse_len * fwd[1];
                    fwd[2] = inverse_len * fwd[2];
                }

                /* Gram-Schmidt: orthogonalize up against forward, then normalize */
                up_dot_fwd = -((fwd[1] * up[1]) + ((up[2] * fwd[2]) + (up[0] * fwd[0])));
                new_up_x = ((-((fwd[1] * up[1]) + ((up[2] * fwd[2]) + (up[0] * fwd[0]))) * fwd[0])) + up[0];
                new_up_y = ((up_dot_fwd * fwd[1]) + up[1]);
                up[0] = new_up_x;
                up[1] = new_up_y;
                up[2] = ((up_dot_fwd * fwd[2]) + up[2]);

                new_up_len = __fsqrts(((up[2] * up[2]) + ((new_up_x * new_up_x) + (new_up_y * new_up_y))));
                if (__fabs(new_up_len) >= 0.000099999997)
                {
                    float inverse_len = ((float)1.0 / new_up_len);
                    up[0] = inverse_len * new_up_x;
                    up[1] = inverse_len * new_up_y;
                    up[2] = inverse_len * up[2];
                }
            }
        }
    }

    /* --- object "at rest" / contact state flags --- */
    {
        float *velocity = od->translational_velocity.n;
        float *angular_velocity = od->angular_velocity.n;
        unsigned int flags;

        if (stopped_count != physics->mass_points.count
            || ground_contact_count < 3
            || secondary_contact_count
            || ((velocity[0] * velocity[0]) + ((velocity[1] * velocity[1]) + (velocity[2] * velocity[2]))) > 0.0011111111
            || ((angular_velocity[2] * angular_velocity[2]) + ((angular_velocity[0] * angular_velocity[0]) + (angular_velocity[1] * angular_velocity[1]))) > 0.0027415568
            || ((velocity_delta_x * velocity_delta_x) + ((velocity_delta_z * velocity_delta_z) + (velocity_delta_y * velocity_delta_y))) > 0.00000030864197
            || ((angular_velocity_delta_x * angular_velocity_delta_x) + ((angular_velocity_delta_z * angular_velocity_delta_z) + (angular_velocity_delta_y * angular_velocity_delta_y))) > 0.0000030461742)
        {
            flags = od->flags & ~(1u << _object_at_rest_bit);   /* clear "at rest" */
        }
        else
        {
            flags = od->flags | (1u << _object_at_rest_bit);          /* set "at rest" */
        }
        od->flags = flags;

        flags = od->flags | (1u << _object_on_ground_bit);
        if (ground_contact_count <= 0)
            flags = od->flags & ~(1u << _object_on_ground_bit);
        od->flags = flags;

        flags |= (1u << _object_on_media_bit);
        if (water_contact_count <= 0)
            flags = od->flags & ~(1u << _object_on_media_bit);
        od->flags = flags;

        flags |= (1u << _object_partially_under_media_bit);
        if (water_contact_count <= 0)
            flags = od->flags & ~(1u << _object_partially_under_media_bit);
        od->flags = flags;

        flags |= (1u << _object_wholly_under_media_bit);
        if (water_contact_count != physics->mass_points.count)
            flags = od->flags & ~(1u << _object_wholly_under_media_bit);
        od->flags = flags;
    }
}
