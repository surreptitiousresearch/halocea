/* unit_get_current_weapon_label @0x836CFB88 — return the marker label of the weapon the unit is currently
 * holding ("unarmed" if it has no weapon slot or no weapon in that slot). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern const char *weapon_get_label(int weapon_index);

const char *unit_get_current_weapon_label(int unit_index)
{
    unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    __int16 weapon_slot = object_data->unit.current_weapon_index;
    if ( weapon_slot == -1 )
        return "unarmed";
    int weapon_object_index = object_data->unit.weapon_object_indices[weapon_slot];
    if ( weapon_object_index == -1 )
        return "unarmed";
    return weapon_get_label(weapon_object_index);
}
