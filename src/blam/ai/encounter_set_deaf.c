/* encounter_set_deaf @0x8370BFE8 — set an encounter's "deaf" flag (+0x41), suppressing its actors' hearing.
 * No-op until the AI system is initialized for the map. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"


void encounter_set_deaf(uint16_t encounter_index, uint8_t deaf)
{
    if ( ai_globals->ai_initialized_for_map )
        DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->deaf = deaf;
}
