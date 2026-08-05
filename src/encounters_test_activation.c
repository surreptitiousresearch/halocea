/* encounters_test_activation @0x8370AB68 — per-frame visibility/activation pass over actors and encounters.
 *
 * Part 1: for every encounterless actor, determine whether it (or, for swarms / parented objects, any of
 * its constituent units) lies in the players' combined potential-visibility set. Actors that are visible
 * (and not forced active by the editor / swarm flag) get a short dormancy delay before deactivating;
 * once the delay expires they are set inactive. Visible actors activate immediately.
 *
 * Part 2: for every encounter, test whether its activation cluster bit-vector intersects the players'
 * combined PVS (only when the encounter is in the current BSP). Visible — or editor/forced/timed —
 * encounters activate with a 150-tick hold; otherwise the hold decays and, once expired, the encounter
 * activates only if a linked encounter is still active, else deactivates.
 *
 * Reconstructed from the decompiler. PPC idioms reduced: `(_cntlzw(x) & 0x20) != 0` -> `(x == 0)`,
 * and `(-x & ~x) < 0` -> `(x > 0)`.
 */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/object_header_datum.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/unit_datum.h"
#include "headers/swarm_datum.h"
#include "headers/structure_bsp.h"
#include "headers/data_iterator.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


extern unsigned int *players_get_combined_pvs(void);
extern int object_get_ultimate_parent(int object_index);
extern uint8_t game_in_editor(void);
extern int actor_set_active(uint16_t actor_index, uint8_t active);
extern void encounter_compute_activation_cluster_bit_vector(uint16_t encounter_index, uint8_t update_actor_dormancy, int bit_vector_size, const unsigned int *active_area, unsigned int *bit_vector);
extern uint8_t bit_vector_and(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result);
extern uint8_t encounter_activate(int encounter_index);
extern void encounter_deactivate(int encounter_index);

void encounters_test_activation(void)
{
    structure_bsp *bsp;
    const unsigned int *combined_pvs;
    int actor_index;
    actor_datum *actor;
    data_iterator encounter_iter;
    unsigned int activation_bits[44];
    int16_t *result;
    int16_t *encounter;

    bsp = global_structure_bsp;
    combined_pvs = players_get_combined_pvs();

    for (actor_index = ai_globals->first_encounterless_actor_index;
         actor_index != -1;
         actor_index = actor->meta.next_actor_index)
    {
        uint8_t active;
        char swarm_flag;
        actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

        if (actor->meta.swarm)           /* actor has a swarm or parent grouping */
        {
            int swarm_index = actor->meta.swarm_cache_index;
            actor->meta.dormant_desire = 1;       /* assume not visible until proven otherwise */
            if (swarm_index == -1)
            {
                int object_index = actor->meta.swarm_unit_index;
                if (object_index == -1)
                    goto resolve_actor;
                {
                    while (1)
                    {
                        unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                        int parent = object_get_ultimate_parent(object_index);
                        int cluster = (uint16_t)((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum)->object.location.cluster_index;
                        /* decompiler spelled the word index (__int16)cluster>>5; same value (cluster index < 0x8000) */
                        if (cluster != 0xFFFF && BIT_VECTOR_TEST_FLAG(combined_pvs, cluster))
                            break;
                        object_index = object_data->unit.swarm_next_unit_index;
                        if (object_index == -1)
                            goto resolve_actor;
                    }
                }
            }
            else
            {
                swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, swarm_index);
                if (swarm->unit_count <= 0)
                    goto resolve_actor;
                {
                    int unit = 0;
                    while (1)
                    {
                        int parent = object_get_ultimate_parent(swarm->unit_indices[unit]);
                        int cluster = (uint16_t)(((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum))->object.location.cluster_index;
                        /* decompiler spelled the word index (__int16)cluster>>5; same value (cluster index < 0x8000) */
                        if (cluster != 0xFFFF && BIT_VECTOR_TEST_FLAG(combined_pvs, cluster))
                            break;
                        unit = (int16_t)(unit + 1);
                        if (unit >= swarm->unit_count)
                            goto resolve_actor;
                    }
                }
            }
            actor->meta.dormant_desire = 0;       /* visible */
        }
        else
        {
            int parent = object_get_ultimate_parent(actor->meta.unit_index);
            int16_t cluster = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum))->object.location.cluster_index;
            /* (_cntlzw(x) & 0x20) != 0  ==  (x == 0) */
            actor->meta.dormant_desire = cluster == -1 || !BIT_VECTOR_TEST_FLAG(combined_pvs, cluster);
        }

resolve_actor:
        swarm_flag = actor->meta.force_active;
        /* visible OR (editor || swarm flag): force active and reset the dormancy delay */
        if (actor->meta.dormant_desire == 0 || (game_in_editor() | swarm_flag))
        {
            active = 1;
            actor->meta.encounterless_active_timer = 90;
        }
        else
        {
            char delay_expired = 0;
            int delay = actor->meta.encounterless_active_timer;
            if (delay <= 30)
            {
                delay_expired = 1;
                actor->meta.encounterless_active_timer = 0;
            }
            else
            {
                actor->meta.encounterless_active_timer = delay - 30;
            }
            if (!delay_expired)
                continue;
            active = 0;
            actor->meta.encounterless_active_timer = 0;
        }
        actor_set_active(actor_index, active);
    }

    /* Part 2: encounter activation */
    data_iterator_new(&encounter_iter, encounter_data);
    result = data_iterator_next(&encounter_iter);
    for (encounter = result; result; encounter = result)
    {
        encounter_datum *enc = (encounter_datum *)encounter;
        char force_active = enc->force_active;
        encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_iter.index;
        uint8_t in_editor = game_in_editor();
        int16_t def_bsp = encounter_def->runtime_structure_bsp_reference_index;
        /* (-x & ~x) < 0  ==  (x > 0) */
        char forced = in_editor | force_active | (enc->respawn_delay_ticks > 0);
        char visible;

        if (def_bsp == -1 || def_bsp == global_structure_bsp_index)
        {
            encounter_compute_activation_cluster_bit_vector(encounter_iter.index, 1u, 512, combined_pvs, activation_bits);
            visible = bit_vector_and(bsp->clusters.count, combined_pvs, activation_bits, 0) | forced;
        }
        else
        {
            visible = 0;
        }

        if (visible)
        {
            enc->remain_active_timer = 150;
            encounter_activate(encounter_iter.index);
        }
        else if (enc->active && enc->remain_active_timer > 30)
        {
            enc->remain_active_timer -= 30;
        }
        else
        {
            char linked_active = 0;
            if (enc->link_encounter_count > 0)
            {
                int n = 0;
                do
                {
                    if (DATUM_GET(encounter_data, encounter_datum,
                            enc->link_encounter_indices[n])->remain_active_timer > 0)
                        linked_active = 1;
                    n = (int16_t)(n + 1);
                } while (n < enc->link_encounter_count);
            }
            enc->remain_active_timer = 0;
            if (linked_active)
                encounter_activate(encounter_iter.index);
            else
                encounter_deactivate(encounter_iter.index);
        }
        result = data_iterator_next(&encounter_iter);
    }
}
