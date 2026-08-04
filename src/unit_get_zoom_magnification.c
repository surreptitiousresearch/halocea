/* unit_get_zoom_magnification @0x836D1A20 — magnification of the unit's current weapon at the given
 * zoom level, or 1.0 when the unit has no current weapon. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern float weapon_get_zoom_magnification(int weapon_index, int16_t zoom_level);

float unit_get_zoom_magnification(int unit_index, int16_t zoom_level)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int16_t weapon_slot = unit->unit.current_weapon_index;
    if ( weapon_slot != -1 )
    {
        int weapon_object_index = unit->unit.weapon_object_indices[weapon_slot];
        if ( weapon_object_index != -1 )
            return weapon_get_zoom_magnification(weapon_object_index, zoom_level);
    }
    return 1.0f;
}
