/* unit_get_current_grenade_type @0x836CB640 — return the unit's currently-selected grenade type (unit
 * object byte at +0x31C). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int16_t unit_get_current_grenade_type(int unit_index)
{
    unit_datum *unit_object =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return unit_object->unit.current_grenade_index;
}
