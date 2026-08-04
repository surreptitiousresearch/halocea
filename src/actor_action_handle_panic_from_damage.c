/* actor_action_handle_panic_from_damage @0x837F1DA8 — trigger a panic reaction when the actor was recently
 * damaged (actor->stimuli.was_damaged) beyond its definition's panic damage threshold (definition float[171] vs the
 * actor's recent-damage accumulator actor->input.recent_body_damage). Latches the damaging prop as the panic prop
 * (actor->stimuli.panic_prop_index) unless a panic is already in progress with a valid prop, raises
 * panic_type (actor->stimuli.panic_type) to at least _actor_panic_damage, clears the damage flag, and reports whether a panic was
 * started. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


extern int actor_perception_find_recent_damaging_prop_index(int actor_index, uint8_t skip_friendlies);

uint8_t actor_action_handle_panic_from_damage(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->stimuli.was_damaged )
        return 0;
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
    if ( actor->input.recent_body_damage <= definition->panic.panic_damage_threshold )  /* def+684 */
        return 0;

    if ( !actor->stimuli.panic_type || actor->stimuli.panic_prop_index == -1 )
        actor->stimuli.panic_prop_index = actor_perception_find_recent_damaging_prop_index(actor_index, 1u);

    /* floor is a panic_type priority, not a tick count */
    __int16 panic_type = actor->stimuli.panic_type;
    if ( panic_type <= _actor_panic_damage )
        panic_type = _actor_panic_damage;
    actor->stimuli.panic_type = panic_type;
    actor->stimuli.was_damaged = 0;
    return 1;
}
