/* unit_get_current_flashlight_state @0x836CA978 — return whether a unit's flashlight is currently on (object
 * flags +0x204 bit 0x80000), or 0 for an invalid unit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


uint8_t unit_get_current_flashlight_state(int unit_index)
{
    if ( unit_index == -1 )
        return 0;
    unit_datum *object_data =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return (object_data->unit.flags >> _unit_integrated_light_on_bit) & 1;
}
