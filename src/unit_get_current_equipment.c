/* unit_get_current_equipment @0x836CB000 — the object index of the unit's currently held equipment (unit data
 * +0x318). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int unit_get_current_equipment(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return unit->unit.equipment_object_index;
}
