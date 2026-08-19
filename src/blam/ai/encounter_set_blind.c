/* encounter_set_blind @0x8370BFB0 — set an encounter's "blind" flag (+0x40), suppressing its actors' sight.
 * No-op until the AI system is initialized for the map. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"


void encounter_set_blind(uint16_t encounter_index, uint8_t blind)
{
    if ( ai_globals->ai_initialized_for_map )
        DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->blind = blind;
}
