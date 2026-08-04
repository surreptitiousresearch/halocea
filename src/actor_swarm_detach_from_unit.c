/* actor_swarm_detach_from_unit @0x8371D850 — remove one member unit from a swarm actor. If the unit is still
 * controlled by this actor, re-enable its auto-deactivation, drop it from the swarm's unit/component arrays
 * (swap-removing with the last entry and freeing the swarm component datum), unlink it from the swarm's object
 * doubly-linked list (unit.swarm_next/prev_unit_index), clear unit.swarm_actor_index and decrement the
 * actor's swarm member count. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/swarm_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void datum_delete(data_array *data, int index);

void actor_swarm_detach_from_unit(uint16_t actor_index, int unit_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    if ( object->unit.swarm_actor_index != actor_index )
        return;

    object_set_automatic_deactivation(unit_index, 1u);
    unit_set_actively_controlled(unit_index, 0);

    if ( actor->meta.swarm_cache_index != -1 )  /* swarm cache index */
    {
        swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
        int count = swarm->unit_count;
        if ( count > 0 )
        {
            int i = 0;
            while ( swarm->unit_indices[i] != unit_index )
            {
                i = (int16_t)(i + 1);
                if ( i >= swarm->unit_count )
                    goto unlink;
            }
            int16_t new_count = (int16_t)(count - 1);
            int component = swarm->component_indices[i];
            swarm->unit_count = new_count;
            if ( i < new_count )
            {
                swarm->unit_indices[i] = swarm->unit_indices[new_count];
                swarm->component_indices[i] = swarm->component_indices[swarm->unit_count];
            }
            datum_delete(swarm_component_data, component);
        }
    }

unlink:
    if ( object->unit.swarm_prev_unit_index == -1 )
        actor->meta.swarm_unit_index = object->unit.swarm_next_unit_index;
    else
        (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object->unit.swarm_prev_unit_index)->datum))
            ->unit.swarm_next_unit_index = object->unit.swarm_next_unit_index;
    if ( object->unit.swarm_next_unit_index != -1 )
        (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object->unit.swarm_next_unit_index)->datum))
            ->unit.swarm_prev_unit_index = object->unit.swarm_prev_unit_index;
    object->unit.swarm_actor_index = -1;
    --actor->meta.swarm_unit_count;
}
