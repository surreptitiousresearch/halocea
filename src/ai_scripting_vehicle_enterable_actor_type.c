/* ai_scripting_vehicle_enterable_actor_type @0x83772390 — allow actors of the given type to enter a vehicle
 * (set its bit in the AI-enterable record's actor-type bitmask). */

#include <stdint.h>
#include "headers/ai_vehicle_enterable.h"

extern ai_vehicle_enterable * ai_scripting_find_vehicle_enterable(int vehicle_index);

void ai_scripting_vehicle_enterable_actor_type(int unit_index, int16_t actor_type)
{
    char type = actor_type;
    if ( unit_index != -1 )
    {
        ai_vehicle_enterable *vehicle_enterable = ai_scripting_find_vehicle_enterable(unit_index);
        if ( vehicle_enterable )
            vehicle_enterable->actor_type_bitmask |= 1 << type;
    }
}
