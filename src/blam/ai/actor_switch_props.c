/* actor_switch_props @0x8371DF70 — repoint every reference an actor holds to a given prop over to a
 * replacement prop (or -1 when the prop is being removed). This covers the actor's target, engagement,
 * grenade, look, search, charge and other behaviour slots (each guarded by its mode word so stale slots are
 * left alone), plus every swarm component's target. Finally the action layer is updated.
 *
 * Accesses resolved to named actor_datum members (the decompiler's raw offsets are noted per site). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/actor_target_type.h"
#include "headers/actor_fire_target_type.h"
#include "headers/actor_postcombat_type.h"
#include "headers/destination_type.h"
#include "headers/direction_specification_type.h"
#include "headers/blam_data_globals.h"

extern void actor_action_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index);

void actor_switch_props(int actor_index, int prop_index, int replacement_prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->target.target_prop_index == prop_index )
    {
        actor->target.target_prop_index = replacement_prop_index;
        if ( replacement_prop_index == -1 )
            actor->target.target_type = actor_target_none;
    }
    if ( actor->control.current_fire_target_type == actor_fire_target_prop && actor->control.___u58.current_fire_target_prop_index == prop_index )
    {
        actor->control.___u58.current_fire_target_prop_index = replacement_prop_index;
        if ( replacement_prop_index == -1 )
            actor->control.current_fire_target_type = actor_fire_target_none;
    }
    if ( actor->control.grenade_current_prop_index == prop_index )
        actor->control.grenade_current_prop_index = replacement_prop_index;
    if ( actor->stimuli.surprise_prop_index == prop_index )
        actor->stimuli.surprise_prop_index = replacement_prop_index;
    if ( actor->stimuli.panic_prop_index == prop_index )
        actor->stimuli.panic_prop_index = replacement_prop_index;
    if ( actor->stimuli.combat_transition_prop_index == prop_index )
        actor->stimuli.combat_transition_prop_index = replacement_prop_index;
    if ( actor->emotions.unopposable_retreat_prop_index == prop_index )
    {
        if ( replacement_prop_index == -1 )
            actor->emotions.unopposable_retreat_timer = 0;
        actor->emotions.unopposable_retreat_prop_index = replacement_prop_index;
    }
    if ( actor->external_orders.pursuit_group_prop_index == prop_index )
        actor->external_orders.pursuit_group_prop_index = replacement_prop_index;
    if ( actor->external_orders.postcombat_prop_index == prop_index )
    {
        actor->external_orders.postcombat_prop_index = replacement_prop_index;
        if ( replacement_prop_index == -1 )
            actor->external_orders.postcombat_type = actor_postcombat_none;
    }
    if ( actor->control.path.destination_orders.destination_type == _destination_prop && actor->control.path.destination_orders.___u3.prop.prop_index == prop_index )
    {
        if ( replacement_prop_index == -1 )
        {
            actor->control.path.destination_orders.destination_type = _destination_none;
            actor->control.path.destination_orders.ignore_target_object_index = -1;
        }
        else
        {
            actor->control.path.destination_orders.___u3.prop.prop_index = replacement_prop_index;
        }
    }
    if ( actor->control.secondary_look_direction.type == _direction_specification_prop && actor->control.secondary_look_direction.___u1.prop_index == prop_index )
        actor->control.secondary_look_direction.___u1.prop_index = replacement_prop_index;
    if ( actor->control.idle_major_direction.type == _direction_specification_prop && actor->control.idle_major_direction.___u1.prop_index == prop_index )
        actor->control.idle_major_direction.___u1.prop_index = replacement_prop_index;
    if ( actor->control.idle_minor_direction.type == _direction_specification_prop && actor->control.idle_minor_direction.___u1.prop_index == prop_index )
        actor->control.idle_minor_direction.___u1.prop_index = replacement_prop_index;

    if ( actor->meta.swarm )
    {
        int swarm_cache_index = actor->meta.swarm_cache_index;
        if ( swarm_cache_index != -1 )
        {
            swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, swarm_cache_index);
            if ( swarm->unit_count > 0 )
            {
                for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
                {
                    swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                                    swarm->component_indices[i]);
                    if ( component->combat_target_prop_index == prop_index )
                        component->combat_target_prop_index = replacement_prop_index;
                }
            }
        }
    }

    actor_action_replace_prop(actor_index, prop_index, replacement_prop_index);
}
