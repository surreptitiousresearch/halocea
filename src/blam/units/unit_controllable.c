/* unit_controllable @0x836C9BA8 — true if a unit is currently flagged controllable (bit 6 of
 * unit.flags). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


uint8_t unit_controllable(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    return (unit->unit.flags >> _unit_controllable_bit) & 1;
}
