/* ai_scripting_migrate_find_target_squad @0x837702A8 — pick which squad of the target encounter a migrating
 * actor should join, preferring (in order): the same squad index (only when migrating within one
 * encounter), the same actor definition, the same actor variant, a variant sharing the same base unit, and
 * finally the first squad. Returns the chosen target squad index, or 0/-1 if the encounter has no squads.
 *
 * NOTE: the decompiler's auto-named parameters are misleading; renamed here to match actual usage
 * (param1 is a squad index, param2/param3 are the source actor's variant/definition pointers, param4 is the
 * within-same-encounter flag, param5 is the target encounter index; the last two are unused). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/ai_index_squad_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_reference.h"
#include "headers/blam_data_globals.h"

extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern void *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);
extern uint32_t tag_get_group_tag(int16_t tag_index);

int ai_scripting_migrate_find_target_squad(
        int16_t source_squad_index,
        actor_variant_definition *source_variant,
        actor_definition *source_actor,
        int match_by_squad_index,
        int target_encounter_index,
        int unused_target_ai_index,
        const char *unused_debug_description)
{
    (void)unused_target_ai_index;
    (void)unused_debug_description;

    scenario *scenario_globals = global_scenario;
    char match_by_index = (char)match_by_squad_index;
    encounter_definition *target_encounter = &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)target_encounter_index];

    int match_same_index = -1;
    int match_same_actor = -1;
    int match_same_variant = -1;
    int match_same_unit = -1;
    int first_squad = -1;

    ai_index_squad_iterator iterator;
    ai_index_squad_iterator_new(target_encounter_index, &iterator);
    while ( ai_index_squad_iterator_next(&iterator) )
    {
        int squad_index = iterator.squad_index;
        actor_definition *candidate_actor = nullptr;
        actor_variant_definition *candidate_variant = nullptr;

        /* squad def (232 bytes) +0x20 = actor palette index; target encounter +0x84 = squads address */
        int16_t palette_index = ((squad_definition *)target_encounter->squads.address)[iterator.squad_index].actor_palette_index;
        if ( palette_index >= 0 && palette_index < scenario_globals->ai_actor_palette.count )
        {
            tag_reference *palette_entry = &((tag_reference *)scenario_globals->ai_actor_palette.address)[palette_index];
            int actor_tag_index = palette_entry->index;
            if ( actor_tag_index != -1 && tag_get_group_tag(actor_tag_index) == 0x61637472u /* 'actr' */ )
            {
                candidate_actor = TAG_GET(actor_definition, actor_tag_index);
                unsigned int variant_tag = candidate_actor->unused3[2];
                if ( variant_tag != -1 )
                    candidate_variant = TAG_GET(actor_variant_definition, variant_tag);
            }
        }

        if ( (int16_t)match_same_index == -1 && match_by_index && source_squad_index == squad_index )
            match_same_index = (int16_t)squad_index;
        if ( (int16_t)match_same_actor == -1 && source_actor && candidate_actor && source_actor == candidate_actor )
            match_same_actor = (int16_t)squad_index;
        if ( (int16_t)match_same_variant == -1 && source_variant && candidate_variant
          && source_variant == candidate_variant )
            match_same_variant = (int16_t)squad_index;
        /* recovered: *(u16 *)(variant + 20) -> low half of unit_reference.group_tag (lhz 0x14, faithful) */
        if ( (int16_t)match_same_unit == -1 && source_variant && candidate_variant
          && (uint16_t)source_variant->unit_reference.group_tag == (uint16_t)candidate_variant->unit_reference.group_tag )
            match_same_unit = (int16_t)squad_index;
        if ( (int16_t)first_squad == -1 )
            first_squad = (int16_t)squad_index;
    }

    if ( (int16_t)match_same_index != -1 )
        return match_same_index;
    if ( (int16_t)match_same_actor != -1 )
        return match_same_actor;
    if ( (int16_t)match_same_variant != -1 )
        return match_same_variant;
    if ( (int16_t)match_same_unit != -1 )
        return match_same_unit;
    if ( (int16_t)first_squad == -1 )
    {
        /* no squads iterated: 0 when the encounter declares squads, -1 when it has none */
        int squad_count = target_encounter->squads.count;
        return ~(squad_count >> 31) + (squad_count != 0);
    }
    return first_squad;
}
