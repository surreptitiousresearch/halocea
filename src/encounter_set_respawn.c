/* encounter_set_respawn @0x8370DAE8 — set an encounter's "respawn" flag (+0x3C), force its activation timer
 * (+0xE) to 150 ticks, and activate it. No-op until the AI system is initialized for the map. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_activate(int encounter_index);

void encounter_set_respawn(int encounter_index, uint8_t respawn)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter->respawn_enabled = respawn;
    encounter->remain_active_timer = 150;
    encounter_activate(encounter_index);
}
