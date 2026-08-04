/* ai_scripting_vehicle_enterable_team @0x83772338 — allow actors of the given team to enter a vehicle (set
 * its bit in the AI-enterable record's team bitmask). */

#include <stdint.h>
#include "headers/ai_vehicle_enterable.h"

extern ai_vehicle_enterable * ai_scripting_find_vehicle_enterable(int vehicle_index);

void ai_scripting_vehicle_enterable_team(int unit_index, int16_t team_index)
{
    char team = team_index;
    if ( unit_index != -1 )
    {
        ai_vehicle_enterable *vehicle_enterable = ai_scripting_find_vehicle_enterable(unit_index);
        if ( vehicle_enterable )
            vehicle_enterable->team_bitmask |= 1 << team;
    }
}
