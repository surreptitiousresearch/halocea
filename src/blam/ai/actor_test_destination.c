/* actor_test_destination @0x837C70E8 — update and return an actor's "reached destination" flag (byte @
 * actor +1156). The destination is considered reached when there is no destination/path requirement
 * (mode word @ actor +1132 is 0 or 1), or when the actor is within its destination tolerance of the
 * target point (squared distance test). Actor records are stride 1828; current position floats are at
 * +300/+304/+308, target at +1160/+1164/+1168.
 *
 * DEVIATION: distance and tolerance are computed in floats but routed through doubles by the soft-float
 * ABI. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"

extern float actor_destination_tolerance(int actor_index);

uint8_t actor_test_destination(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    short destination_mode = actor->control.path.destination_orders.destination_type;

    int reached = destination_mode == _destination_none || destination_mode == _destination_halt;
    if ( !reached )
    {
        float tolerance = actor_destination_tolerance(actor_index);
        float dx = actor->control.path.destination.point.z - actor->input.position.body_position.z;
        float dy = actor->control.path.destination.point.x - actor->input.position.body_position.x;
        float dz = actor->control.path.destination.point.y - actor->input.position.body_position.y;
        reached = (dz * dz + (dy * dy + dx * dx)) < (tolerance * tolerance);
    }
    if ( reached )
        actor->control.path.at_destination = 1;
    return actor->control.path.at_destination;
}
