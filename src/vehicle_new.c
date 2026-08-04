/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* vehicle_new @0x8375CDD0 — one-time initialization of a newly-placed vehicle object's runtime state:
 * zeroes a large block of vehicle-specific fields (suspension/wheel/turret physics scratch, +1228..+1312),
 * sets/clears the object at-rest flag (bit _object_at_rest_bit) based on whether the vehicle definition has
 * physics, and if so also nudges the vehicle's z-position up
 * by half the definition's height parameter (+4). In network games (dedicated server or client), clears
 * a few network-replication scratch bytes (+1317..+1319, +9). Always seeds a position-history snapshot
 * (+1452 timestamp, +1460/+1464/+1468 from the current forward vector at +92/+96/+100) and clears
 * +1316. Always returns true.
 *
 * RAW (irreducible): the runtime vehicle_datum fields past the shared object_datum region (e.g. +1316)
 * are the _vehicle_datum extension whose full runtime layout is not modelled here, and the vehicle tag
 * definition's +4/+140 fields are read positionally — kept as raw offsets. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern int game_time_get(void);

uint8_t vehicle_new(int vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle->definition_index);

    vehicle->vehicle.speed = 0.0f;
    vehicle->vehicle.slide = 0.0f;
    vehicle->vehicle.flags = 0;
    vehicle->vehicle.turn = 0.0f;
    vehicle->vehicle.stop_time = 0;
    vehicle->vehicle.wheel = 0.0f;
    vehicle->vehicle.airborne_ticks = 0;
    vehicle->vehicle.left_tread = 0.0f;
    vehicle->vehicle.upending_type = 0;
    vehicle->vehicle.right_tread = 0.0f;
    vehicle->vehicle.upending_ticks = 0;
    vehicle->vehicle.thrust = 0.0f;
    vehicle->vehicle.on_ground_ticks = 0;
    vehicle->vehicle.hover = 0.0f;
    *(int *)&vehicle->vehicle.suspension[0] = 0;
    *(int *)&vehicle->vehicle.suspension[4] = 0;
    vehicle->vehicle.collision_force.n[0] = 0.0f;
    vehicle->vehicle.collision_force.n[1] = 0.0f;
    vehicle->vehicle.collision_force.n[2] = 0.0f;
    vehicle->vehicle.collision_torque.n[0] = 0.0f;
    vehicle->vehicle.collision_torque.n[1] = 0.0f;
    vehicle->vehicle.collision_torque.n[2] = 0.0f;
    vehicle->vehicle.stuck_mass_point_flags = 0;

    unsigned int flags = vehicle->object.flags;
    unsigned int new_flags = flags | (1u << _object_at_rest_bit);
    if ( definition->object.physics.index != -1 )
        new_flags = flags & ~(1u << _object_at_rest_bit);
    vehicle->object.flags = new_flags;

    if ( definition->object.physics.index != -1 )
        vehicle->object.position.n[2] += definition->object.bounding_radius * 0.5f;

    __int16 connection = game_connection();
    if ( connection == 1 || connection == 2 )
    {
        vehicle->vehicle.baseline_valid = 0;
        vehicle->vehicle.baseline_index = 0;
        vehicle->vehicle.message_index = 0;
        vehicle->object.was_network_at_rest = 0;
    }

    int time = game_time_get();
    float spawn_x = vehicle->object.position.n[0];
    float spawn_y = vehicle->object.position.n[1];
    float spawn_z = vehicle->object.position.n[2];

    vehicle->vehicle.last_controlled_time = time;
    vehicle->vehicle.has_been_accelerated_since_last_incremental = 0;
    vehicle->vehicle.spawn_position.n[0] = spawn_x;
    vehicle->vehicle.spawn_position.n[1] = spawn_y;
    vehicle->vehicle.spawn_position.n[2] = spawn_z;

    return 1;
}
