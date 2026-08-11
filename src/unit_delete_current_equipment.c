/* unit_delete_current_equipment @0x836CB2B0 — immediately delete the equipment object a unit is
 * currently holding (unit.equipment_object_index), and clear the slot. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void object_delete(int object_index);

void unit_delete_current_equipment(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int equipment_index = unit->unit.equipment_object_index;
    if ( equipment_index != -1 )
    {
        object_delete(equipment_index);
        unit->unit.equipment_object_index = -1;
    }
}
