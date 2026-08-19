/* actor_type_swarm_control @ 0x837F8F88 — dispatch to the actor type's swarm_control callback (indexed by
 * meta.type). Tail call passes actor_index (the decompiler dropped the argument). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_type_definition.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_type_swarm_control(int actor_index)
{
    int16_t type = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.type;
    actor_type_definitions[type]->swarm_control(actor_index);
}
