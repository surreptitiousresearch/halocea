/* object_type_should_force_baseline_update @0x837028D0 — checks and clears an object's pending
 * force-baseline-update flag (bit 0x4000000), and separately tracks a network-at-rest transition (via
 * `was_network_at_rest`) that also forces a baseline update the first time it's observed while flag 0x20
 * is set. Returns whether either condition demands a forced baseline resend. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_should_force_baseline_update(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    uint8_t result = 0;
    if ( object->object.flags & (1u << _object_force_baseline_update_bit) )
    {
        result = 1;
        object->object.flags &= ~(1u << _object_force_baseline_update_bit);
    }

    if ( object->object.network_at_rest && (object->object.flags & (1u << _object_at_rest_bit)) )
    {
        if ( !object->object.was_network_at_rest )
            result = 1;
        object->object.was_network_at_rest = 1;
    }
    else
    {
        object->object.was_network_at_rest = 0;
    }

    return result;
}
