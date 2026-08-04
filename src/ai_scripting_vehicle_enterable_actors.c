/* ai_scripting_vehicle_enterable_actors @0x837723E8 — allow a specific AI (encounter) to enter a vehicle by
 * appending its index to the AI-enterable record's allow-list (capacity 6). */

#include "headers/ai_vehicle_enterable.h"

extern ai_vehicle_enterable * ai_scripting_find_vehicle_enterable(int vehicle_index);

void ai_scripting_vehicle_enterable_actors(int unit_index, int ai_index)
{
    if ( unit_index != -1 && ai_index != -1 )
    {
        ai_vehicle_enterable *vehicle_enterable = ai_scripting_find_vehicle_enterable(unit_index);
        if ( vehicle_enterable )
        {
            int count = vehicle_enterable->ai_indices_count;
            if ( count < 6 )
            {
                vehicle_enterable->ai_indices[count] = ai_index;
                ++vehicle_enterable->ai_indices_count;
            }
        }
    }
}
