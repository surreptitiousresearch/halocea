/* ai_scripting_migrate_internal @0x83770500 — move every actor of a source encounter into a target
 * encounter, remapping each source squad to the best-matching target squad (ai_scripting_migrate_find_target_squad).
 *
 * Per-squad mapping is computed first; then the source encounter's actors are re-homed (with an optional
 * advance/retreat stimulus), pre-vehicle encounter references are migrated (and the "in vehicle" flag moved
 * to the target), and disconnected (encounterless) actors that reference the source are re-homed too,
 * re-attaching them when the target encounter is in the current BSP. Finally team status is refreshed if the
 * two encounters differ in team.
 *
 * DEVIATION: the decompiler aliases the squad-mapping scratch and three iterator types onto one stack
 * buffer; reconstructed here with a dedicated squad_mapping[] array and separate iterator locals. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_iterator.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/ai_index_squad_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/actor_palette_entry.h"
#include "headers/squad_definition.h"
#include "headers/blam_data_globals.h"


extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);
extern uint32_t tag_get_group_tag(int16_t tag_index);
extern int ai_scripting_migrate_find_target_squad(__int16 source_squad_index, actor_variant_definition *source_variant,
                                                  actor_definition *source_actor, int match_by_squad_index,
                                                  int target_encounter_index, int unused_target_ai_index,
                                                  const char *unused_debug_description);
extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void actor_stimulus_maneuvering(uint16_t actor_index, uint8_t advancing, uint8_t flee);
extern int global_structure_bsp_index_get(void);
extern void encounterless_detach_actor(int actor_index);
extern void encounter_attach_actor(int actor_index, int encounter_index, int16_t squad_index, uint8_t has_previous_team);
extern void ai_update_team_status(void);
extern void encounters_update_dirty_status(void);

void ai_scripting_migrate_internal(int source_index, int target_index, uint8_t generate_stimulus, uint8_t advancing_stimulus)
{
    if ( source_index == -1 || target_index == -1 )
        return;

    int source = (unsigned __int16)source_index;
    int target = (unsigned __int16)target_index;
    if ( source == 0xFFFF )
        return;

    scenario *scenario_globals = global_scenario;
    char *encounter_defs = (char *)global_scenario->ai_encounters.address;
    encounter_datum *source_encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, source);
    encounter_datum *target_encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, target);
    encounter_definition *source_def = &((encounter_definition *)encounter_defs)[source];
    encounter_definition *target_def = &((encounter_definition *)encounter_defs)[target];

    __int16 squad_mapping[64];
    for ( int i = 0; i < 64; ++i )
        squad_mapping[i] = -1;

    /* Map each source squad to the best-matching target squad. */
    ai_index_squad_iterator squad_iterator;
    ai_index_squad_iterator_new(source_index, &squad_iterator);
    for ( squad_datum *squad = ai_index_squad_iterator_next(&squad_iterator);
          squad;
          squad = ai_index_squad_iterator_next(&squad_iterator) )
    {
        if ( squad->current_count <= 0 && !source_encounter->is_prevehicle_encounter )
            continue;

        int squad_index = squad_iterator.squad_index;
        actor_definition *source_actor = nullptr;
        actor_variant_definition *source_variant = nullptr;  /* was int holding the pointer bits */

        __int16 palette_index =
            ((squad_definition *)source_def->squads.address)[squad_index].actor_palette_index;
        if ( palette_index >= 0 && palette_index < scenario_globals->ai_actor_palette.count )
        {
            actor_palette_entry *palette_entry =
                &((actor_palette_entry *)scenario_globals->ai_actor_palette.address)[palette_index];
            int actor_tag_index = palette_entry->reference.index;
            if ( actor_tag_index != -1 && tag_get_group_tag(actor_tag_index) == 0x61637472u /* 'actr' */ )
            {
                source_actor = TAG_GET(actor_definition, actor_tag_index);
                unsigned int variant_tag = source_actor->unused3[2];
                if ( variant_tag != -1 )
                    source_variant = TAG_GET(actor_variant_definition, variant_tag);
            }
        }

        squad_mapping[squad_index] = ai_scripting_migrate_find_target_squad(
            (__int16)squad_index, source_variant, source_actor,
            target == source, target_index, 0, nullptr);
    }

    /* Re-home the source encounter's actors. */
    encounter_actor_iterator actor_iterator_state;
    encounter_actor_iterator_new(&actor_iterator_state, source);
    for ( actor_datum *actor = encounter_actor_iterator_next(&actor_iterator_state);
          actor;
          actor = encounter_actor_iterator_next(&actor_iterator_state) )
    {
        int squad_index = actor->meta.squad_index;
        __int16 target_squad = squad_mapping[squad_index];
        if ( target_squad != -1 && (target != source || target_squad != squad_index) )
        {
            actor_change_encounter(actor_iterator_state.index, target, target_squad);
            if ( generate_stimulus )
                actor_stimulus_maneuvering(actor_iterator_state.index, advancing_stimulus, 0);
        }
    }

    /* Migrate pre-vehicle encounter references, and move the in-vehicle flag to the target. */
    if ( source_encounter->is_prevehicle_encounter )
    {
        actor_iterator all_actors;
        actor_iterator_new(&all_actors, 0);
        for ( actor_datum *actor = actor_iterator_next(&all_actors);
              actor;
              actor = actor_iterator_next(&all_actors) )
        {
            if ( (unsigned __int16)actor->meta.prevehicle_encounter_index == source )
            {
                __int16 prevehicle_squad_index = actor->meta.prevehicle_squad_index;
                __int16 target_squad = squad_mapping[prevehicle_squad_index];
                if ( target_squad != -1 && (target != source || target_squad != prevehicle_squad_index) )
                {
                    actor->meta.prevehicle_encounter_index = target;
                    actor->meta.prevehicle_squad_index = target_squad;
                }
            }
        }
        if ( target != source )
        {
            source_encounter->is_prevehicle_encounter = 0;
            target_encounter->is_prevehicle_encounter = 1;
        }
    }

    /* Migrate disconnected references (encounterless actors that came from the source). */
    encounter_actor_iterator disconnected_iterator;
    encounter_actor_iterator_new(&disconnected_iterator, -1);
    for ( actor_datum *actor = encounter_actor_iterator_next(&disconnected_iterator);
          actor;
          actor = encounter_actor_iterator_next(&disconnected_iterator) )
    {
        if ( (unsigned __int16)actor->meta.disconnected_encounter_index == source )
        {
            __int16 disconnected_squad_index = actor->meta.disconnected_squad_index;
            __int16 target_squad = squad_mapping[disconnected_squad_index];
            if ( target_squad != -1 && (target != source || target_squad != disconnected_squad_index) )
            {
                actor->meta.disconnected_encounter_index = target;
                actor->meta.disconnected_squad_index = target_squad;
                if ( target != source && target_def->runtime_structure_bsp_reference_index == global_structure_bsp_index_get() )
                {
                    encounterless_detach_actor(disconnected_iterator.index);
                    encounter_attach_actor(disconnected_iterator.index, actor->meta.disconnected_encounter_index,
                                           actor->meta.disconnected_squad_index, 1u);
                }
            }
        }
    }

    void *updated = nullptr;
    if ( source_encounter->team_index != target_encounter->team_index )
        ai_update_team_status(); /* attested void: r3-thread dropped */
    encounters_update_dirty_status();
}
