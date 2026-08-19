/* squad_get_actor_type @0x8370A838 — resolve a squad's actor palette entry (an actor variant tag)
 * through its actor_reference to the actor definition's actor type. Defaults to _actor_type_none
 * if any link is unresolved. Same palette chase as encounter_create.c. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/squad_definition.h"
#include "headers/actor_palette_entry.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_type.h"

int16_t squad_get_actor_type(squad_definition *squad_definition)
{
    int16_t actor_palette_index = squad_definition->actor_palette_index;

    if ( actor_palette_index >= 0 && actor_palette_index < global_scenario->ai_actor_palette.count )
    {
        int actor_variant_index = ((actor_palette_entry *)global_scenario->ai_actor_palette.address)[actor_palette_index].reference.index;

        if ( actor_variant_index != -1 )
        {
            /* DEVIATION FIX: restored missing derefs — DB reads (*(_DWORD **)slot)[4] then
             * (*(_WORD **)slot)[10]: actv.actor_reference.index (0x10), then actr.type (0x14) */
            int actor_tag_index = TAG_GET(actor_variant_definition, actor_variant_index)->actor_reference.index;

            if ( actor_tag_index != -1 )
                return TAG_GET(actor_definition, actor_tag_index)->type;
        }
    }
    return _actor_type_none;
}
