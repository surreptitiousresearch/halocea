/* ai_scripting_vehicle_enterable_distance @0x83772300 — set the enter-from radius for a vehicle's
 * AI-enterable record. */

#include "headers/ai_vehicle_enterable.h"

extern ai_vehicle_enterable * ai_scripting_find_vehicle_enterable(int vehicle_index);

void ai_scripting_vehicle_enterable_distance(int unit_index, float distance)
{
    if ( unit_index != -1 )
    {
        ai_vehicle_enterable *vehicle_enterable = ai_scripting_find_vehicle_enterable(unit_index);
        if ( vehicle_enterable )
            vehicle_enterable->radius = distance;
    }
}
