/* actor_action_handle_panic_from_surprise @0x837F1D1C — transitions into panic when a pending "surprised"
 * flag (actor byte +752) is set and the actor_definition allows it (flags bit 0x400). Copies a fallback
 * panic-source index (actor dword +756) into the current panic-target slot (actor dword +780) if the
 * latter hasn't been set yet, raises the panic_type to at least _actor_panic_surprise, then clears the pending
 * flag. Mirrors actor_action_handle_berserking_from_attacking_mode.c's raw-offset style. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


uint8_t actor_action_handle_panic_from_surprise(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( !actor->stimuli.was_surprised || (actor_def->flags & (1u << _actor_definition_panic_on_surprise_bit)) == 0 )
        return 0;

    if ( !actor->stimuli.panic_type || actor->stimuli.panic_prop_index == -1 )
        actor->stimuli.panic_prop_index = actor->stimuli.surprise_prop_index;

    /* floor is a panic_type priority, not a tick count */
    int16_t panic_type = actor->stimuli.panic_type;
    if ( panic_type <= _actor_panic_surprise )
        panic_type = _actor_panic_surprise;
    actor->stimuli.panic_type = panic_type;

    actor->stimuli.was_surprised = 0;
    return 1;
}
