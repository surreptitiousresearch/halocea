/* ai_scripting_migrate_find_target_squad @0x837702A8 — pick which squad of the target encounter a migrating
 * actor should join, preferring (in order): the same squad index (only when migrating within one
 * encounter), the same actor definition, the same actor variant, a variant sharing the same base unit, and
 * finally the first squad. Returns the chosen target squad index, or 0/-1 if the encounter has no squads.
 *
 * NOTE: the decompiler's auto-named parameters are misleading; renamed here to match actual usage
 * (param1 is a squad index, param2/param3 are the source actor's definition/variant pointers, param4 is the
 * within-same-encounter flag, param5 is the target encounter index; the last two are unused).
 *
 * DEVIATION (G13 2026-08-17): the palette group-tag check is 'actv' (0x61637476 @0x83770318), not 'actr';
 * the palette entry resolves to the candidate VARIANT (r30 @0x83770384), whose actor_reference.index
 * (+0x10 @0x83770388) yields the candidate ACTOR (r29). r4 (r25) is compared against the actor
 * (0x8377040C) and r5 (r20) against the variant (0x837703E4), so arg2=source_actor / arg3=source_variant
 * (previous source had them swapped, mirrored the derivation through 'actr', and mislabeled the final
 * lhz 0x14 tiebreak, which compares actor_definition->type, not a variant field). Match priority per the
 * epilogue: same squad index, same variant, same actor, same actor type, first squad.
 *
 * Deviation (attestation 2026-08-05): the DB prototype names r3 "source_encounter_index" and r7
 * "same_encounter"; the disassembly refutes it — r7 is the ai/encounter index (mr r3,r7 into
 * ai_index_squad_iterator_new @837702D4, clrlwi 16 then *0xB0 encounter_definition index @837702B8) and
 * r3 is the 16-bit source squad index (extsh r11,r17 @837703B8). r6 is an 8-bit flag (clrlwi r11,r16,24
 * @837703AC). Return is 16-bit: every result register is extsh-normalized and both callers narrow
 * (extsh r11,r3 @83770984 / sthx r3 @83770680). */

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

#include "headers/squad_datum.h"
extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);
extern uint32_t tag_get_group_tag(int16_t tag_index);

int16_t ai_scripting_migrate_find_target_squad(
        int16_t source_squad_index,
        actor_definition *source_actor,
        actor_variant_definition *source_variant,
        uint8_t match_by_squad_index,
        int target_encounter_index,
        int unused_target_ai_index,
        const char *unused_debug_description)
{
    (void)unused_target_ai_index;
    (void)unused_debug_description;

    scenario *scenario_globals = global_scenario;
    encounter_definition *target_encounter = &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)target_encounter_index];

    int16_t match_same_index = -1;
    int16_t match_same_actor = -1;
    int16_t match_same_variant = -1;
    int16_t match_same_type = -1;
    int16_t first_squad = -1;

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
            int variant_tag_index = palette_entry->index;
            if ( variant_tag_index != -1 && tag_get_group_tag(variant_tag_index) == 0x61637476u /* 'actv' */ )
            {
                candidate_variant = TAG_GET(actor_variant_definition, variant_tag_index);
                int actor_tag_index = candidate_variant->actor_reference.index;
                if ( actor_tag_index != -1 )
                    candidate_actor = TAG_GET(actor_definition, actor_tag_index);
            }
        }

        if ( match_same_index == -1 && match_by_squad_index && source_squad_index == squad_index )
            match_same_index = (int16_t)squad_index;
        if ( match_same_variant == -1 && source_variant && candidate_variant
          && source_variant == candidate_variant )
            match_same_variant = (int16_t)squad_index;
        if ( match_same_actor == -1 && source_actor && candidate_actor && source_actor == candidate_actor )
            match_same_actor = (int16_t)squad_index;
        /* lhz 0x14 on both ACTOR definitions -> actor_definition->type (lhz zero-extends; faithful) */
        if ( match_same_type == -1 && source_actor && candidate_actor
          && (uint16_t)source_actor->type == (uint16_t)candidate_actor->type )
            match_same_type = (int16_t)squad_index;
        if ( first_squad == -1 )
            first_squad = (int16_t)squad_index;
    }

    if ( match_same_index != -1 )
        return match_same_index;
    if ( match_same_variant != -1 )
        return match_same_variant;
    if ( match_same_actor != -1 )
        return match_same_actor;
    if ( match_same_type != -1 )
        return match_same_type;
    if ( first_squad == -1 )
    {
        /* no squads iterated: 0 when the encounter declares squads, -1 when it has none */
        int squad_count = target_encounter->squads.count;
        return (int16_t)(~(squad_count >> 31) + (squad_count != 0));
    }
    return first_squad;
}
