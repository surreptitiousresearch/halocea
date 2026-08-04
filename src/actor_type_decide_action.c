/* actor_type_decide_action @ 0x837F8F48 — dispatch to the actor type's decide_action callback. The actor
 * type index is meta.type (*(short*)(actor_data->data + 1828*idx + 4)). Tail call passes actor_index (the
 * decompiler dropped the argument). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_type_definition.h"
#include "headers/blam_data_globals.h"


void actor_type_decide_action(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    __int16 type = actor->meta.type;
    actor_type_definitions[type]->decide_action(actor_index);
}
