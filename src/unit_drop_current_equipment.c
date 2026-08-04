/* unit_drop_current_equipment @0x836CF548 — drop (eject into the world) the equipment object a unit is
 * currently holding (unit.equipment_object_index), and clear the slot. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void unit_drop_item(int unit_index, int item_index);

void unit_drop_current_equipment(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int equipment_index = unit->unit.equipment_object_index;
    if ( equipment_index != -1 )
    {
        unit_drop_item(unit_index, equipment_index);
        unit->unit.equipment_object_index = -1;
    }
}
