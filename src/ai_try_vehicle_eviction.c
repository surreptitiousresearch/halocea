/* ai_try_vehicle_eviction @0x836E8868 — decide whether an actor may evict a passenger to take a vehicle
 * seat. Allowed when the requesting unit is player-controlled (unit.player_index != -1) and is not on an
 * enemy team relative to the actor (requesting unit object.owner_team_index vs actor meta.team_index). If
 * actually_evict is set, raise the eviction stimulus. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void actor_stimulus_vehicle_eviction(uint16_t actor_index);

uint8_t ai_try_vehicle_eviction(int actor_index, int requesting_unit_index, uint8_t actually_evict)
{
    int evicted = 0;
    if ( requesting_unit_index != -1 )
    {
        unit_datum *requesting_unit = (unit_datum *)DATA_ARRAY_ELEMENT(
            object_header_data, object_header_datum, requesting_unit_index)->datum;
        if ( requesting_unit->unit.player_index != -1
          && !game_team_is_enemy(requesting_unit->object.owner_team_index,
                                 DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.team_index) )
        {
            evicted = 1;
            if ( actually_evict )
                actor_stimulus_vehicle_eviction(actor_index);
        }
    }
    return evicted;
}
