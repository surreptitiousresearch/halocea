/* ai_scripting_playfight @0x83772070 — set an encounter's playfight flag (+0x60), making its actors
 * "fight" without actually harming each other. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


void ai_scripting_playfight(int ai_index, uint8_t playfight)
{
    if ( ai_index != -1 )
        DATUM_GET(encounter_data, encounter_datum, ai_index)->playfighting = playfight;
}
