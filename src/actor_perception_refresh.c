/* actor_perception_refresh @0x837DB2A0 — rebuild an actor's perception of the units around it.
 *
 * Phase 1: build the potentially-visible-set (PVS) cluster bit vector — from every member of the actor's
 *   swarm if it belongs to one, otherwise from the actor's own cluster.
 * Phase 2: walk the actor's existing props; for each one still in use, re-test whether the actor still
 *   desires it (actor_perception_desire_prop) and whether its unit is in a visible cluster. Mark the
 *   visited objects (and the whole swarm behind a prop's swarm actor) so phase 3 skips them; count each
 *   surviving prop into the enemy/friend environment, or delete props that are no longer desired.
 * Phase 3: for every visible cluster, test its collideable and non-collideable objects as new perception
 *   candidates (actor_perception_refresh_test_object), filling the optional lists.
 * Phase 4: sort each environment's optional candidates and promote the best up to the per-side cap
 *   (4 enemies; enemies+friends bounded), creating new unacknowledged props and deleting the rest.
 *
 * DEVIATION: the decompiler rendered actor_perception_desire_prop's last three arguments as uninitialised
 * stack slots; disasm (0x837DB440) shows they are prop->dead_ticks, prop->required_ticks, and a pointer to a
 * zero-initialised local (the optional_reference output, whose result is discarded here). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/object_cluster_iterator.h"
#include "headers/actor_prop_environment.h"
#include "headers/actor_position_data.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/swarm_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include <stdlib.h>
#include <string.h>
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"



extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern void bit_vector_or(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result);
extern void object_marker_begin(void);
extern void object_marker_end(void);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actor_perception_desire_prop(int actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern int16_t object_get_first_cluster(object_cluster_iterator *iterator, int object_index);
extern int16_t object_get_next_cluster(object_cluster_iterator *iterator, int object_index);
extern int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_collideable_object(int *reference_index);
extern int cluster_get_first_noncollideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_noncollideable_object(int *reference_index);
extern void actor_perception_refresh_test_object(int actor_index, int object_index, actor_prop_environment *enemies, actor_prop_environment *friends);
extern int actor_perception_qsort_compare_optional_props(const void *p1, const void *p2);
extern int prop_new_unacknowledged(int actor_index, int unit_index, uint8_t enemy);
extern void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position, uint8_t forced_position_update, uint8_t full_status_update);
extern void actor_switch_props(int actor_index, int prop_index, int replacement_prop_index);
extern void prop_delete(int actor_index, int prop_index);

void actor_perception_refresh(int actor_index)
{
    structure_bsp *bsp = global_structure_bsp;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_prop_environment enemies;
    actor_prop_environment friends;
    enemies.optional_count = 0;
    enemies.existing_count = 0;
    friends.optional_count = 0;
    friends.existing_count = 0;

    unsigned int *pvs = 0;
    unsigned int pvs_buffer[16];

    if ( actor->meta.swarm )
    {
        /* actor is in a swarm: union the PVS of every swarm member's cluster */
        char pvs_valid = 0;
        memset(pvs_buffer, 0, sizeof(pvs_buffer));
        swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
        for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
        {
            int cluster = (uint16_t)((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, swarm->unit_indices[i])->datum)->object.location.cluster_index;
            if ( cluster != 0xFFFF )
            {
                unsigned int *cluster_pvs = structure_bsp_get_cluster_pvs(bsp, (int16_t)cluster);
                bit_vector_or(bsp->clusters.count, cluster_pvs, pvs_buffer, pvs_buffer);
                pvs_valid = 1;
            }
        }
        if ( pvs_valid )
            pvs = pvs_buffer;
    }
    else
    {
        int cluster = (uint16_t)actor->input.position.body_location.cluster_index;   /* DEVIATION: int16_t field sign-extended into the int local, so the `!= 0xFFFF` guard below was always TRUE and pvs was fetched for cluster -1; binary zero-extends, lhz r4,0x148(r27) @0x837DB3E0 + cmplwi cr6,r4,0xFFFF @0x837DB3E4 (matches the cast the swarm arm above already carries) */
        if ( cluster != 0xFFFF )
            pvs = structure_bsp_get_cluster_pvs(global_structure_bsp, (int16_t)cluster);
    }

    object_marker_begin();

    prop_iterator prop_iter;
    object_cluster_iterator cluster_iter;
    actor_position_data sense_position;

    prop_iterator_new(&prop_iter, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&prop_iter); prop; prop = prop_iterator_next(&prop_iter) )
    {
        int state = prop->state;
        if ( state >= _prop_state_uninspected_orphan && state <= _prop_state_inspected_orphan )
            continue;  /* prop is being deleted */

        uint8_t optional_reference = 0;
        uint8_t desired = actor_perception_desire_prop(
            actor_index, -1, prop->unit_index, prop->actor_index, prop->in_use, prop->player, prop->enemy,
            prop->dead, prop->dead_ticks, prop->suicide_radius, prop->distance * prop->distance,
            prop->required_ticks, &optional_reference);

        if ( desired && pvs )
        {
            /* keep the prop only if its unit occupies a visible cluster */
            desired = 0;
            for ( int cluster = object_get_first_cluster(&cluster_iter, prop->unit_index);
                  cluster != -1;
                  cluster = object_get_next_cluster(&cluster_iter, prop->unit_index) )
            {
                if ( BIT_VECTOR_TEST_FLAG(pvs, cluster) )
                {
                    desired = 1;
                    break;
                }
            }
        }

        /* mark the objects behind this prop's swarm actor so phase 3 skips them */
        if ( prop->swarm && prop->actor_index != -1 )
        {
            actor_datum *prop_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
            int prop_actor_swarm = prop_actor->meta.swarm_cache_index;
            if ( prop_actor_swarm == -1 )
            {
                int child = actor->meta.swarm_unit_index;
                if ( child != -1 )
                {
                    int marker = global_object_marker;
                    do
                    {
                        unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
                        if ( object->object.magic_number != marker )
                        {
                            object->object.magic_number = marker;
                            marker = global_object_marker;
                        }
                        child = object->unit.swarm_next_unit_index;
                    }
                    while ( child != -1 );
                }
            }
            else
            {
                swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, prop_actor_swarm);
                if ( swarm->unit_count > 0 )
                {
                    int marker = global_object_marker;
                    for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
                    {
                        unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, swarm->unit_indices[i])->datum);
                        if ( object->object.magic_number != marker )
                        {
                            object->object.magic_number = marker;
                            marker = global_object_marker;
                        }
                    }
                }
            }
        }

        /* mark the prop's own unit object */
        int object_marker = global_object_marker;
        unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum);
        if ( unit_object->object.magic_number != object_marker )
            unit_object->object.magic_number = object_marker;

        if ( desired )
        {
            actor_prop_environment *environment = prop->enemy ? &enemies : &friends;
            if ( !prop->dead )
                ++environment->existing_count;
        }
        else
        {
            /* no longer desired: delete the prop (and its orphan, unless in a delete state) */
            if ( prop->state < _prop_state_uninspected_orphan || prop->state > _prop_state_inspected_orphan )
            {
                if ( prop->___u3.orphan_prop_index != -1 )
                {
                    actor_switch_props(actor_index, prop->___u3.orphan_prop_index, -1);
                    prop_delete(actor_index, prop->___u3.orphan_prop_index);
                }
            }
            actor_switch_props(actor_index, prop_iter.index, -1);
            prop_delete(actor_index, prop_iter.index);
        }
    }

    /* phase 3: test every object in each visible cluster as a new perception candidate */
    if ( pvs )
    {
        for ( int16_t cluster = 0; cluster < bsp->clusters.count; cluster = (int16_t)(cluster + 1) )
        {
            if ( BIT_VECTOR_TEST_FLAG(pvs, cluster) )
            {
                for ( int object = cluster_get_first_collideable_object(&prop_iter.index, cluster);
                      object != -1;
                      object = cluster_get_next_collideable_object(&prop_iter.index) )
                    actor_perception_refresh_test_object(actor_index, object, &enemies, &friends);
                for ( int object = cluster_get_first_noncollideable_object(&prop_iter.index, cluster);
                      object != -1;
                      object = cluster_get_next_noncollideable_object(&prop_iter.index) )
                    actor_perception_refresh_test_object(actor_index, object, &enemies, &friends);
            }
        }
    }

    /* phase 4a: promote enemy candidates (cap of 4) */
    if ( enemies.optional_count > 0 )
    {
        int stop_index = 0;
        if ( enemies.existing_count < 4 )
        {
            qsort(enemies.optional_props, enemies.optional_count, sizeof(actor_optional_prop), actor_perception_qsort_compare_optional_props);
            if ( enemies.optional_count > 0 )
            {
                int i = 0;
                while ( 1 )
                {
                    if ( enemies.optional_props[i].prop_index == -1 )
                    {
                        int new_prop = prop_new_unacknowledged(actor_index, enemies.optional_props[i].unit_index, 1u);
                        if ( new_prop == -1 )
                        {
                            /* creation failed: advance without counting toward the cap */
                            stop_index = (int16_t)(i + 1);
                            i = stop_index;
                            if ( stop_index >= enemies.optional_count )
                                goto enemies_delete;
                            continue;
                        }
                        prop_position_refresh(actor_index, new_prop, &sense_position, 0, 0);
                    }
                    ++enemies.existing_count;
                    if ( enemies.existing_count >= 4 )
                        goto enemies_delete;
                    stop_index = (int16_t)(i + 1);
                    i = stop_index;
                    if ( stop_index >= enemies.optional_count )
                        goto enemies_delete;
                }
            }
        }
enemies_delete:
        /* delete the candidates that were not promoted */
        for ( int i = (int16_t)stop_index; i < enemies.optional_count; i = (int16_t)(i + 1) )
        {
            if ( enemies.optional_props[i].prop_index != -1 )
            {
                prop_datum *candidate = DATA_ARRAY_ELEMENT(prop_data, prop_datum, enemies.optional_props[i].prop_index);
                int16_t candidate_state = candidate->state;   /* recovered: *((__int16 *)candidate + 18) */
                if ( candidate_state < _prop_state_uninspected_orphan || candidate_state > _prop_state_inspected_orphan )
                {
                    int orphan = candidate->___u3.orphan_prop_index;   /* recovered: *((int *)candidate + 3) */
                    if ( orphan != -1 )
                    {
                        actor_switch_props(actor_index, orphan, -1);
                        prop_delete(actor_index, candidate->___u3.orphan_prop_index);
                    }
                }
                actor_switch_props(actor_index, enemies.optional_props[i].prop_index, -1);
                prop_delete(actor_index, enemies.optional_props[i].prop_index);
            }
        }
    }

    /* phase 4b: promote friend candidates (cap: at least 4, at most enemies+2) */
    if ( friends.optional_count > 0 )
    {
        int stop_index = 0;
        int limit = enemies.existing_count + 2;
        if ( limit <= 4 )
            limit = 4;
        int promoted = (int16_t)(friends.existing_count + enemies.existing_count);
        if ( (int16_t)(friends.existing_count + enemies.existing_count) < (int16_t)limit )
        {
            qsort(friends.optional_props, friends.optional_count, sizeof(actor_optional_prop), actor_perception_qsort_compare_optional_props);
            if ( friends.optional_count > 0 )
            {
                int i = 0;
                while ( 1 )
                {
                    if ( friends.optional_props[i].prop_index == -1 )
                    {
                        int new_prop = prop_new_unacknowledged(actor_index, friends.optional_props[i].unit_index, 0);
                        if ( new_prop == -1 )
                        {
                            stop_index = (int16_t)(i + 1);
                            i = stop_index;
                            if ( stop_index >= friends.optional_count )
                                goto friends_delete;
                            continue;
                        }
                        prop_position_refresh(actor_index, new_prop, &sense_position, 0, 0);
                    }
                    promoted = (int16_t)(promoted + 1);
                    ++friends.existing_count;
                    if ( promoted >= (int16_t)limit )
                        goto friends_delete;
                    stop_index = (int16_t)(i + 1);
                    i = stop_index;
                    if ( stop_index >= friends.optional_count )
                        goto friends_delete;
                }
            }
        }
friends_delete:
        for ( int i = (int16_t)stop_index; i < friends.optional_count; i = (int16_t)(i + 1) )
        {
            if ( friends.optional_props[i].prop_index != -1 )
            {
                prop_datum *candidate = DATA_ARRAY_ELEMENT(prop_data, prop_datum, friends.optional_props[i].prop_index);
                int16_t candidate_state = candidate->state;   /* recovered: *((__int16 *)candidate + 18) */
                if ( candidate_state < _prop_state_uninspected_orphan || candidate_state > _prop_state_inspected_orphan )
                {
                    int orphan = candidate->___u3.orphan_prop_index;   /* recovered: *((int *)candidate + 3) */
                    if ( orphan != -1 )
                    {
                        actor_switch_props(actor_index, orphan, -1);
                        prop_delete(actor_index, candidate->___u3.orphan_prop_index);
                    }
                }
                actor_switch_props(actor_index, friends.optional_props[i].prop_index, -1);
                prop_delete(actor_index, friends.optional_props[i].prop_index);
            }
        }
    }

    object_marker_end();
}
