/* actor_look_compute_prop_interest @0x837FA828 — score how visually interesting a prop is for idle/secondary
 * looking. Base interest comes from the prop's state (acknowledged enemy, fresh vs stale corpse, live enemy,
 * neutral); the actor's own unit gets zero. The score is then boosted by movement (quantized speed), a recent
 * shot, and a vehicle bonus, and attenuated with distance.
 *
 * Deviation: the decompiler returns *((float*)&v3+1) (a single/double register-aliasing artifact); the real
 * return value is the accumulated single-precision interest. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


float actor_look_compute_prop_interest(uint16_t actor_index, uint16_t prop_index)
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

    /* no DB enum found: prop_datum.quantized_speed is a runtime-quantized bucket index (char, no named domain) */
    switch ( (uint8_t)prop->quantized_speed )
    {
        case 1: interest = (weight * 0.5f) + interest; break;
        case 2: interest = weight + interest; break;
        case 3: interest = (weight * 2.0f) + interest; break;
    }
    if ( prop->shooting )
        interest = (weight * 2.0f) + interest;

    /* no DB enum found: prop_datum.quantized_distance is a runtime-quantized bucket index (char, no named domain) */
    switch ( (uint8_t)prop->quantized_distance )
    {
        case 1: interest = interest * 0.60000002f; break;
        case 3: interest = interest * 0.40000001f; break;
        case 4: interest = interest * 0.2f; break;
    }
    return interest;
}
