/* physics_compute_vehicle_collision @0x837BF670 — resolve interpenetration between two physics instances
 * (vehicles) by treating every mass point as a sphere. For each pair of overlapping mass-point spheres it
 * computes a penetration-depth restoring impulse along the contact normal (scaled by gravity, the global
 * collision depth, and the geometric-mean mass), and accumulates an equal-and-opposite force plus the
 * corresponding torque (r x f about each object's center) onto both objects' force/torque accumulators
 * (object+1288 force, object+1300 torque). The contact flag (object+1316) is set and the "at rest" bit
 * (_object_at_rest_bit) cleared. Object0 always receives its impulse (subject to the unit-driver gate); object1
 * only if its physics radius is <= 0. Returns 1 if any pair collided.
 *
 * All float; the decompiler's double temporaries are single-precision intermediates. Mass-point records are
 * the 128-byte physics_definition.mass_points elements (local position@+56, radius@+104). Object position is
 * at object+92; raw object offsets are kept in the corpus style. */

#include <stdint.h>
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/vehicle_datum.h"
#include "headers/object_flags.h"
#include "headers/mass_point_definition.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/networked_datum_role.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t unit_driver_seat_is_filled(int unit_index);

uint8_t physics_compute_vehicle_collision(const physics_instance *instance0, const physics_instance *instance1)
{
    vehicle_datum *object0 = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, instance0->object_index)->datum);
    vehicle_datum *object1 = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, instance1->object_index)->datum);
    float mass_scale = __fsqrts(instance0->physics->mass * instance1->physics->mass);

    int collided = 0;
    real_vector3d force0 = {0.0f, 0.0f, 0.0f};
    real_vector3d torque0 = {0.0f, 0.0f, 0.0f};
    real_vector3d force1 = {0.0f, 0.0f, 0.0f};
    real_vector3d torque1 = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < instance0->physics->mass_points.count; i++)
    {
        mass_point_definition *mass_point0 = &((mass_point_definition *)instance0->physics->mass_points.address)[i];
        real_point3d world0;
        matrix4x3_transform_point(&instance0->world_matrix, &mass_point0->position, &world0);
        float radius0 = mass_point0->radius;

        for (int j = 0; j < instance1->physics->mass_points.count; j++)
        {
            mass_point_definition *mass_point1 = &((mass_point_definition *)instance1->physics->mass_points.address)[j];
            float radius_sum = mass_point1->radius + radius0;
            real_point3d world1;
            matrix4x3_transform_point(&instance1->world_matrix, &mass_point1->position,
                                      &world1);

            float dir_x = world1.n[0] - world0.n[0];
            float dir_y = world1.n[1] - world0.n[1];
            float dir_z = world1.n[2] - world0.n[2];
            float distance = __fsqrts(dir_x * dir_x + (dir_z * dir_z + dir_y * dir_y));
            if (__fabs(distance) < 0.000099999997f)
            {
                distance = 0.0f;
            }
            else
            {
                float inverse = 1.0f / distance;
                dir_x = inverse * dir_x;
                dir_y = dir_y * inverse;
                dir_z = dir_z * inverse;
            }

            if (distance < radius_sum && distance > 0.0f)
            {
                collided = 1;
                float half_penetration = (radius_sum - distance) * 0.5f;
                float contact_offset = radius0 - half_penetration;   /* mass_point0 center -> contact point */
                float contact_x = contact_offset * dir_x + world0.n[0];
                float contact_y = dir_y * contact_offset + world0.n[1];
                float contact_z = dir_z * contact_offset + world0.n[2];

                /* contact point relative to each object's center */
                float r0_x = contact_x - object0->object.position.n[0];
                float r0_y = contact_y - object0->object.position.n[1];
                float r0_z = contact_z - object0->object.position.n[2];
                float r1_x = contact_x - object1->object.position.n[0];
                float r1_y = contact_y - object1->object.position.n[1];
                float r1_z = contact_z - object1->object.position.n[2];

                float impulse = (global_gravity / global_physics_collision_depth) * half_penetration
                                * mass_scale * 2.0f;

                /* equal and opposite contact forces */
                float f0_x = dir_x * -impulse, f0_y = dir_y * -impulse, f0_z = dir_z * -impulse;
                float f1_x = dir_x * impulse, f1_y = dir_y * impulse, f1_z = dir_z * impulse;

                force0.n[0] += f0_x;
                force0.n[1] += f0_y;
                force0.n[2] += f0_z;
                force1.n[0] += f1_x;
                force1.n[1] += f1_y;
                force1.n[2] += f1_z;

                /* torque = r x f about each object center */
                torque0.n[0] += r0_y * f0_z - r0_z * f0_y;
                torque0.n[1] += r0_z * f0_x - r0_x * f0_z;
                torque0.n[2] += r0_x * f0_y - r0_y * f0_x;
                torque1.n[0] += r1_y * f1_z - r1_z * f1_y;
                torque1.n[1] += r1_z * f1_x - r1_x * f1_z;
                torque1.n[2] += r1_x * f1_y - r1_y * f1_x;
            }
        }
    }

    if (collided)
    {
        if (object0->object.datum_role != _networked_datum_puppet || unit_driver_seat_is_filled(instance0->object_index) == 1)
        {
            object0->vehicle.collision_force.n[0] += force0.n[0];
            object0->vehicle.collision_force.n[1] += force0.n[1];
            object0->vehicle.collision_force.n[2] += force0.n[2];
            object0->vehicle.collision_torque.n[0] += torque0.n[0];
            object0->vehicle.collision_torque.n[1] += torque0.n[1];
            object0->vehicle.collision_torque.n[2] += torque0.n[2];
            object0->vehicle.has_been_accelerated_since_last_incremental = 1;
            object0->object.flags &= ~(1u << _object_at_rest_bit);
        }
        if (instance1->physics->radius <= 0.0f
            && (object1->object.datum_role != _networked_datum_puppet || unit_driver_seat_is_filled(instance1->object_index) == 1))
        {
            object1->vehicle.collision_force.n[0] += force1.n[0];
            object1->vehicle.collision_force.n[1] += force1.n[1];
            object1->vehicle.collision_force.n[2] += force1.n[2];
            object1->vehicle.collision_torque.n[0] += torque1.n[0];
            object1->vehicle.collision_torque.n[1] += torque1.n[1];
            object1->vehicle.collision_torque.n[2] += torque1.n[2];
            object1->vehicle.has_been_accelerated_since_last_incremental = 1;
            object1->object.flags &= ~(1u << _object_at_rest_bit);
        }
    }

    return collided;
}
