/* ai_scripting_find_vehicle_enterable @0x83772238 — find the AI-enterable record for a vehicle, creating a
 * fresh one (cleared, default radius 8) if the vehicle is not yet registered and there is room (max 32).
 * Returns the record, or null for an invalid vehicle or a full table.
 *
 * DEVIATION: the decompiler clears the new entry through the adjacent spatial_effects array (the two arrays
 * are contiguous in ai_globals); reconstructed as a direct field clear of the new ai_vehicle_enterable. */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/ai_vehicle_enterable.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dest, int value, unsigned int count);

ai_vehicle_enterable * ai_scripting_find_vehicle_enterable(int vehicle_index)
{
    if ( vehicle_index == -1 )
        return nullptr;

    int count = ai_globals->enterable_vehicle_count;
    int index = 0;
    if ( count > 0 )
    {
        for ( index = 0; index < count; index = (int16_t)(index + 1) )
        {
            if ( ai_globals->enterable_vehicles[index].vehicle_index == vehicle_index )
                break;
        }
    }

    if ( (int16_t)index >= 32 )
        return nullptr;

    ai_vehicle_enterable *result = &ai_globals->enterable_vehicles[(int16_t)index];
    if ( (int16_t)index >= count )
    {
        memset(result, 0, sizeof(ai_vehicle_enterable));
        result->vehicle_index = vehicle_index;
        result->radius = 8.0f;
        ++ai_globals->enterable_vehicle_count;
    }
    return result;
}
