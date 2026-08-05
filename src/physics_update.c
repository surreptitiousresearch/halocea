/* physics_update @0x837C1638 — advance an object's rigid-body physics for one tick. The object's physics
 * definition selects between the legacy ("old") integrator and the current ("new") one: a non-positive
 * leading float in the physics definition selects the new path. The new path builds a physics_instance,
 * derives each powered mass point's rotation matrix from its quaternion, computes the net force/torque
 * (physics_compute_new), folds in the object's accumulated force/torque (and clears those accumulators),
 * adds any external magic force/torque, integrates (physics_update_new), then resolves unit collisions. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/physics_definition.h"
#include "headers/vehicle_datum.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/physics_instance.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"
extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern real_matrix4x3 *matrix4x3_rotation_from_quaternion(real_matrix4x3 *matrix, const real_quaternion *quaternion);
extern void matrix4x3_transpose(real_matrix4x3 *matrix);
extern void physics_compute_new(const physics_instance *instance, const powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, real_vector3d *total_force, real_vector3d *total_torque);
extern void physics_update_new(const physics_instance *instance, const powered_mass_point_datum *powered_mass_points, const mass_point_datum *mass_points, const real_vector3d *total_force, const real_vector3d *total_torque);
extern void physics_update_old(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern void physics_compute_unit_collisions(int vehicle_index);

void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points,
                    const real_vector3d *magic_force, const real_vector3d *magic_torque)
{
    vehicle_datum *object_data = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_def = TAG_GET(_object_definition, object_data->definition_index);
    physics_definition *physics_def = TAG_GET(physics_definition, object_def->physics.index); /* object_def+140 = physics tag */

    if (physics_def->radius > 0.0f)
    {
        physics_update_old(object_index, powered_mass_points, mass_points, magic_force, magic_torque);
        return;
    }

    physics_instance instance;
    physics_instance_new(&instance, object_index);

    int powered_mass_point_count = physics_def->powered_mass_points.count; /* +104 */
    if (powered_mass_points && powered_mass_point_count > 0)
    {
        for (int i = 0; i < powered_mass_point_count; i = (int16_t)(i + 1))
        {
            powered_mass_point_datum *powered = &powered_mass_points[i];
            matrix4x3_rotation_from_quaternion(&powered->rotation_matrix, &powered->rotation);
            matrix4x3_transpose(&powered->rotation_matrix);
        }
    }

    real_vector3d total_force;
    real_vector3d total_torque;
    physics_compute_new(&instance, powered_mass_points, mass_points, &total_force, &total_torque);

    /* fold in and clear the vehicle's accumulated collision force/torque (vehicle sub-struct +60/+72) */
    total_force.n[0] = total_force.n[0] + object_data->vehicle.collision_force.n[0];
    total_force.n[1] = object_data->vehicle.collision_force.n[1] + total_force.n[1];
    total_force.n[2] = object_data->vehicle.collision_force.n[2] + total_force.n[2];
    total_torque.n[0] = total_torque.n[0] + object_data->vehicle.collision_torque.n[0];
    total_torque.n[1] = object_data->vehicle.collision_torque.n[1] + total_torque.n[1];
    total_torque.n[2] = object_data->vehicle.collision_torque.n[2] + total_torque.n[2];
    object_data->vehicle.collision_force.n[0] = 0.0f;
    object_data->vehicle.collision_force.n[1] = 0.0f;
    object_data->vehicle.collision_force.n[2] = 0.0f;
    object_data->vehicle.collision_torque.n[0] = 0.0f;
    object_data->vehicle.collision_torque.n[1] = 0.0f;
    object_data->vehicle.collision_torque.n[2] = 0.0f;

    if (magic_force)
    {
        total_force.n[0] = magic_force->n[0] + total_force.n[0];
        total_force.n[1] = magic_force->n[1] + total_force.n[1];
        total_force.n[2] = magic_force->n[2] + total_force.n[2];
    }
    if (magic_torque)
    {
        total_torque.n[0] = magic_torque->n[0] + total_torque.n[0];
        total_torque.n[1] = magic_torque->n[1] + total_torque.n[1];
        total_torque.n[2] = magic_torque->n[2] + total_torque.n[2];
    }

    physics_update_new(&instance, powered_mass_points, mass_points, &total_force, &total_torque);
    physics_compute_unit_collisions(object_index);
}
