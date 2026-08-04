/* actors_dispose_from_old_map @0x8371CD8C — per-map teardown: invalidate the actor, swarm, and
 * swarm-component data pools. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void actors_dispose_from_old_map(void)
{
    data_make_invalid(actor_data);
    data_make_invalid(swarm_data);
    data_make_invalid(swarm_component_data);
}
