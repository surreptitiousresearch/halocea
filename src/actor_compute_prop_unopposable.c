/* actor_compute_prop_unopposable @0x837D6C88 — decide whether a prop represents an "unopposable" threat (a
 * live, acknowledged enemy that the actor currently cannot answer: unreachable while it is the actor's target,
 * a dangerous/gunner vehicle the actor cannot oppose, or a betrayal-class enemy). On transition out of the
 * unopposable state, clears the casualty/hysteresis counters and any pending unopposable retreat slot. */

#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_type.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


uint8_t actor_compute_prop_unopposable(uint16_t actor_index, int prop_index)
{
    prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
    __int16 state = prop->state;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 0;

    if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged && prop->enemy && !prop->dead
      && ((prop->unreachable_ticks && (actor->target.target_prop_index == prop_index || !actor->situation.visible_reachable_enemies))
          || ((prop->vehicle_gunner || prop->dangerous_vehicle_driver) && !actor->input.vehicle_gunner && !actor->situation.area_fire_support_friends)
          || prop->type == _actor_type_mounted_weapon) )
    {
        result = 1;
    }

    if ( prop->unopposable_enemy && !(unsigned char)result )
    {
        prop->unopposable_trigger_hysteresis = 0;
        prop->unopposable_trigger_threshold = 0;
        prop->unopposable_trigger_timer = 0;
    }

    if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged && !(unsigned char)result
      && actor->emotions.unopposable_retreat_timer > 0 && actor->emotions.unopposable_retreat_prop_index == prop_index )
    {
        actor->emotions.unopposable_retreat_timer = 0;
        actor->emotions.unopposable_retreat_prop_index = -1;
    }

    prop->unopposable_enemy = result;
    return result;
}
