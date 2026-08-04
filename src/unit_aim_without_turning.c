/* unit_aim_without_turning @0x836CCE78 — set whether a unit aims without turning its body
 * (unit_datum.unit.flags bit _unit_aim_without_turning_bit). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_aim_without_turning(int unit_index, uint8_t aim_without_turning)
{
    if ( unit_index == -1 )
        return;
    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int flags = object_data->unit.flags;
    object_data->unit.flags = aim_without_turning
        ? (flags | (1u << _unit_aim_without_turning_bit))
        : (flags & ~(1u << _unit_aim_without_turning_bit));
}
