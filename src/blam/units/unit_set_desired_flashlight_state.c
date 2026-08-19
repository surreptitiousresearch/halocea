/* unit_set_desired_flashlight_state @0x836CA928 — request a unit's flashlight turn on (flags +0x204 bit
 * 0x10000000) or off (bit 0x20000000). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_set_desired_flashlight_state(int unit_index, uint8_t state)
{
    if ( unit_index == -1 )
        return;
    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int flags = object_data->unit.flags;
    object_data->unit.flags = state ? (flags | (1u << _unit_desired_integrated_light_on_bit))
                                    : (flags | (1u << _unit_desired_integrated_light_off_bit));
}
