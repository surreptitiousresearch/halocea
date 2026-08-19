/* action_vehicle_flush_structure_indices @0x83821468 — invalidates the actor's cached "vehicle" action
 * destination surface index (offset 0xE4 == actor->state.action_data reinterpreted as vehicle_state_data,
 * destination_surface_index field — same 76-byte arm action_vehicle_setup_specific.c already established). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/vehicle_state_data.h"
#include "headers/blam_data_globals.h"


void action_vehicle_flush_structure_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.vehicle.destination_surface_index = -1;
}
