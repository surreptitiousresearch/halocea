/* actor_combat_get_firing_variant_definition @0x837B7B48 — return the actor-variant definition that governs
 * the actor's current firing behaviour. This is normally the weapon's AI firing-parameters variant; it falls
 * back to the actor's own variant tag (character variant @ actor word +23) when the actor has no weapon or the
 * weapon specifies no firing variant. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/blam_data_globals.h"


extern weapon_definition *actor_get_weapon_definition(int actor_index);

actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index)
{
    /* recovered: raw dword (457*idx + 23) -> actor->meta.variant_definition_index (byte +92) */
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    actor_variant_definition *actor_variant =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    weapon_definition *weapon = actor_get_weapon_definition(actor_index);
    if ( !weapon )
        return actor_variant;
    /* recovered: *(int *)((char *)weapon + 0x3C8) -> weapon.ai_firing_parameters.index */
    int firing_variant_index = weapon->weapon.ai_firing_parameters.index;
    if ( firing_variant_index == -1 )
        return actor_variant;
    return TAG_GET(actor_variant_definition, firing_variant_index);
}
