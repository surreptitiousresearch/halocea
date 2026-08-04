/* actor_flush_structure_indices @0x8371E1B8 — invalidate every cached structure-BSP surface/location index
 * an actor holds when the BSP switches: the pathfinding/perception surface indices (word +328, dwords
 * +324/+356/+804), the mode-conditional cached surfaces (dword +1040 when the movement-state word +1024
 * is 2; dword +1148 when word +1132 is 2), the destination surface (+1172), and — for swarm actors (meta
 * flag byte +6) — every component's surface index (dword +16 of each 64-byte swarm component, via the
 * swarm's component list at +88, count word +2). Ends by flushing the current action's cached indices. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


extern void actor_action_flush_structure_indices(uint16_t actor_index);

void actor_flush_structure_indices(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    unsigned __int16 movement_state = (unsigned __int16)actor->orders.move.destination.destination_type;
    /* recovered: *((_WORD*)(actor+328)) -> input.position.body_location.cluster_index; *((_DWORD*)(actor+324)) -> input.position.body_location.leaf_index */
    actor->input.position.body_location.cluster_index = -1;
    actor->input.position.body_location.leaf_index = -1;
    actor->input.pathfinding_surface_index = -1;
    actor->stimuli.combat_transition_guard_point_surface_index = -1;
    if ( movement_state == _destination_raw_location )
        actor->orders.move.destination.___u3.raw.surface_index = -1;
    if ( (unsigned __int16)actor->control.path.destination_orders.destination_type == _destination_raw_location )
        actor->control.path.destination_orders.___u3.raw.surface_index = -1;
    actor->control.path.destination.surface_index = -1;

    if ( actor->meta.swarm )
    {
        int swarm_index = actor->meta.swarm_cache_index;
        if ( swarm_index != -1 )
        {
            swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, swarm_index);
            for ( __int16 i = 0; i < swarm->unit_count; i++ )
            {
                int component_index = swarm->component_indices[i];
                DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, component_index)->surface_index = -1;
            }
        }
    }

    actor_action_flush_structure_indices(actor_index);
}
