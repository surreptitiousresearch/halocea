/* actor_action_handle_berserking_from_attacking_mode @0x837F2040 — transition into berserking if the
 * actor's definition allows it (actor_definition flags bit 0x80000, via actor->meta.definition_index),
 * the actor hasn't already given up (actor->external_orders.defending), and its awareness is high
 * (actor->state.combat_status >= _actor_combat_status_clear_los). Ensures actor->stimuli.berserk_type is at least 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"


uint8_t actor_action_handle_berserking_from_attacking_mode(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( (actor_def->flags & (1u << _actor_definition_always_berserk_in_attacking_mode_bit)) != 0
        && !actor->external_orders.defending
        && actor->state.combat_status >= _actor_combat_status_clear_los )
    {
        int16_t berserk_timer = actor->stimuli.berserk_type;

        if ( berserk_timer <= 1 )
            berserk_timer = 1;

        actor->stimuli.berserk_type = berserk_timer;
        return 1;
    }

    return 0;
}
