/* squad_reset_starting_locations @0x8370A220 — reset a squad's starting-location bitvectors from its scenario
 * definition: mark every location unused, then flag the "required" ones (scenario location flag bit 0).
 *
 * The scenario squad definition is reached through the encounter's squads block (encounter def dword 33 =
 * squads address; 232-byte squad stride); each starting location is 28 bytes, byte 19 bit 0 = required. */

#include <stdint.h>
#include <string.h>
#include "headers/squad_definition.h"
#include "headers/encounter_definition.h"
#include "headers/actor_starting_location_definition.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


void squad_reset_starting_locations(uint16_t encounter_index, int16_t squad_index)
{
    squad_definition *scenario_squad =
        &((squad_definition *)((encounter_definition *)global_scenario->ai_encounters.address)[encounter_index].squads.address)[squad_index];
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    squad_datum *squad = &squad_array[(int16_t)(encounter->squad_base + squad_index)];

    int location_count = scenario_squad->starting_locations.count;
    memset(squad->unused_locations, -1, 4 * BIT_VECTOR_SIZE_IN_LONGS(location_count));
    for ( int i = 0; i < location_count; i = (int16_t)(i + 1) )
    {
        if ( (((actor_starting_location_definition *)scenario_squad->starting_locations.address)[i].flags & 1) != 0 )
            BIT_VECTOR_SET_FLAG(squad->required_locations, i);
    }
}
