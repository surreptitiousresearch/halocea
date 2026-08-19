/* ai_scripting_force_active @0x83771CB8 — force an encounter active (or release the force) regardless of
 * the normal activation rules (runtime encounter datum +0xC). Bounds-checked against the scenario encounter
 * list and gated on AI being initialized for the map. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


void ai_scripting_force_active(int ai_index, uint8_t active)
{
    if ( ai_globals->ai_initialized_for_map
      && ai_index != -1
      && (uint16_t)ai_index < global_scenario->ai_encounters.count )
    {
        DATUM_GET(encounter_data, encounter_datum, ai_index)->force_active = active;
    }
}
