/* actor_look_compute_prop_interest @0x837FA828 — score how visually interesting a prop is for idle/secondary
 * looking. Base interest comes from the prop's state (acknowledged enemy, fresh vs stale corpse, live enemy,
 * neutral); the actor's own unit gets zero. The score is then boosted by movement (quantized speed), a recent
 * shot, and a vehicle bonus, and attenuated with distance.
 *
 * Deviation: the decompiler returns the high float of a fused 64-bit local (a single/double register-aliasing
 * artifact); the real return value is the accumulated single-precision interest. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/prop_distance.h"
#include "headers/prop_speed.h"
#include "headers/blam_data_globals.h"


float actor_look_compute_prop_interest(int actor_index, int prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    float interest = 0.0f;
    float weight = 1.0f;

    int state = prop->state;
    if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged )
    {
        if ( prop->enemy && state >= _prop_state_uninspected_orphan && state <= _prop_state_inspected_orphan )
            interest = 1.5f;
    }
    else if ( prop->dead )
    {
        interest = (prop->dead_ticks >= 210) ? 0.40000001f : 1.8f;
    }
    else if ( prop->enemy )  /* live enemy */
    {
        interest = 2.0f;
    }
    else
    {
        interest = 1.0f;
    }

    int self_unit = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->input.vehicle_index;
    if ( prop->unit_index == self_unit || prop->vehicle_index == self_unit )
        interest = 0.0f;
    if ( prop->vehicle_index != -1 )
        weight = 1.5f;

    /* prop_datum.quantized_speed domain: prop_speed.h (types_enum_values $2A7E5BA6EC108C666DF6E50CD8BAED3C);
     * producer prop_status_refresh assigns 0/1/2/3 by ascending speed thresholds (_prop_speed_stopped is 0). */
    switch ( (uint8_t)prop->quantized_speed )
    {
        case _prop_speed_walking: interest = (weight * 0.5f) + interest; break;
        case _prop_speed_running: interest = weight + interest; break;
        case _prop_speed_driving: interest = (weight * 2.0f) + interest; break;
    }
    if ( prop->shooting )
        interest = (weight * 2.0f) + interest;

    /* prop_datum.quantized_distance domain: prop_distance.h (types_enum_values $44ADBAE443330A02246AB7D2120EBFC3) */
    switch ( (uint8_t)prop->quantized_distance )
    {
        case _prop_distance_near: interest = interest * 0.60000002f; break;
        case _prop_distance_far: interest = interest * 0.40000001f; break;
        case _prop_distance_distant: interest = interest * 0.2f; break;
    }
    return interest;
}
