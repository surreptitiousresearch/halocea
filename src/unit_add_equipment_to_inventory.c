/* unit_add_equipment_to_inventory @0x836CB1C8 — give a unit a piece of equipment (the inventory slot
 * unit.equipment_object_index). In _unit_add_equipment_as_only_equipment mode any existing equipment is deleted first. Fails (returns 0) if the
 * slot is still occupied. Otherwise the equipment is disconnected from the map and hidden, a pickup is
 * registered for a local player holding it, it is marked as in the unit's inventory, and stored in the slot.
 * Returns 1 on success. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_add_equipment_mode.h"
#include "headers/blam_data_globals.h"

extern void object_delete(int object_index);
extern void object_disconnect_from_map(int object_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern int player_index_from_unit_index(int unit_index);
extern void equipment_handle_pickup(int equipment_index);
extern void item_in_unit_inventory(int item_index, int owner_unit_index);

uint8_t unit_add_equipment_to_inventory(int unit_index, int equipment_index, int16_t mode)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    int existing = unit->unit.equipment_object_index;
    if ( existing != -1 && mode == _unit_add_equipment_as_only_equipment )
    {
        object_delete(existing);
        unit->unit.equipment_object_index = -1;
    }
    if ( unit->unit.equipment_object_index != -1 )
        return 0;

    object_disconnect_from_map(equipment_index);
    object_set_visibility(equipment_index, 0);
    if ( player_index_from_unit_index(unit_index) != -1
      && (uint16_t)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index_from_unit_index(unit_index))->local_player_index != 0xFFFF )
    {
        equipment_handle_pickup(equipment_index);
    }
    item_in_unit_inventory(equipment_index, unit_index);
    unit->unit.equipment_object_index = equipment_index;
    return 1;
}
