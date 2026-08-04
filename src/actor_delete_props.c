/* actor_delete_props @0x8371FF50 — delete every prop on the actor's prop list (actor +0x50 / dword[20] head;
 * prop_delete re-heads the list). For each prop, first scrub every actor field that still references that prop
 * index (targets, aim, look, search, charge, retreat, suppression, vehicle, etc.), clear any matching swarm
 * component reference, run actor_action_replace_prop to detach it from the action state, then free the prop. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/swarm_datum.h"
#include "headers/data_array.h"
#include "headers/actor_target_type.h"
#include "headers/actor_fire_target_type.h"
#include "headers/actor_postcombat_type.h"
#include "headers/destination_type.h"
#include "headers/direction_specification_type.h"
#include "headers/blam_data_globals.h"

extern void actor_action_replace_prop(uint16_t actor_index, int invalid_prop_index, int replacement_prop_index);
extern void prop_delete(int actor_index, int prop_index);

void actor_delete_props(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int prop_index = actor->meta.first_prop_index;
    if ( prop_index == -1 )
        return;

    while ( 1 )
    {
        actor_datum *a = actor;

        if ( a->target.target_prop_index == prop_index ) { a->target.target_prop_index = -1; a->target.target_type = actor_target_none; }
        if ( a->control.current_fire_target_type == actor_fire_target_prop && a->control.___u58.current_fire_target_prop_index == prop_index )
            { a->control.___u58.current_fire_target_prop_index = -1; a->control.current_fire_target_type = actor_fire_target_none; }
        if ( a->control.grenade_current_prop_index == prop_index ) a->control.grenade_current_prop_index = -1;
        if ( a->stimuli.surprise_prop_index == prop_index ) a->stimuli.surprise_prop_index = -1;
        if ( a->stimuli.panic_prop_index == prop_index ) a->stimuli.panic_prop_index = -1;
        if ( a->stimuli.combat_transition_prop_index == prop_index ) a->stimuli.combat_transition_prop_index = -1;
        if ( a->emotions.unopposable_retreat_prop_index == prop_index )
            { a->emotions.unopposable_retreat_timer = 0; a->emotions.unopposable_retreat_prop_index = -1; }
        if ( a->external_orders.pursuit_group_prop_index == prop_index ) a->external_orders.pursuit_group_prop_index = -1;
        if ( a->external_orders.postcombat_prop_index == prop_index )
            { a->external_orders.postcombat_prop_index = -1; a->external_orders.postcombat_type = actor_postcombat_none; }
        if ( a->control.path.destination_orders.destination_type == _destination_prop
          && a->control.path.destination_orders.___u3.prop.prop_index == prop_index )
            { a->control.path.destination_orders.destination_type = _destination_none; a->control.path.destination_orders.ignore_target_object_index = -1; }
        if ( a->control.secondary_look_direction.type == _direction_specification_prop && a->control.secondary_look_direction.___u1.prop_index == prop_index )
            a->control.secondary_look_direction.___u1.prop_index = -1;
        if ( a->control.idle_major_direction.type == _direction_specification_prop && a->control.idle_major_direction.___u1.prop_index == prop_index )
            a->control.idle_major_direction.___u1.prop_index = -1;
        if ( a->control.idle_minor_direction.type == _direction_specification_prop && a->control.idle_minor_direction.___u1.prop_index == prop_index )
            a->control.idle_minor_direction.___u1.prop_index = -1;

        if ( a->meta.swarm && a->meta.swarm_cache_index != -1 )
        {
            swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, a->meta.swarm_cache_index);
            if ( swarm->unit_count > 0 )
            {
                int i = 0;
                do
                {
                    swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                                    swarm->component_indices[i]);
                    if ( component->combat_target_prop_index == prop_index )
                        component->combat_target_prop_index = -1;
                    i = (__int16)(i + 1);
                }
                while ( i < swarm->unit_count );
            }
        }

        actor_action_replace_prop(actor_index, prop_index, -1);
        prop_delete(actor_index, actor->meta.first_prop_index);
        prop_index = actor->meta.first_prop_index;
        if ( prop_index == -1 )
            break;
    }
}
