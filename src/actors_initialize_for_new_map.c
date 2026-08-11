/* actors_initialize_for_new_map @0x8371CD38 — revalidate the actor, swarm, and swarm-component datum arrays
 * for a new map. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void actors_initialize_for_new_map(void)
{
    data_make_valid(actor_data);
    data_make_valid(swarm_data);
    data_make_valid(swarm_component_data);
}
