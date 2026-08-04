/* encounter_attach_unit @0x83709748 — when the sandbox (non-game-engine) is running and the encounter has no team
 * yet, seed the encounter's team from the unit's team and, if the encounter had a valid first-actor, refresh team
 * allegiance status. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_running(void);
extern void ai_update_team_status(void);

void encounter_attach_unit(int encounter_index, int unit_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( !game_engine_running() && !encounter->team_index )
    {
        int first_actor = encounter->first_actor_index;
        encounter->team_index = unit->object.owner_team_index;  /* team */
        if ( first_actor != -1 )
            ai_update_team_status();
    }
}
