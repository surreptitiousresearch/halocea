/* actor_perception_assess_vehicle_danger @0x837D6140 — record an incoming "dangerous moving vehicle" threat.
 * Ignored for actors riding in a vehicle, for vehicles whose tag lacks the danger flag (0x80) or that are
 * nearly stationary, or when the vehicle is beyond its danger radius + 10. Otherwise, if the actor has no
 * stronger threat already, latches the vehicle (threat type 3) with its position, velocity, driver and a
 * 20-tick fuse; the enemy flag is set unless the driver is on the actor's team. Returns 1 on latch.
 *
 * The sense position is provided by the caller, or computed here when null. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include <string.h>
#include "headers/actor_danger_zone_type.h"
#include "headers/vehicle_definition_flags.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void actor_perception_find_sense_position(int actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);

int actor_perception_assess_vehicle_danger(int actor_index, int vehicle_index,
                                           actor_position_data *sense_position, uint8_t currently_perceived)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->input.vehicle_index != -1 )
        return 0;

    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle->definition_index);

    if ( (definition->vehicle.flags & (1u << _vehicle_causes_collision_damage_bit)) == 0
      || (vehicle->object.translational_velocity.n[2] * vehicle->object.translational_velocity.n[2])
           + ((vehicle->object.translational_velocity.n[0] * vehicle->object.translational_velocity.n[0])
              + (vehicle->object.translational_velocity.n[1] * vehicle->object.translational_velocity.n[1])) <= 0.0011111111f )
    {
        return 0;
    }

    real_point3d vehicle_origin;
    object_get_origin(vehicle_index, &vehicle_origin);
    actor_position_data computed_sense[2];
    if ( !sense_position )
    {
        actor_perception_find_sense_position(actor_index, &vehicle_origin, -1, computed_sense);
        sense_position = computed_sense;
    }

    float dy = vehicle_origin.n[1] - sense_position->body_position.n[1];
    float dx = vehicle_origin.n[0] - sense_position->body_position.n[0];
    float dz = vehicle_origin.n[2] - sense_position->body_position.n[2];
    float distance = __fsqrts((dy * dy) + ((dx * dx) + (dz * dz)));
    if ( distance >= (definition->object.bounding_radius + 10.0f) )
        return 0;

    int threat_count = actor->danger_zone.danger_type;
    if ( threat_count >= actor_danger_zone_vehicle
      && (threat_count != actor_danger_zone_vehicle || actor->danger_zone.object_index == vehicle_index
          || distance >= actor->danger_zone.current_distance_from_actor) )
        return 0;

    int driver = vehicle->unit.driver_object_index;
    memset(&actor->danger_zone, 0, 0x6Cu);
    actor->danger_zone.object_index = vehicle_index;
    actor->danger_zone.danger_type = actor_danger_zone_vehicle;
    actor->danger_zone.owner_unit_index = driver;
    actor->danger_zone.danger_radius = definition->object.bounding_radius;
    actor->danger_zone.initial_position.n[0] = vehicle_origin.n[0];
    actor->danger_zone.initial_position.n[1] = vehicle_origin.n[1];
    actor->danger_zone.initial_position.n[2] = vehicle_origin.n[2];
    actor->danger_zone.initial_velocity.n[0] = vehicle->object.translational_velocity.n[0];
    actor->danger_zone.initial_velocity.n[1] = vehicle->object.translational_velocity.n[1];
    actor->danger_zone.initial_velocity.n[2] = vehicle->object.translational_velocity.n[2];
    actor->danger_zone.acknowledgement_timer = 20;
    actor->danger_zone.currently_perceived = currently_perceived;
    actor->danger_zone.hostility = 0;
    if ( driver != -1
      && !game_team_is_enemy(actor->meta.team_index,
                             (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle->unit.driver_object_index)->datum))->object.owner_team_index) )
    {
        actor->danger_zone.hostility = 1;
    }
    return 1;
}
