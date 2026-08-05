/* ai_scripting_migrate_by_unit_internal @0x837708C0 — migrate the single actor controlling a unit into the
 * target encounter, choosing the best-matching target squad. Used by ai_scripting_migrate_by_unit for each
 * unit (and its child objects).
 *
 * DEVIATION: the binary passes the actor's definition-tag and variant-tag pointers to
 * ai_scripting_migrate_find_target_squad in the opposite order to that helper's nominal (variant, actor)
 * parameters, and uses the actor field at +0x3A as the squad-match key (verified against the disassembly);
 * reproduced verbatim. The callee's two unused trailing params receive incidental register residue in the
 * binary (r8 = the (uint16)(encounter_index ^ ai_index) scratch from the same_encounter test, r9 = the
 * scaled tag-table offset); encounter_index/nullptr below are readable stand-ins, not attested values. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern int16_t ai_scripting_migrate_find_target_squad(int16_t source_squad_index, actor_variant_definition *source_variant, actor_definition *source_actor, uint8_t match_by_squad_index, int target_encounter_index, int unused_target_ai_index, const char *unused_debug_description);
extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void actor_stimulus_maneuvering(uint16_t actor_index, uint8_t advancing, uint8_t flee);

void ai_scripting_migrate_by_unit_internal(int unit_index, int ai_index, uint8_t generate_stimulus, uint8_t advancing_stimulus)
{
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int actor_index = unit_object->unit.actor_index;
    if ( actor_index == -1 )
        actor_index = unit_object->unit.swarm_actor_index;

    if ( actor_index == -1 || ai_index == -1 )
        return;

    int target = (uint16_t)ai_index;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int same_encounter = (uint16_t)(actor->meta.encounter_index ^ ai_index) == 0;   /* meta.encounter_index == ai_index */

    int16_t target_squad = ai_scripting_migrate_find_target_squad(
        actor->meta.squad_index,                                                         /* actor +0x3A */
        TAG_GET(actor_variant_definition, actor->meta.definition_index),        /* +0x58 definition_index */
        TAG_GET(actor_definition, actor->meta.variant_definition_index),        /* +0x5C variant_definition_index */
        same_encounter, ai_index, actor->meta.encounter_index, nullptr);

    if ( target_squad != -1 && (!same_encounter || target_squad != actor->meta.squad_index) )
    {
        actor_change_encounter(actor_index, target, target_squad);
        if ( generate_stimulus )
            actor_stimulus_maneuvering(actor_index, advancing_stimulus, 0);
    }
}
