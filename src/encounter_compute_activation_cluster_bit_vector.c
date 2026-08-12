/* encounter_compute_activation_cluster_bit_vector @0x837091A8 — build the set of structure-BSP clusters an
 * encounter occupies, as a bit vector (one bit per cluster). Walks every actor in the encounter and ORs in the
 * cluster of each constituent unit (the ultimate parent object's cluster). When the encounter is active it also
 * folds in the clusters reached by the actors' firing positions: per-actor combat state selects firing-position
 * groups (accumulated in firing_position_group_mask) and squads (active_squad_mask), and the matching scenario
 * firing positions / squad firing points contribute their clusters too. Each actor's prop body-location cluster
 * is added as well.
 *
 * When update_actor_dormancy is set, each actor's dormant-desire flag is updated from whether any of its units
 * fell inside active_area (unless its squad disables dormancy).
 *
 * Style follows the sibling encounters_test_activation: actor/object/encounter datums are accessed as char* / int*
 * with the object-datum index idiom (object data ptr = ((int*)object_header_data->data)[3*(u16)index + 2]). */

#include <stdint.h>
#include <string.h>
#include "headers/bit_vector.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/move_position_definition.h"
#include "headers/unit_datum.h"
#include "headers/prop_datum.h"
#include "headers/structure_bsp.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/squad_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/actor_mode.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/firing_position_group_selection.h"
#include "headers/blam_data_globals.h"

/* actor_data / prop_data come from blam_data_globals.h (data_array *) */
extern int object_get_ultimate_parent(int object_index);
extern unsigned int actor_get_firing_position_group(int actor_index, int16_t evaluation_mode, int16_t group_selection_mode);

void encounter_compute_activation_cluster_bit_vector(int encounter_index, uint8_t update_actor_dormancy,
                                                     int bit_vector_size, const unsigned int *active_area,
                                                     unsigned int *bit_vector)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + encounter_index;
    int active_squad_mask = 0;
    int firing_position_group_mask = 0;

    memset(bit_vector, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(global_structure_bsp->clusters.count));

    int actor_index = encounter->first_actor_index;
    if ( actor_index == -1 )
        return;

    int update_dormancy = update_actor_dormancy;
    do
    {
        char actor_in_active_area = 1;
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

        if ( actor->meta.swarm )  /* actor.meta.swarm — iterate the unit object chain */
        {
            for ( int object_index = actor->meta.swarm_unit_index;  /* meta.swarm_unit_index (0x24) = first unit */
                  object_index != -1; )
            {
                unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
                int parent = object_get_ultimate_parent(object_index);
                int cluster = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum))->object.location.cluster_index;
                if ( cluster != -1 )
                {
                    int word_index = cluster >> 5;
                    int bit = 1 << (cluster & 0x1F);
                    bit_vector[word_index] |= bit;
                    if ( active_area )
                        actor_in_active_area = (active_area[word_index] & bit) != 0 ? 0 : actor_in_active_area;
                }
                object_index = object_data->unit.swarm_next_unit_index;
            }
        }
        else
        {
            int parent = object_get_ultimate_parent(actor->meta.unit_index);  /* meta.unit_index (0x18) */
            int16_t cluster = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum))->object.location.cluster_index;
            if ( cluster != -1 )
            {
                int word_index = cluster >> 5;
                int bit = 1 << (cluster & 0x1F);
                bit_vector[word_index] |= bit;
                if ( active_area )
                    actor_in_active_area = (active_area[word_index] & bit) == 0;
            }

            if ( encounter->active )  /* fold in firing positions / props */
            {
                int16_t actor_mode = actor->state.mode;    /* state.mode (0x6A) */
                if ( actor_mode == _actor_mode_combat )
                {
                    if ( actor->state.combat_status >= _actor_combat_status_investigate )          /* state.combat_status (0x6E) */
                        firing_position_group_mask |= actor_get_firing_position_group(actor_index, _firing_point_evaluation_mode_pursue, _firing_position_group_when_searching);
                    int16_t actor_action = actor->state.action;  /* state.action (0x6C) */
                    if ( actor_action == actor_action_guard || actor_action == actor_action_flee )
                        firing_position_group_mask |= actor_get_firing_position_group(actor_index, _firing_point_evaluation_mode_guard, _firing_position_group_normal);
                    else if ( actor_action == actor_action_fight || actor_action == actor_action_uncover )
                        firing_position_group_mask |= actor_get_firing_position_group(actor_index, _firing_point_evaluation_mode_fight, _firing_position_group_normal);
                }
                else if ( actor_mode == _actor_mode_alert
                          && actor->state.action_data.___u0.alert.move_position_order )  /* alert arm of the action_data union (0x9C); mode-alert actors run the alert action */
                {
                    active_squad_mask |= 1 << actor->meta.squad_index;  /* meta.squad_index (0x3A) */
                }

                int prop_index = actor->target.target_prop_index;  /* interesting prop index */
                if ( prop_index != -1 )
                {
                    /* prop.body_location.cluster_index (prop stride 312, field 0x100) */
                    int16_t prop_cluster = DATUM_GET(prop_data, prop_datum, prop_index)->body_location.cluster_index;
                    if ( prop_cluster != -1 )
                        BIT_VECTOR_SET_FLAG(bit_vector, prop_cluster);
                }
            }
        }

        if ( update_dormancy )
            /* squad_array[actor.squad_index + encounter.squad_base].disable_dormant pins the actor active */
            actor->meta.dormant_desire = squad_array[(int16_t)(actor->meta.squad_index + encounter->squad_base)].disable_dormant
                            ? 0
                            : actor_in_active_area;

        actor_index = actor->meta.next_actor_index;  /* meta.next_actor_index (0x2C) */
    }
    while ( actor_index != -1 );

    /* firing-position groups: each scenario firing position (24-byte) whose group bit is set contributes its
     * cluster (firing position @ +14), gated by group index @ +12. encounter_def[38]=count, [39]=address. */
    if ( firing_position_group_mask && encounter_def->firing_positions.count > 0 )
    {
        for ( int i = 0; i < encounter_def->firing_positions.count; i = (int16_t)(i + 1) )
        {
            firing_position_definition *firing_position = (firing_position_definition *)encounter_def->firing_positions.address + i;
            if ( (uint16_t)firing_position->cluster_index != 0xFFFF
                 && ((1 << firing_position->group_index) & firing_position_group_mask) != 0 )
                BIT_VECTOR_SET_FLAG(bit_vector, firing_position->cluster_index);
        }
    }

    /* active squads: for each squad (232-byte) whose bit is set, OR in every firing point's cluster
     * (firing points stride 80, cluster @ +40; squad: count @ +196, array @ +200). encounter_def[32]=squads.count,
     * [33]=squads.address. */
    if ( active_squad_mask && encounter_def->squads.count > 0 )
    {
        for ( int squad = 0; squad < encounter_def->squads.count; squad = (int16_t)(squad + 1) )
        {
            if ( ((1 << squad) & active_squad_mask) != 0 )
            {
                squad_definition *squad_def = (squad_definition *)encounter_def->squads.address + squad;
                if ( squad_def->move_positions.count > 0 )
                {
                    for ( int point = 0; point < squad_def->move_positions.count; point = (int16_t)(point + 1) )
                    {
                        int16_t cluster = (uint16_t)((move_position_definition *)squad_def->move_positions.address + point)->cluster_index;
                        if ( cluster != -1 )
                            BIT_VECTOR_SET_FLAG(bit_vector, cluster);
                    }
                }
            }
        }
    }
}
