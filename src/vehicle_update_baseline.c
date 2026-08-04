/* vehicle_update_baseline @0x8375E038 — snapshot a vehicle object's network-replicated state (15 opaque
 * dword fields at +92..+148) into its baseline copy (+1324..+1380), bump the baseline sequence byte
 * (+1318, wrapping via plain overflow) and mark the baseline valid/dirty (+1317 = 1, +1319 word = 1). */

#include "headers/vehicle_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void vehicle_update_baseline(int object_index)
{
    vehicle_datum *vehicle = object_try_and_get_and_verify_type(object_index, object_mask_vehicle);
    if ( !vehicle )
        return;

    /* snapshot the 15-dword replicated block (object.position .. object.up) into the baseline copy */
    int fields[15];
    for ( int i = 0; i < 15; ++i )
        fields[i] = ((int *)&vehicle->object.position)[i];

    unsigned char next_sequence = vehicle->vehicle.baseline_index + 1;

    vehicle->vehicle.baseline_valid = 1;
    vehicle->vehicle.baseline_index = next_sequence;
    vehicle->vehicle.message_index = 1;

    for ( int i = 0; i < 15; ++i )
        ((int *)&vehicle->vehicle.baseline.position)[i] = fields[i];
}
