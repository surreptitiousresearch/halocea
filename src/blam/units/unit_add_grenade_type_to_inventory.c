/* unit_add_grenade_type_to_inventory @0x836CB170 — add grenade_count grenades of the given type to a unit's
 * inventory, and make that type the current/selected grenade. Returns the new count of that grenade type. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int16_t unit_add_grenade_type_to_inventory(int unit_index, int16_t grenade_type, int16_t grenade_count)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit->unit.grenade_counts[grenade_type] += grenade_count;
    unit->unit.desired_grenade_index = grenade_type;
    unit->unit.current_grenade_index = grenade_type;
    return unit->unit.grenade_counts[grenade_type];
}
