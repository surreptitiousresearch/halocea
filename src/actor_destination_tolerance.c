/* actor_destination_tolerance @0x8371EDB8 — how close (world units) an actor must get to its destination
 * to consider it reached. Defaults to 0.5; if the actor is riding a vehicle, uses that vehicle
 * definition's ai_destination_radius. actor_action_obey (11) with the override flag
 * (action_data.obey.complex_control.destination_radius_valid) set uses the actor's own stored tolerance
 * (obey.complex_control.destination_radius). actor_action_vehicle (9) forces 0.7; otherwise floored at 0.2. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


float actor_destination_tolerance(int actor_index)
{
    float tolerance = 0.5f;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int object_index = actor->input.vehicle_index;
    if ( object_index != -1 )
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, object->definition_index);
        tolerance = vehicle_def->vehicle.ai_destination_radius;
    }

    int state = actor->state.action;
    if ( state == actor_action_obey && actor->state.action_data.___u0.obey.complex_control.destination_radius_valid )
        tolerance = actor->state.action_data.___u0.obey.complex_control.destination_radius;

    float result;
    if ( state == actor_action_vehicle )
    {
        result = 0.69999999f;
    }
    else
    {
        result = 0.2f;
        if ( tolerance > 0.2f )
            result = tolerance;
    }
    return result;
}
