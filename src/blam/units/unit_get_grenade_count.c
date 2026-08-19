/* unit_get_grenade_count @0x836CB5F8 — return how many grenades of the given type the unit holds (unit
 * object byte at +0x31E + grenade_type). Returns 0 for an invalid grenade type. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_grenade_count(int unit_index, int16_t grenade_type)
{
    if ( grenade_type == -1 )
        return 0;

    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return unit->unit.grenade_counts[grenade_type]; /* was *(char*)(datum + grenade_type + 798); _unit_datum.grenade_counts @0x12A */
}
