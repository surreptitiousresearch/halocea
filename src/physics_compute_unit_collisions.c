/* physics_compute_unit_collisions @0x837C14A8 — resolve a vehicle against nearby units. Gathers objects
 * within the vehicle's bounding sphere (vehicles always; bipeds too when the vehicle has a collision
 * model), then for each: vehicles are pushed apart via physics_compute_vehicle_collision (gated so each
 * pair is only processed once, unless the other is flagged for collision or has a positive physics
 * radius); bipeds that are not flagged "no collision" are pushed by physics_compute_biped_collision.
 *
 * DEVIATION: objects_in_sphere takes a float `radius` arg which (PPC FPR-shadow ABI) reserves its
 * positional GPR slot, so the decompiler mis-bound the trailing two args. Restored from the disassembly:
 * object_indices is the on-stack results buffer and maximum_count is 2048 (the buffer is ~8KB, not the
 * 80 bytes the decompiler inferred). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_model_instance.h"
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern uint8_t physics_compute_vehicle_collision(const physics_instance *instance0, const physics_instance *instance1);
extern uint8_t physics_compute_biped_collision(collision_model_instance *instance, int biped_index);

void physics_compute_unit_collisions(int vehicle_index)
{
    collision_model_instance collision_instance;
    unsigned __int8 collision_model_valid = collision_model_instance_new(&collision_instance, vehicle_index);

    physics_instance vehicle_physics;
    if (!physics_instance_new(&vehicle_physics, vehicle_index))
        return;

    int found_objects[2048];
    object_datum *vehicle_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    unsigned int type_flags = (collision_model_valid != 0) + 2; /* bipeds + vehicles when a collision model exists */

    int found_count = objects_in_sphere(1u, type_flags, &vehicle_data->object.location,
                                        &vehicle_data->object.bounding_sphere_center,
                                        vehicle_data->object.bounding_sphere_radius,
                                        found_objects, 2048);
    if (found_count <= 0)
        return;

    for (int i = 0; i < found_count; i++)
    {
        int object_index = found_objects[i];
        object_header_datum *header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
        unsigned __int8 object_type = header->type;

        if (object_type)
        {
            if (object_type == object_type_vehicle && object_index != vehicle_index)
            {
                physics_instance other_physics;
                if (physics_instance_new(&other_physics, object_index)
                    && ((unsigned __int16)object_index < (unsigned __int16)vehicle_index
                        || (header->datum->object.flags & (1u << _object_at_rest_bit)) != 0
                        || other_physics.physics->radius > 0.0f))
                {
                    physics_compute_vehicle_collision(&vehicle_physics, &other_physics);
                }
            }
        }
        else
        {
            if ((header->datum->object.damage_flags & (1u << _object_dead_bit)) == 0)
                physics_compute_biped_collision(&collision_instance, object_index);
        }
    }
}
