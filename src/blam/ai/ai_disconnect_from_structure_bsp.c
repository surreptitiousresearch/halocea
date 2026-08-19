/* ai_disconnect_from_structure_bsp @0x836E9D98 — run when the active structure BSP is torn down (BSP switch).
 * For every active encounter it walks its actors and decides whether each should be "disconnected" from its
 * encounter (detached and re-parented to the encounterless list) because its target is no longer relevant:
 * either it has no real target and no visible/near enemy player prop, or its target prop is stale (uncertain
 * for a while and now far). Swarm actors additionally split off any swarm units that fell outside the
 * combined PVS, spawning replacement actors for them. Every encounter is then force-deactivated. Finally it
 * clears the structure/pathfinding indices on all encounterless actors so nothing dangles into the old BSP.
 *
 * The actor's target sub-block is now modeled as actor_target_data (target_prop_index @0x270,
 * target_type @0x268). DEVIATION: the decompiler overlays the
 * detached-unit scratch list on the encounter_actor_iterator stack array; reproduced as a plain local array
 * (swarm units are bounded by the 152-byte swarm component, ≤32 unit handles). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/scenario.h"
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/swarm_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/prop_iterator.h"
#include "headers/ai_globals.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_default_state.h"
#include "headers/actor_target_type.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern unsigned int *players_get_combined_pvs(void);
extern int object_get_ultimate_parent(int object_index);
extern void actor_swarm_detach_from_unit(int actor_index, int unit_index);
extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);
extern void object_delete(int object_index);
extern void actor_flush_position_indices(int actor_index);
extern void encounter_detach_actor(int actor_index, uint8_t died);
extern void encounterless_attach_actor(int actor_index);
extern void encounter_force_deactivate(int encounter_index);
extern void actor_flush_structure_indices(int actor_index);
extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);


void ai_disconnect_from_structure_bsp(void)
{
    scenario *scn = global_scenario;
    if ( scn->ai_encounters.count > 0 )
    {
        int encounter_index = 0;
        do
        {
            encounter_datum *enc = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
            if ( enc->active && enc->current_count > 0 )
            {
                encounter_actor_iterator it;
                encounter_actor_iterator_new(&it, encounter_index);
                for ( actor_datum *actor = encounter_actor_iterator_next(&it); actor;
                      actor = encounter_actor_iterator_next(&it) )
                {
                    uint8_t disconnect = 0;
                    int target_prop_index = actor->target.target_prop_index;
                    int16_t target_type = actor->target.target_type;

                    if ( target_prop_index == -1 || target_type < actor_target_uninspected_orphan )
                    {
                        if ( !game_team_is_enemy(actor->meta.team_index, _game_team_player) )
                        {
                            prop_iterator pit;
                            prop_iterator_new(&pit, it.index);
                            for ( prop_datum *prop = prop_iterator_next(&pit); prop; prop = prop_iterator_next(&pit) )
                            {
                                if ( prop->player && (prop->visibility >= 2 || prop->distance < 3.0) )
                                    disconnect = 1;
                            }
                        }
                    }
                    else
                    {
                        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, target_prop_index);
                        int prop_state = prop->state;   /* recovered: *((__int16 *)prop + 18) */
                        if ( prop_state >= _prop_state_uninspected_orphan && prop_state <= _prop_state_inspected_orphan )   /* follow the target-of-target chain */
                            prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop->___u3.orphan_prop_index);
                        if ( prop->player )   /* recovered: prop[302] */
                        {
                            int uncertain = actor->state.uncertain_combat_timer;
                            if ( uncertain != -1 && uncertain < 90 && prop->distance < 10.0 )   /* recovered: *((float *)prop + 71) */
                                disconnect = 1;
                        }
                    }

                    if ( disconnect && actor->meta.swarm )
                    {
                        if ( actor->meta.swarm_cache_index == -1 )
                        {
                            disconnect = 0;
                        }
                        else
                        {
                            swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
                            const unsigned int *combined_pvs = players_get_combined_pvs();
                            int16_t detached_count = 0;
                            int detached_units[32];
                            int unit_count = swarm->unit_count;
                            for ( int k = 0; k < unit_count; k = (int16_t)(k + 1) )
                            {
                                int unit_handle = swarm->unit_indices[k];
                                int ultimate_parent = object_get_ultimate_parent(unit_handle);
                                int cluster = (uint16_t)((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum)->object.location.cluster_index;
                                if ( cluster == 0xFFFF
                                  || !BIT_VECTOR_TEST_FLAG(combined_pvs, (int16_t)cluster) )
                                {
                                    detached_units[detached_count++] = unit_handle;
                                }
                                unit_count = swarm->unit_count;
                            }
                            if ( detached_count )
                            {
                                if ( detached_count == unit_count )
                                {
                                    disconnect = 0;
                                }
                                else
                                {
                                    for ( int m = 0; m < detached_count; m = (int16_t)(m + 1) )
                                    {
                                        int unit_handle = detached_units[m];
                                        actor_swarm_detach_from_unit(it.index, unit_handle);
                                        /* DEVIATION: all four stack-passed args ARE written @0x836EA0A4-0x836EA0B0
                                         * (sth r22=2 @+0x56, sth r25=0 @+0x5E, sth r21=-1 @+0x66, stb r25=0 @+0x6F),
                                         * matching the arg_56/5E/66/6F reads in the callee prologue @0x8372151C-48;
                                         * the decompiler dropped the four stores and read them uninitialized. */
                                        if ( actor_create_for_unit(1u, unit_handle,
                                                actor->meta.variant_definition_index, actor->meta.encounter_index,
                                                actor->meta.squad_index, 0, it.index, 0,
                                                actor_default_state_alert, actor_default_state_none, -1,
                                                0) == -1 )
                                            object_delete(unit_handle);
                                    }
                                }
                            }
                        }
                    }

                    if ( disconnect )
                    {
                        int16_t squad_index = actor->meta.squad_index;
                        actor->meta.disconnected_encounter_index = encounter_index;
                        actor->meta.disconnected_squad_index = squad_index;
                        actor_flush_position_indices(it.index);
                        encounter_detach_actor(it.index, 0);
                        encounterless_attach_actor(it.index);
                    }
                }
            }
            encounter_force_deactivate(encounter_index);
            encounter_index = (int16_t)(encounter_index + 1);
        }
        while ( encounter_index < scn->ai_encounters.count );
    }

    int actor_index = ai_globals->first_encounterless_actor_index;
    if ( actor_index != -1 )
    {
        int next;
        do
        {
            next = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.next_actor_index;
            actor_flush_structure_indices(actor_index);
            prop_iterator pit;
            prop_iterator_new(&pit, actor_index);
            for ( prop_datum *prop = prop_iterator_next(&pit); prop; prop = prop_iterator_next(&pit) )
            {
                prop->body_location.cluster_index = -1;
                prop->body_location.leaf_index = -1;
                prop->pathfinding_surface_index = -1;
            }
            actor_index = next;
        }
        while ( next != -1 );
    }
}
