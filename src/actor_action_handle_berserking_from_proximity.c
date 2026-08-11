/* actor_action_handle_berserking_from_proximity @0x837F2190 — transitions into berserking when a hostile
 * prop is within the actor_definition's berserk-from-proximity distance threshold (float +928) and the
 * actor's awareness is high (combat_status word +0x6E >= 5, see
 * actor_action_handle_berserking_from_attacking_mode.c). Ensures the berserk timer (word +392) is at least 2.
 *
 * DEVIATION: the decompiler folded the tag-instance lookup and the prop_data lookup into one garbled nested
 * expression; disasm_range(0x837F219C,0x837F2220) confirms they are two independent lookups keyed by two
 * separate actor fields (actor+0x58 = definition tag index, actor+0x270 = "closest hostile prop" index). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_definition.h"
#include "headers/blam_data_globals.h"


uint8_t actor_action_handle_berserking_from_proximity(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.combat_status < _actor_combat_status_clear_los )
        return 0;

    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);

    if ( prop->distance >= definition->berserk.proximity_berserk_distance )
        return 0;

    int16_t berserk_timer = actor->stimuli.berserk_type;
    if ( berserk_timer <= 2 )
        berserk_timer = 2;
    actor->stimuli.berserk_type = berserk_timer;

    return 1;
}
