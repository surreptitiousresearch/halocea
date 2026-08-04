/* actor_stimulus_vehicle_eviction @0x837D536C — flag an actor to be evicted from its vehicle:
 * sets the eviction-request byte (actor datum +749). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_vehicle_eviction(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->stimuli.vehicle_eviction = 1;
}
