/* actor_type_initialize @0x837F8F00 — dispatch to the per-actor-type initialize handler (if any) from the global
 * actor-type definition table, keyed by the actor's type (meta.type). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_type_definition.h"
#include "headers/blam_data_globals.h"

extern actor_type_definition *actor_type_definitions[];

void actor_type_initialize(uint16_t actor_index)
{
    void (*initialize)(int) =
        actor_type_definitions[DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.type]->initialize;
    if ( initialize )
        initialize(actor_index);
}
