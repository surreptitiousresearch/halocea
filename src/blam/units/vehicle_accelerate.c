/* vehicle_accelerate @0x8375D840 — apply a damage impulse to a vehicle: add the acceleration to its linear
 * velocity (+104..+112) and a torque about the world up axis (acceleration x up, normalised and scaled by
 * pi) to its angular velocity (+140..+148), flag it as physics-awake (+1316) and clear the at-rest flag
 * (+16 bit 0x20). No-op if the vehicle definition has no physics ([35] == -1). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/_object_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

extern float normalize3d(real_vector3d *v);

void vehicle_accelerate(int vehicle_index, const real_vector3d *acceleration)
{
    vehicle_datum *vehicle = (vehicle_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    if ( TAG_GET(_object_definition, vehicle->definition_index)->physics.index == -1 )
        return;

    vehicle->object.translational_velocity.n[0] += acceleration->n[0];
    vehicle->object.translational_velocity.n[1] += acceleration->n[1];
    vehicle->object.translational_velocity.n[2] += acceleration->n[2];

    const real_vector3d *up = global_up3d;
    real_vector3d torque;  /* acceleration x up */
    torque.n[0] = (up->n[1] * acceleration->n[2]) - (up->n[2] * acceleration->n[1]);
    torque.n[1] = (up->n[2] * acceleration->n[0]) - (up->n[0] * acceleration->n[2]);
    torque.n[2] = (up->n[0] * acceleration->n[1]) - (up->n[1] * acceleration->n[0]);
    float magnitude = normalize3d(&torque);
    if ( magnitude > 0.0f )
    {
        vehicle->object.angular_velocity.n[0] += torque.n[0] * (magnitude * PI);
        vehicle->object.angular_velocity.n[1] += torque.n[1] * (magnitude * PI);
        vehicle->object.angular_velocity.n[2] += torque.n[2] * (magnitude * PI);
    }

    vehicle->vehicle.has_been_accelerated_since_last_incremental = 1;
    vehicle->object.flags &= ~(1u << _object_at_rest_bit);
}
