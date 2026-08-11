/* actor_action_handle_berserking_from_damage @0x837F2220 — transitions into berserking when a pending
 * damage-triggered flag (byte +748) is set and the actor's current health value (float +448) and shield
 * value (float +440) both fall within its actor_definition's berserk-from-damage thresholds (floats +920/
 * +924). Ensures the berserk timer (word +392, see actor_action_handle_berserking_from_attacking_mode.c)
 * is at least 3, then clears the pending flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/blam_data_globals.h"


uint8_t actor_action_handle_berserking_from_damage(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( actor->stimuli.was_damaged
        && actor->input.recent_body_damage > definition->berserk.damage_berserk_amount
        && actor->input.body_vitality < definition->berserk.damage_berserk_threshold )
    {
        int16_t berserk_timer = actor->stimuli.berserk_type;

        if ( berserk_timer <= 3 )
            berserk_timer = 3;

        actor->stimuli.berserk_type = berserk_timer;
        actor->stimuli.was_damaged = 0;
        return 1;
    }

    return 0;
}
