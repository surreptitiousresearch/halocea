/* ai_scripting_vehicle_enterable_disable @0x83772458 — remove a vehicle from the AI-enterable table,
 * compacting the table by moving the last entry into the freed slot.
 *
 * DEVIATION: the decompiler performs the compaction copy through the adjacent spatial_effects array (the
 * two arrays are contiguous in ai_globals); reconstructed here as a direct ai_vehicle_enterable copy. */

#include "headers/ai_globals.h"
#include "headers/ai_vehicle_enterable.h"
#include "headers/blam_data_globals.h"


void ai_scripting_vehicle_enterable_disable(int unit_index)
{
    if ( unit_index == -1 )
        return;

    int count = ai_globals->enterable_vehicle_count;
    if ( count <= 0 )
        return;

    int index = 0;
    while ( ai_globals->enterable_vehicles[index].vehicle_index != unit_index )
    {
        index = (__int16)(index + 1);
        if ( index >= count )
            return;
    }

    __int16 new_count = --ai_globals->enterable_vehicle_count;
    if ( index < new_count )
        ai_globals->enterable_vehicles[index] = ai_globals->enterable_vehicles[new_count];
}
